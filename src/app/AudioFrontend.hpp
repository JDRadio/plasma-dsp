#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <cstddef>
#include <cstdint>
#include <list>
#include <array>
#include <complex>
#include <memory>

using namespace std;

class AudioFrontend
{
public:
    typedef shared_ptr<AudioFrontend> sptr;

    static sptr make(void) { return make_shared<AudioFrontend>(); }

    AudioFrontend(void);
    ~AudioFrontend(void);

    size_t receive(int16_t* in, size_t in_size);
    size_t receive(double* in, size_t in_size);
    size_t receive(complex<double>* in, size_t in_size);
    void transmit(const int16_t* out, size_t out_size);
    void transmit(const double* out, size_t out_size);
    void transmit(const complex<double>* out, size_t out_size);

private:
    void samplesToDouble(const int16_t* in, size_t in_size, double* out);
    void samplesToComplex(const int16_t* in, size_t in_size, complex<double>* out);
    void doubleToSamples(const double* in, size_t in_size, int16_t* out);
    void complexToSamples(const complex<double>* in, size_t out_size, int16_t* out);

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
