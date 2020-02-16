#include "AudioFrontend.hpp"
#include <thread>
#include <algorithm>
#include <cmath>

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
		this_thread::yield();
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
        this_thread::yield();
        alcGetIntegerv(m_inputDevice, ALC_CAPTURE_SAMPLES, 1, &avail);
    }
    while (avail < static_cast<int>(in_size));

    alcCaptureSamples(m_inputDevice, in, in_size);

    return in_size;
}
