#pragma once
#include "data/resources.hpp"
#include <string>
struct C_AnimationSimple
{
        C_AnimationSimple(int16_t line_, int16_t frames_, int16_t current_frame_, double seconds_per_frame_, double timer_, bool repeat_, bool animating_)
                : line(line_), frames(frames_), current_frame(current_frame_), seconds_per_frame(seconds_per_frame_), timer(timer_), repeat(repeat_), animating(animating_)
        {
        }
        uint16_t line;
        uint16_t frames;
        uint16_t current_frame;
        double seconds_per_frame;
        double timer;
        bool repeat = true;
        bool animating = false;
};

struct C_Animation
{
        C_Animation(std::string file_path)
        {
                json = json_manager.Get(file_path);
        }
        void PlayAnimation(std::string name)
        {
                animation_line = json->root(name.c_str())("line").toNumber();
                frames = json->root(name.c_str())("frame count").toNumber();
                current_frame = 0;
                seconds_per_frame = (1.0 / float(json->root(name.c_str())("fps").toNumber()));
                animating = true;
        }
        uint16_t animation_line = 0;
        uint16_t frames = 0;
        uint16_t current_frame = 0;
        double seconds_per_frame;
        double timer = 0.0;
        uint8_t direction = 0;
        bool repeat = true;
        bool animating = false;
        std::shared_ptr<Json> json;
};
