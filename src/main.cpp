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
#include <complex>

#include <liquid/liquid.h>

using namespace std;

class AudioFrontend
{
public:
    AudioFrontend(void);
    ~AudioFrontend(void);

    size_t receive(int16_t* in, size_t in_size);
    size_t receive(float* in, size_t in_size);
    size_t receive(complex<float>* in, size_t in_size);
    void transmit(const int16_t* out, size_t out_size);
    void transmit(const float* out, size_t out_size);
    void transmit(const complex<float>* out, size_t out_size);

protected:
    void samplesToFloat(const int16_t* in, size_t in_size, float* out);
    void samplesToComplex(const int16_t* in, size_t in_size, complex<float>* out);
    void floatToSamples(const float* in, size_t in_size, int16_t* out);
    void complexToSamples(const complex<float>* in, size_t out_size, int16_t* out);

protected:
    static constexpr ALCuint m_samplingRate = 48000;
    static constexpr ALCsizei m_captureBufferSize = m_samplingRate / 2;

    ALCdevice* m_outputDevice;
    ALCdevice* m_inputDevice;
    ALCcontext* m_context;
    list<ALuint> m_bufferQueue;
    array<ALuint, 16> m_buffers;
    ALuint m_source;
    array<int16_t, 2048*2> m_inBuffer;
    array<int16_t, 2048*2> m_outBuffer;
};

AudioFrontend::AudioFrontend(void)
{
    m_outputDevice = alcOpenDevice(nullptr);
    m_context = alcCreateContext(m_outputDevice, nullptr);
    alcMakeContextCurrent(m_context);

    alGenBuffers(m_buffers.size(), m_buffers.data());
    alGenSources(1, &m_source);

    m_bufferQueue.insert(end(m_bufferQueue), begin(m_buffers), end(m_buffers));

    m_inputDevice = alcCaptureOpenDevice(nullptr, m_samplingRate, AL_FORMAT_STEREO16, m_captureBufferSize);
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

void AudioFrontend::samplesToFloat(const int16_t* in, size_t in_size, float* out)
{
    for (size_t i = 0; i < in_size; i++) {
        out[i] = static_cast<float>(in[i]) / 32768.f;
    }
}

void AudioFrontend::samplesToComplex(const int16_t* in, size_t in_size, complex<float>* out)
{
    for (size_t i = 0; i < in_size; i++) {
        out[i] = complex<float>(static_cast<float>(in[i*2+0]) / 32768.f, static_cast<float>(in[i*2+1]) / 32768.f);
    }
}

void AudioFrontend::floatToSamples(const float* in, size_t in_size, int16_t* out)
{
    for (size_t i = 0; i < in_size; i++) {
        out[i] = static_cast<int16_t>(round((in[i]) * 32768.f));
    }
}

void AudioFrontend::complexToSamples(const complex<float>* in, size_t in_size, int16_t* out)
{
    for (size_t i = 0; i < in_size; i++) {
        out[i*2+0] = static_cast<int16_t>(round((in[i].real()) * 32768.f));
        out[i*2+1] = static_cast<int16_t>(round((in[i].imag()) * 32768.f));
    }
}

void AudioFrontend::transmit(const complex<float>* out, size_t out_size)
{
	size_t sent = 0;

	while (sent < out_size) {
        size_t toSend = min(out_size - sent, m_outBuffer.size());
		this->complexToSamples(out + sent, toSend, m_outBuffer.data());
		this->transmit(m_outBuffer.data(), toSend);
		sent += toSend;
	}
}

void AudioFrontend::transmit(const float* out, size_t out_size)
{
	size_t sent = 0;

	while (sent < out_size) {
        size_t toSend = min(out_size - sent, m_outBuffer.size());
		this->floatToSamples(out + sent, toSend, m_outBuffer.data());
		this->transmit(m_outBuffer.data(), toSend);
		sent += toSend;
	}
}

void AudioFrontend::transmit(const int16_t* out, size_t out_size)
{
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

	alBufferData(myBuffer, AL_FORMAT_STEREO16, out, out_size * sizeof(int16_t) * 2, m_samplingRate);

	alSourceQueueBuffers(m_source, 1, &myBuffer);

	ALint sourceState = 0;
	alGetSourcei(m_source, AL_SOURCE_STATE, &sourceState);

	if (sourceState != AL_PLAYING) {
		alSourcePlay(m_source);
	}
}

size_t AudioFrontend::receive(complex<float>* in, size_t in_size)
{
    size_t toRetrieve = min(in_size, m_inBuffer.size() / 2);

	size_t retrieved = this->receive(m_inBuffer.data(), toRetrieve);
    this->samplesToComplex(m_inBuffer.data(), retrieved, in);

	return retrieved;
}

size_t AudioFrontend::receive(float* in, size_t in_size)
{
    size_t toRetrieve = min(in_size, m_inBuffer.size() / 2);

	size_t retrieved = this->receive(m_inBuffer.data(), toRetrieve);
    this->samplesToFloat(m_inBuffer.data(), retrieved, in);

	return retrieved;
}

size_t AudioFrontend::receive(int16_t* in, size_t in_size)
{
    ALCint avail = 0;
    alcGetIntegerv(m_inputDevice, ALC_CAPTURE_SAMPLES, 1, &avail);

    do {
        this_thread::sleep_for(chrono::milliseconds(10));
        alcGetIntegerv(m_inputDevice, ALC_CAPTURE_SAMPLES, 1, &avail);
    }
    while (avail < static_cast<int>(in_size));

    alcCaptureSamples(m_inputDevice, in, in_size);

    return in_size;
}

int main(int argc, char* argv[])
{
    AudioFrontend* audio = new AudioFrontend();
    array<complex<float>, 2048> buffer_in;
    array<complex<float>, 512> buffer_down;
    array<complex<float>, 2048> buffer_up;

    firdecim_cccf resamp_down = firdecim_cccf_create_kaiser(4, 2*8, 40.f);
    firinterp_cccf resamp_up = firinterp_cccf_create_kaiser(4, 2*8, 40.f);

    agc_crcf agc = agc_crcf_create();
    agc_crcf_set_scale(agc, 0.04f);
    agc_crcf_set_bandwidth(agc, 0.003f);

    agc_crcf signal_detect = agc_crcf_create();
    agc_crcf_squelch_enable(signal_detect);
    agc_crcf_squelch_set_threshold(signal_detect, -90.f);

    bool last_squelch = false;

    while (true) {
        audio->receive(buffer_in.data(), buffer_in.size());
        // 48 kHz
        firdecim_cccf_execute_block(resamp_down, buffer_in.data(), buffer_down.size(), buffer_down.data());
        // 12 kHz

        for (auto& n : buffer_down) {
            complex<float> dummy = 0;

            agc_crcf_execute(signal_detect, n, &dummy);
            agc_crcf_execute(agc, n, &n);

            if (agc_crcf_squelch_get_status(signal_detect) == LIQUID_AGC_SQUELCH_ENABLED) {
                agc_crcf_lock(agc);
                last_squelch = true;
                n *= 0;
            }
            else if (last_squelch) {
                agc_crcf_unlock(agc);
                last_squelch = false;
            }
        }

        // 12 kHz
        firinterp_cccf_execute_block(resamp_up, buffer_down.data(), buffer_down.size(), buffer_up.data());
        // 48 kHz
        audio->transmit(buffer_up.data(), buffer_up.size());
    }

    firdecim_cccf_destroy(resamp_down);
    firinterp_cccf_destroy(resamp_up);

    delete audio;

    return 0;
}
