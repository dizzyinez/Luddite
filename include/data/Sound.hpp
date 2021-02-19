#pragma once
#include <dr_mp3.h>
#include <dr_wav.h>
#include <stdint.h>

struct Sound
{
        unsigned int sound_id;
        // unsigned int source_id;
        Sound(unsigned int sound_id_) : sound_id(sound_id_) {}
        // ~Sound();
};