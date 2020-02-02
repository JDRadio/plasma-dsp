#include <AL/al.h>
#include <AL/alc.h>

#include <list>
#include <array>
#include <cstdint>
#include <cstddef>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    list<ALuint> bufferQueue;

    ALenum errorCode = 0;
    array<ALuint, 16> helloBuffer;
    ALuint helloSource;

    auto szDefaultCaptureDevice = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
    cout << "Default Capture Device: " << szDefaultCaptureDevice << endl;

    auto szDefaultOutputDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    cout << "Default Output Device: " << szDefaultOutputDevice << endl;

    ALCdevice* audioDevice = alcOpenDevice(nullptr);
    if (! audioDevice) {
        errorCode = alcGetError(audioDevice);
        cerr << "alcOpenDevice: " << errorCode << endl;
        return 1;
    }

    ALCcontext* audioContext = alcCreateContext(audioDevice, nullptr);
    if (! audioContext) {
        errorCode = alcGetError(audioDevice);
        cerr << "alcCreateContext: " << errorCode << endl;
        return 1;
    }

    alcMakeContextCurrent(audioContext);

    alGetError();
    alGenBuffers(helloBuffer.size(), helloBuffer.data());
    errorCode = alGetError();
    //cerr << "alGenBuffers: " << errorCode << endl;

    for (size_t i = 0; i < helloBuffer.size(); i++) {
        //cout << "Pushing buffer " << helloBuffer[i] << endl;
        bufferQueue.push_back(helloBuffer[i]);
    }

    alGetError();
    alGenSources(1, &helloSource);
    errorCode = alGetError();
    //cerr << "alGenSources: " << errorCode << endl;

    const ALCuint samplingRate = 22050;
    const ALCsizei bufferSize = samplingRate / 2;
    ALCdevice* inputDevice = alcCaptureOpenDevice(nullptr, samplingRate, AL_FORMAT_MONO16, bufferSize);

    if (! inputDevice) {
        errorCode = alcGetError(inputDevice);
        cerr << "alcCaptureOpenDevice: " << errorCode << endl;
        return 1;
    }

    alcCaptureStart(inputDevice);
    errorCode = alcGetError(inputDevice);
    //cerr << "alcCaptureStart: " << errorCode << endl;

    const ALCint captureSize = 2048;
    uint16_t buffer[captureSize];
    ALCint samplesIn = 0;
    ALint availBuffers = 0;
    ALuint myBuffer;
    ALuint bufferHolder[16];
    bool done = false;

    while (not done) {
        alGetSourcei(helloSource, AL_BUFFERS_PROCESSED, &availBuffers);

        if (availBuffers > 0) {
            //cout << "Available buffers: " << availBuffers << endl;
            alSourceUnqueueBuffers(helloSource, availBuffers, &bufferHolder[0]);
            for (int i = 0; i < availBuffers; i++) {
                //cout << "< Push buffer " << bufferHolder[i] << endl;
                bufferQueue.push_back(bufferHolder[i]);
            }
        }

        alcGetIntegerv(inputDevice, ALC_CAPTURE_SAMPLES, 1, &samplesIn);

        if (samplesIn < captureSize) {
            continue;
        }

        //cout << samplesIn << endl;
        alcCaptureSamples(inputDevice, buffer, captureSize);
        errorCode = alcGetError(inputDevice);

        if (not bufferQueue.empty()) {
            myBuffer = bufferQueue.front();
            bufferQueue.pop_front();
            //cout << "> Pop buffer " << myBuffer << endl;

            alBufferData(myBuffer, AL_FORMAT_MONO16, buffer, captureSize * sizeof(uint16_t), samplingRate);
            errorCode = alGetError();
            //cerr << "alBufferData: " << errorCode << endl;

            alSourceQueueBuffers(helloSource, 1, &myBuffer);
                ALint queuedBuffers;
                alGetSourcei(helloSource, AL_BUFFERS_QUEUED, &queuedBuffers);
                //cout << queuedBuffers << " buffers queued" << endl;

            ALint sourceState = 0;
            alGetSourcei(helloSource, AL_SOURCE_STATE, &sourceState);
            if (sourceState != AL_PLAYING) {
                alSourcePlay(helloSource);
            }
        }
    }

    alcCaptureStop(inputDevice);
    alcCaptureCloseDevice(inputDevice);

    alSourceStopv(1, &helloSource);
    alSourcei(helloSource, AL_BUFFER, 0);

    alDeleteSources(1, &helloSource);
    alDeleteBuffers(helloBuffer.size(), helloBuffer.data());
    errorCode = alGetError();

    alcMakeContextCurrent(nullptr);
    errorCode = alGetError();

    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);

    return 0;
}
