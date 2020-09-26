#include "systems/Animation.hpp"
#include "components/Animation.hpp"
#include "components/Tileset.hpp"
#include <math.h>

void S_Animation::update(float dt, entt::registry &reg)
{
        reg.view<C_Animation>().each([&reg, dt](auto Entity, auto &animation) {
                if (animation.animating)
                {
                        auto &tileset = reg.get<C_Tileset>(Entity);
                        animation.timer += dt;
                        if (animation.timer >= animation.seconds_per_frame)
                        {
                                animation.timer -= animation.seconds_per_frame;
                                if (animation.current_frame < animation.frames - 1)
                                {
                                        animation.current_frame++;
                                }
                                else if (animation.repeat)
                                {
                                        animation.current_frame = 0;
                                }
                                else
                                {
                                        animation.animating = false;
                                        animation.timer = 0.0;
                                }
                        }
                        tileset.index = tileset.tiles_width * (animation.animation_line + animation.direction) + animation.current_frame;
                }
        });
}
