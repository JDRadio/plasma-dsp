#include <AL/al.h>
#include <AL/alc.h>

#include <list>
#include <array>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

class AudioFrontend
{
public:
    AudioFrontend(void);
    ~AudioFrontend(void);

    size_t receive(float* in, size_t in_size);
    void transmit(const float* out, size_t out_size);

protected:
    void samplesToFloat(const uint8_t* in, size_t in_size, float* out);
    void floatToSamples(const float* in, size_t in_size, uint8_t* out);

protected:
    static constexpr ALCuint m_samplingRate = 44100;
    static constexpr ALCsizei m_captureBufferSize = m_samplingRate / 2;

    ALCdevice* m_outputDevice;
    ALCdevice* m_inputDevice;
    ALCcontext* m_context;
    list<ALuint> m_bufferQueue;
    array<ALuint, 16> m_buffers;
    ALuint m_source;
    array<uint8_t, 2048> m_inBuffer;
    array<uint8_t, 2048> m_outBuffer;
};

AudioFrontend::AudioFrontend(void)
{
    m_outputDevice = alcOpenDevice(nullptr);
    m_context = alcCreateContext(m_outputDevice, nullptr);
    alcMakeContextCurrent(m_context);

    alGenBuffers(m_buffers.size(), m_buffers.data());
    alGenSources(1, &m_source);

    m_bufferQueue.insert(end(m_bufferQueue), begin(m_buffers), end(m_buffers));

    m_inputDevice = alcCaptureOpenDevice(nullptr, m_samplingRate, AL_FORMAT_MONO8, m_captureBufferSize);
    alcCaptureStart(m_inputDevice);
}

AudioFrontend::~AudioFrontend(void)
{
    alcCaptureStop(m_inputDevice);
    alcCaptureCloseDevice(m_inputDevice);

    alSourceStopv(1, &m_source);
    alSourcei(m_source, AL_BUFFER, 0);

    alDeleteSources(1, &m_source);
    alDeleteBuffers(m_buffers.size(), m_buffers.data());

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_context);
    alcCloseDevice(m_outputDevice);
}

void AudioFrontend::samplesToFloat(const uint8_t* in, size_t in_size, float* out)
{
    for (size_t i = 0; i < in_size; i++) {
        out[i] = static_cast<float>(in[i]) / 128.f - 1.f;
    }
}

void AudioFrontend::floatToSamples(const float* in, size_t in_size, uint8_t* out)
{
    for (size_t i = 0; i < in_size; i++) {
        out[i] = static_cast<uint8_t>(round((in[i] + 1.f) * 128.f));
    }
}

void AudioFrontend::transmit(const float* out, size_t out_size)
{
    size_t sent = 0;

    while (sent < out_size) {
        size_t toSend = min(out_size - sent, m_outBuffer.size());
        ALint availBuffers = 0;
        alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &availBuffers);

        if (availBuffers > 0) {
            array<ALuint, 16> bufferHolder;
            alSourceUnqueueBuffers(m_source, availBuffers, bufferHolder.data());
            m_bufferQueue.insert(end(m_bufferQueue), begin(bufferHolder), begin(bufferHolder) + availBuffers);
        }

        while (m_bufferQueue.empty()) {
            this_thread::sleep_for(chrono::milliseconds(10));
            alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &availBuffers);

            if (availBuffers > 0) {
                array<ALuint, 16> bufferHolder;
                alSourceUnqueueBuffers(m_source, availBuffers, bufferHolder.data());
                m_bufferQueue.insert(end(m_bufferQueue), begin(bufferHolder), begin(bufferHolder) + availBuffers);
            }
        }

        ALuint myBuffer = m_bufferQueue.front();
        m_bufferQueue.pop_front();

        this->floatToSamples(out + sent, toSend, m_outBuffer.data());
        alBufferData(myBuffer, AL_FORMAT_MONO8, m_outBuffer.data(), toSend * sizeof(uint8_t), m_samplingRate);

        alSourceQueueBuffers(m_source, 1, &myBuffer);

        ALint sourceState = 0;
        alGetSourcei(m_source, AL_SOURCE_STATE, &sourceState);

        if (sourceState != AL_PLAYING) {
            alSourcePlay(m_source);
        }

        sent += toSend;
    }
}

size_t AudioFrontend::receive(float* in, size_t in_size)
{
    size_t retrieved = min(in_size, m_inBuffer.size());
    ALCint avail = 0;
    alcGetIntegerv(m_inputDevice, ALC_CAPTURE_SAMPLES, 1, &avail);

    do {
        this_thread::sleep_for(chrono::milliseconds(10));
        alcGetIntegerv(m_inputDevice, ALC_CAPTURE_SAMPLES, 1, &avail);
    }
    while (avail < static_cast<int>(retrieved));

    alcCaptureSamples(m_inputDevice, m_inBuffer.data(), retrieved);
    this->samplesToFloat(m_inBuffer.data(), retrieved, in);

    return retrieved;
}

int main(int argc, char* argv[])
{
    AudioFrontend* audio = new AudioFrontend();
    array<float, 2048> buffer;

    while (true) {
        size_t received = audio->receive(buffer.data(), buffer.size());
        audio->transmit(buffer.data(), received);
    }

    delete audio;

    return 0;
}
