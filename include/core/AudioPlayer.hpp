#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include "data/Sound.hpp"
#include "data/SoundAllocator.hpp"
#include <vector>

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
        }

        static void Update()
        {
                ALenum state;
                // for (SoundSource s : current_sounds)
                for (auto it = current_sounds.rbegin(); it != current_sounds.rend(); it++)
                {
                        alGetSourcei((*it).source_id, AL_SOURCE_STATE, &state);
                        if (state != AL_PLAYING)
                        {
                                current_sounds.erase(std::next(it).base());
                        }
                }
        }

        static void PlaySound(std::shared_ptr<Sound> sound)
        {
                unsigned int sound_source;
                alGenSources(1, &sound_source);
                // alSource3f(stereo_source, AL_POSITION, 1.f, 0.f, 0.f);
                // alSource3f(stereo_source, AL_VELOCITY, 0.f, 0.f, 0.f);
                alSourcef(sound_source, AL_PITCH, 1.f);
                alSourcef(sound_source, AL_GAIN, 1.f);
                alSourcei(sound_source, AL_LOOPING, AL_FALSE);
                alSourcei(sound_source, AL_BUFFER, sound->sound_id);
                alSourcePlay(sound_source);
        }
private:
        inline static ALCcontext* context;
        struct SoundSource
        {
                std::shared_ptr<Sound> sound;
                unsigned int source_id;
        };
        inline static std::vector<SoundSource> current_sounds;
};