#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <data/Sound.hpp>

#include <data/SoundAllocator.hpp>

struct AudioPlayer
{
        static void Initialize()
        {
                const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
                ALCdevice* device = alcOpenDevice(defaultDeviceString);
                if (!device)
                {
                        std::cout << "failed to get the default audio device for OpenAL" << std::endl;
                }
                std::cout << "Current OpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;

                context = alcCreateContext(device, nullptr);

                if (!alcMakeContextCurrent(context))
                {
                        std::cout << "failed to set OpenAL context as current context" << std::endl;
                }

                alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
                alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
                ALfloat forward_and_up_vectors[] = {
                        1.f, 0.f, 0.f,
                        0.f, 1.f, 0.f
                };
                alListenerfv(AL_ORIENTATION, forward_and_up_vectors);

                alGenSources(1, &stereo_source);
                // alSource3f(stereo_source, AL_POSITION, 1.f, 0.f, 0.f);
                // alSource3f(stereo_source, AL_VELOCITY, 0.f, 0.f, 0.f);
                alSourcef(stereo_source, AL_PITCH, 1.f);
                alSourcef(stereo_source, AL_GAIN, 1.f);
                alSourcei(stereo_source, AL_LOOPING, AL_FALSE);
                alSourcei(stereo_source, AL_BUFFER, SoundAllocator::Get("../assets/sounds/Geothermal.wav")->sound_id);

                // alSourcePlay(stereo_source);
        }
private:
        inline static ALCcontext* context;
        inline static ALuint stereo_source;
};