#pragma once
#include "core/soloudincl.h"
// #include "soloud_bassboostfilter.h"
#include "soloud_freeverbfilter.h"
#include "data/Assets.hpp"
#include "data/Sound.hpp"
#include "data/SoundAllocator.hpp"
#include <memory>

struct AudioPlayer
{
        static void Initialize()
        {
                soloud.init();
                // static SoLoud::FreeverbFilter reverb;
                // reverb.setParams(0, 0.5f, 0.5f, 1);
                // soloud.setGlobalFilter(0, &reverb);
        }
        static void Clean()
        {
                soloud.deinit();
        }

        static void Update()
        {
        }

        static void PlaySound(std::shared_ptr<Sound> sound)
        {
                soloud.play(sound->wav);
        }
private:
        static inline SoLoud::Soloud soloud;
};