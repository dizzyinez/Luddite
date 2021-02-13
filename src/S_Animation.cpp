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
        lua_State*& L = lua_anim_behavior.L;

        //lua end function
        char str[100];
        snprintf(str, sizeof(str), "%s_End", lua_anim_behavior_state.current_animation.data());
        lua_getglobal(L, str);
        if (lua_isfunction(L, -1))
        {
                if (CheckLua(L, lua_pcall(L, 0, 0, 0)))
                {
                }
        }

        // // auto [lua_animation, lua_anim_behavior] = lua_reg->get<C_Animation, C_AnimationBehavior>(lua_entity);

        lua_animation.animation_line = lua_anim_behavior.json->root(name.c_str())("line").toNumber();
        // lua_animation.frames = 50;//lua_anim_behavior.json->root(name.c_str())("framecount").toNumber();
        // std::cout << "animation: " << lua_anim_behavior.json->root("Idle")("framecount").toNumber() << std::endl;
        lua_animation.frames = lua_anim_behavior.json->root(name.c_str())("framecount").toNumber();
        lua_animation.current_frame = 0;
        lua_animation.seconds_per_frame = (1.0 / float(lua_anim_behavior.json->root(name.c_str())("fps").toNumber()));
        lua_animation.animating = true;
        lua_anim_behavior_state.current_animation = name;


        //lua start function
        snprintf(str, sizeof(str), "%s_Start", lua_anim_behavior_state.current_animation.data());
        lua_getglobal(L, str);
        if (lua_isfunction(L, -1))
        {
                if (CheckLua(L, lua_pcall(L, 0, 0, 0)))
                {
                }
        }
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

void LookAtMouse()
{
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);
        lua_anim_behavior_state.points_towards_mouse = true;
}
int lua_LookAtMouse(lua_State* L) {LookAtMouse(); return 0;}

void LookAtMotion()
{
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);
        lua_anim_behavior_state.points_towards_mouse = false;
}
int lua_LookAtMotion(lua_State* L) {LookAtMotion(); return 0;}

void LockRotation(uint16_t time)
{
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);
        lua_anim_behavior_state.rotation_lock_timer = time;
}

int lua_LockRotation(lua_State* L)
{
        if (lua_isnumber(L, 1))
        {
                LockRotation(lua_tonumber(L, 1) + 1);
        }
        else
        {
                LockRotation(1);
        }
        return 0;
}

void UnlockRotation()
{
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);
        lua_anim_behavior_state.rotation_lock_timer = 0;
        lua_anim_behavior_state.rotation_lock = false;
}

int lua_UnlockRotation(lua_State* L) {UnlockRotation(); return 0;}


void LockMotion(uint16_t time)
{
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);
        lua_anim_behavior_state.motion_lock_timer = time;
}

int lua_LockMotion(lua_State* L)
{
        if (lua_isnumber(L, 1))
        {
                LockMotion(lua_tonumber(L, 1) + 1);
        }
        else
        {
                LockMotion(1);
        }
        return 0;
}

void UnlockMotion()
{
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);
        lua_anim_behavior_state.motion_lock_timer = 0;
        lua_anim_behavior_state.motion_lock = false;
}

int lua_UnlockMotion(lua_State* L) {UnlockMotion(); return 0;}

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
                                C_PlayerDirection &pd = reg.get<C_PlayerDirection>(Entity);
                                put_input_on_lua_stack(L, pi);
                        }
                        if (CheckLua(L, lua_pcall(L, 0, 0, 0)))
                        {
                                char str[100];
                                snprintf(str, sizeof(str), "%s_%u", abs.current_animation.data(), anim.current_frame);
                                lua_getglobal(L, str);//ab.current_animation.c_str());
                                if (lua_isfunction(L, -1))
                                {
                                        if (CheckLua(L, lua_pcall(L, 0, 0, 0)))
                                        {
                                        }
                                }
                                if (anim.current_frame == anim.frames - 1)
                                {
                                        snprintf(str, sizeof(str), "%s_Last", abs.current_animation.data());
                                        lua_getglobal(L, str);//ab.current_animation.c_str());
                                        if (lua_isfunction(L, -1))
                                        {
                                                if (CheckLua(L, lua_pcall(L, 0, 0, 0)))
                                                {
                                                }
                                        }
                                }
                        }
                        if (reg.has<C_PlayerInput>(Entity))
                        {
                                C_PlayerInput &pi = reg.get<C_PlayerInput>(Entity);
                                C_PlayerDirection &pd = reg.get<C_PlayerDirection>(Entity);
                                put_input_on_lua_stack(L, pi);
                                if (!abs.rotation_lock)
                                        if (abs.points_towards_mouse)
                                                anim.direction = pi.mouse_direction;
                                        else
                                                anim.direction = pd.movement_direction;
                                if (abs.rotation_lock_timer > 0)
                                {
                                        abs.rotation_lock_timer--;
                                        if (abs.rotation_lock_timer == 0)
                                                abs.rotation_lock = true;
                                        else
                                                abs.rotation_lock = false;
                                }
                                if (abs.motion_lock_timer > 0)
                                {
                                        abs.motion_lock_timer--;
                                        if (abs.motion_lock_timer == 0)
                                                abs.motion_lock = true;
                                        else
                                                abs.motion_lock = false;
                                }
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
                        lua_register(L, "LookAtMouse", lua_LookAtMouse);
                        lua_register(L, "LookAtMotion", lua_LookAtMotion);
                        lua_register(L, "LockRotation", lua_LockRotation);
                        lua_register(L, "UnlockRotation", lua_UnlockRotation);
                        lua_register(L, "LockMotion", lua_LockMotion);
                        lua_register(L, "UnlockMotion", lua_UnlockMotion);

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
        reg.view<C_Animation>().each([&reg, dt](auto Entity, C_Animation &animation) {
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
