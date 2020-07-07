#ifndef Input_Velocity_hpp
#define Input_Velocity_hpp

#include "entityx/entityx.h"
#include "vec2.hpp"
#include <SDL2/SDL.h>

struct Input_Velocity : public entityx::Component<Input_Velocity>
{
        float controlThreshold = -1;
        std::array<std::pair<SDL_Keycode, bool>, 4> keymap = { //up down left right
                std::make_pair(SDLK_w, false),
                std::make_pair(SDLK_s, false),
                std::make_pair(SDLK_a, false),
                std::make_pair(SDLK_d, false)
        };
        float speed;

        Input_Velocity(float s) : speed(s)
        {

        }

        Input_Velocity(float s, float ct) : speed(s), controlThreshold(ct)
        {

        }

        vec2f setVel()
        {
                vec2f v;
                v.x = (keymap[2].second == keymap[3].second) ? 0 : (keymap[2].second == true) ? -1 : 1;
                v.y = (keymap[0].second == keymap[1].second) ? 0 : (keymap[0].second == true) ? -1 : 1;
                if (v.length() != 0)
                {
                        v.truncate(speed);
                }
                return v;
        }
};

#endif
