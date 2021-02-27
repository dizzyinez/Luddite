#pragma once

#include "data/JsonAllocator.hpp"
#include <string>
#include <functional>
#include <lua.hpp>
#include <array>

struct C_AnimationBehavior
{
        C_AnimationBehavior(std::string json_file_path, std::string lua_file_path)
        {
                lua_path = lua_file_path;
                json = JsonAllocator::Get(json_file_path);
        }
        ~C_AnimationBehavior()
        {
                // if (L != nullptr)
                // lua_close(L);
        }
        //TODO: create utility function folder

        // void PlayAnimation(std::string name, C_Animation anim)
        // {
        //         animation_line = json->root(name.c_str())("line").toNumber();
        //         frames = json->root(name.c_str())("frame count").toNumber();
        //         current_frame = 0;
        //         seconds_per_frame = (1.0 / float(json->root(name.c_str())("fps").toNumber()));
        //         animating = true;
        // }

        std::string lua_path;
        lua_State* L = nullptr;
        std::shared_ptr<Json> json;
        // int (*lua_test)(lua_State*);// = [](lua_State* L) {return 0;};
};

struct C_AnimationBehaviorState
{
        C_AnimationBehaviorState()
        {
                hitboxes.fill(entt::null);
        }
        std::string current_animation;
        bool points_towards_mouse = false;
        bool rotation_lock = false;
        uint16_t rotation_lock_timer = 0;
        bool motion_lock = false;
        uint16_t motion_lock_timer = 0;
        float movement_speed = 1.f;
        std::array<entt::entity, 5> hitboxes;
        // glm::vec2 anim_velocity = glm::vec2(0.0f, 0.0f);
        float anim_vel_x = 0.0f;
        float anim_vel_y = 0.0f;
        float anim_vel_carried_motion_x = 0.0f;
        float anim_vel_carried_motion_y = 0.0f;
};