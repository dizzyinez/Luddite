#pragma once
#include "core/soloudincl.h"

struct AudioPlayer
{
        static void Initialize()
        {
                soloud.init();
        }
        static void Clean()
        {
        }

        static void Update()
        {
        }

        static void PlaySound()
        {
                soloud.deinit();
        }
private:
        static inline SoLoud::Soloud soloud;
};