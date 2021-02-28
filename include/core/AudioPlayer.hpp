#pragma once
#include "core/soloudincl.h"
#include "soloud_bassboostfilter.h"
#include "data/Assets.hpp"
#include "data/Sound.hpp"
#include "data/SoundAllocator.hpp"
#include <memory>

struct AudioPlayer
{
        static void Initialize()
        {
                soloud.init();
                static SoLoud::BassboostFilter bass_boost;
                bass_boost.setParams(11.0f);
                soloud.setGlobalFilter(0, &bass_boost);
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