#include "systems/Animation.hpp"
#include "components/Animation.hpp"
#include "components/Tileset.hpp"
#include "components/AnimationBehavior.hpp"
#include "components/Player.hpp"

#include <string>

bool CheckLua(lua_State* L, int r)
{
        if (r == LUA_OK)
        {
                return true;
        }
        else
        {
                std::cout << "LUA ERROR: " << lua_tostring(L, -1) << std::endl;
                return false;
        }
};



// C_Animation* lua_animation;
// C_AnimationBehavior* lua_anim_behavior;
entt::entity lua_entity;
entt::registry* lua_reg;

void PlayAnimation(const std::string& name)
{
        // std::cout << "animation to play: " << name << std::endl;
        auto &lua_animation = lua_reg->get<C_Animation>(lua_entity);
        auto &lua_anim_behavior = lua_reg->get<C_AnimationBehavior>(lua_entity);
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);

        // // auto [lua_animation, lua_anim_behavior] = lua_reg->get<C_Animation, C_AnimationBehavior>(lua_entity);

        lua_animation.animation_line = lua_anim_behavior.json->root(name.c_str())("line").toNumber();
        // lua_animation.frames = 50;//lua_anim_behavior.json->root(name.c_str())("framecount").toNumber();
        // std::cout << "animation: " << lua_anim_behavior.json->root("Idle")("framecount").toNumber() << std::endl;
        lua_animation.frames = lua_anim_behavior.json->root(name.c_str())("framecount").toNumber();
        lua_animation.current_frame = 0;
        lua_animation.seconds_per_frame = (1.0 / float(lua_anim_behavior.json->root(name.c_str())("fps").toNumber()));
        lua_animation.animating = true;
        lua_anim_behavior_state.current_animation = name;
}

int lua_PlayAnimation(lua_State* L)
{
        if (lua_isstring(L, 1))
        {
                std::string name = lua_tostring(L, 1);
                PlayAnimation(name);
        }
        else
        {
                std::cout << "[LUA] | PlayAnimation() requires a string!";
        }
        return 0;
}

void Print(const std::string& string)
{
        std::cout << string << std::endl;
}

int lua_Print(lua_State* L)
{
        if (lua_isstring(L, 1))
        {
                std::string text = lua_tostring(L, 1);
                Print(text);
        }
        else
        {
                std::cout << "[LUA] | PlayAnimation() requires a string!";
        }
        return 0;
}

void put_input_on_lua_stack(lua_State* L, C_PlayerInput& pi, bool create = false)
{
        if (create)
                lua_newtable(L);
        else
                lua_getglobal(L, "Input");

        lua_pushstring(L, "moveX");
        lua_pushnumber(L, pi.moveX());
        lua_settable(L, -3);

        lua_pushstring(L, "moveY");
        lua_pushnumber(L, pi.moveY());
        lua_settable(L, -3);

        lua_pushstring(L, "button1");
        lua_pushboolean(L, pi.button1());
        lua_settable(L, -3);

        lua_pushstring(L, "button2");
        lua_pushboolean(L, pi.button2());
        lua_settable(L, -3);

        lua_pushstring(L, "button3");
        lua_pushboolean(L, pi.button3());
        lua_settable(L, -3);

        lua_pushstring(L, "button4");
        lua_pushboolean(L, pi.button4());
        lua_settable(L, -3);

        lua_setglobal(L, "Input");
}

void run_animation_behavior_increment(entt::registry& reg, entt::entity Entity)
{
        if (reg.has<C_AnimationBehavior>(Entity))
        {
                auto& anim = reg.get<C_Animation>(Entity);
                auto& ab = reg.get<C_AnimationBehavior>(Entity);
                auto& abs = reg.get<C_AnimationBehaviorState>(Entity);
                auto& L = ab.L;
                lua_entity = Entity;
                // lua_animation = &animation;
                // lua_anim_behavior = &ab;
                lua_getglobal(L, abs.current_animation.c_str());//ab.current_animation.c_str());
                if (lua_isfunction(L, -1))
                {
                        if (reg.has<C_PlayerInput>(Entity))
                        {
                                C_PlayerInput &pi = reg.get<C_PlayerInput>(Entity);
                                put_input_on_lua_stack(L, pi);
                                if (abs.rotates)
                                        if (abs.points_towards_mouse)
                                                anim.direction = pi.mouse_direction;
                                        else
                                                anim.direction = pi.movement_direction;
                        }
                        if (CheckLua(L, lua_pcall(L, 0, 0, 0)))
                        {
                        }
                }
                else
                {
                        std::cout << "LUA ERROR: could not find function: " << abs.current_animation << std::endl;
                }
        }
}



void S_Animation::update(float dt, entt::registry &reg)
{
        lua_reg = &reg;
        reg.view<C_AnimationBehavior>().each([&reg](auto Entity, auto &ab) {
                lua_entity = Entity;
                // lua_animation = &reg.get<C_Animation>(Entity);
                // C_Animation& anim = reg.get<C_Animation>(Entity);
                C_AnimationBehaviorState& abs = reg.get<C_AnimationBehaviorState>(Entity);
                auto& L = ab.L;
                if (L == nullptr)
                {
                        L = luaL_newstate();
                        abs.current_animation = "Idle";
                        lua_register(L, "PlayAnimation", lua_PlayAnimation);
                        lua_register(L, "Print", lua_Print);

                        if (reg.has<C_PlayerInput>(Entity))
                        {
                                C_PlayerInput &pi = reg.get<C_PlayerInput>(Entity);
                                put_input_on_lua_stack(L, pi, true);
                        }

                        if (CheckLua(L, luaL_dofile(L, ab.lua_path.c_str())))
                        {
                                lua_getglobal(L, "OnCreate");
                                if (lua_isfunction(L, -1))
                                {
                                        if (CheckLua(L, lua_pcall(L, 0, 0, 0)))
                                        {
                                        }
                                }
                        }
                }
        });
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
                                        run_animation_behavior_increment(reg, Entity);
                                }
                                else if (animation.repeat)
                                {
                                        animation.current_frame = 0;
                                        run_animation_behavior_increment(reg, Entity);
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
