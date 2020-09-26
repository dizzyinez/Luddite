#pragma once
#include <string>


struct C_Animation
{
        C_Animation() = default;
        // void PlayAnimation(std::string name)
        // {
        //         animation_line = json->root(name.c_str())("line").toNumber();
        //         frames = json->root(name.c_str())("frame count").toNumber();
        //         current_frame = 0;
        //         seconds_per_frame = (1.0 / float(json->root(name.c_str())("fps").toNumber()));
        //         animating = true;
        // }
        uint16_t animation_line = 0;
        uint16_t frames = 0;
        uint16_t current_frame = 0;
        double seconds_per_frame;
        double timer = 0.0;
        uint8_t direction = 0;
        bool repeat = true;
        bool animating = false;
};
