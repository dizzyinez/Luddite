#include "systems/Animation.hpp"
#include "components/GameComponents.hpp"
#include "data/TextureAllocator.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "script/LuaAssets.hpp"
#include "script/LuaInput.hpp"
#include "script/LuaData.hpp"
#include "core/AudioPlayer.hpp"
// #include "components/Animation.hpp"
// #include "components/Tileset.hpp"
// #include "components/AnimationBehavior.hpp"
// #include "components/Player.hpp"

#include <string>
// constexpr int TOTAL_DIRECTIONS = 32;

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

void PlayAnimation(const std::string& name, bool continue_motion)
{
        // std::cout << "animation to play: " << name << std::endl;
        auto &lua_animation = lua_reg->get<C_Animation>(lua_entity);
        auto &lua_anim_behavior = lua_reg->get<C_AnimationBehavior>(lua_entity);
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);
        lua_State*& L = lua_anim_behavior.L;


        if (continue_motion)
        {
                lua_anim_behavior_state.anim_vel_carried_motion_x = lua_anim_behavior.json->root(lua_anim_behavior_state.current_animation.c_str())("Origin").at(lua_animation.current_frame)("x").toNumber();
                lua_anim_behavior_state.anim_vel_carried_motion_y = lua_anim_behavior.json->root(lua_anim_behavior_state.current_animation.c_str())("Origin").at(lua_animation.current_frame)("y").toNumber();
        }

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
                if (lua_isboolean(L, 2))
                {
                        PlayAnimation(name, lua_toboolean(L, 2));
                }
                else
                {
                        PlayAnimation(name, false);
                }
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

void SetMovementSpeed(float mul)
{
        auto &lua_anim_behavior_state = lua_reg->get<C_AnimationBehaviorState>(lua_entity);
        lua_anim_behavior_state.movement_speed = mul;
}

int lua_SetMovementSpeed(lua_State* L)
{
        if (lua_isnumber(L, 1))
        {
                SetMovementSpeed(lua_tonumber(L, 1));
        }
        else
        {
                SetMovementSpeed(1.0f);
        }
        return 0;
}

void AddTrauma(float t)
{
        lua_reg->get<C_Player>(lua_entity).trauma += t;
}

int lua_AddTrauma(lua_State* L)
{
        if (lua_isnumber(L, 1))
        {
                AddTrauma(lua_tonumber(L, 1));
        }
        else
        {
                AddTrauma(1.0f);
        }
        return 0;
}

void PlaySound(uint32_t sound_id)
{
        AudioPlayer::PlaySound(SoundAllocator::Get(Sounds::Sound_File_Locations.at(sound_id)));
        // AudioPlayer::PlaySound();
}

int lua_PlaySound(lua_State* L)
{
        if (lua_isnumber(L, 1))
        {
                PlaySound(lua_tonumber(L, 1));
        }
        return 0;
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
                        put_data_on_lua_stack(L, anim);
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
                        gason::JsonValue hitboxes_json = ab.json->root(abs.current_animation.c_str())("Hitboxes").at(0).at(anim.current_frame);
                        // ab.json->root.child
                        constexpr float SCALE_COEFFICIENT = 25.f;
                        constexpr float KNOCKBACK_COEFFICIENT = 10.f;
                        // glm::mat4 rotation = glm::rotate(glm::pi<float>() * (anim.direction * (2 / TOTAL_DIRECTIONS)), glm::vec3(0.f, 0.f, 1.f));
                        glm::mat4 rotation = glm::rotate(glm::pi<float>() * ((float)anim.direction * (2.f / (float)TOTAL_DIRECTIONS)), glm::vec3(0.f, 0.f, 1.f));
                        // std::cout << (int)anim.direction << " " << glm::to_string(rotation) << std::endl;
                        if (hitboxes_json("enabled").toBool())
                        {
                                // reg.get<C_Size>(abs.hitboxes.at(0)).size = glm::vec2(hitboxes_json("scale").toNumber() * SCALE_COEFFICIENT, hitboxes_json("scale").toNumber() * SCALE_COEFFICIENT);
                                // reg.get<C_Origin>(abs.hitboxes.at(0)).origin = glm::vec2(hitboxes_json("scale").toNumber() * SCALE_COEFFICIENT * 0.5f, hitboxes_json("scale").toNumber() * SCALE_COEFFICIENT * 0.5f);
                                reg.get<C_CircleCollider>(abs.hitboxes.at(0)).radius = hitboxes_json("scale").toNumber() * SCALE_COEFFICIENT;
                                C_Child& child = reg.get<C_Child>(abs.hitboxes.at(0));
                                child.offset = glm::vec3(rotation * glm::vec4(hitboxes_json("x").toNumber() * SCALE_COEFFICIENT, hitboxes_json("y").toNumber() * SCALE_COEFFICIENT, 0.f, 1.f));
                                //STORE THE ROTATE VECTOR AND USE IT TO CHANGE THE KB DIR
                                C_Hitbox& hitbox = reg.get<C_Hitbox>(abs.hitboxes.at(0));
                                hitbox.enabled = true;
                                hitbox.damage = hitboxes_json("damage").toNumber();
                                hitbox.kb_dir = -glm::vec2(rotation * glm::vec4(hitboxes_json("x_dir").toNumber() * SCALE_COEFFICIENT * KNOCKBACK_COEFFICIENT, hitboxes_json("y_dir").toNumber() * SCALE_COEFFICIENT * KNOCKBACK_COEFFICIENT, 0.f, 1.f));
                        }
                        else
                        {
                                C_Hitbox& hitbox = reg.get<C_Hitbox>(abs.hitboxes.at(0));
                                hitbox.enabled = false;
                                // reg.get<C_Size>(abs.hitboxes.at(0)).size = glm::vec2(0.f, 0.f);
                        }
                        // std::cout << (int)anim.direction << std::endl;
                        auto new_vel = rotation * glm::vec4(
                                (ab.json->root(abs.current_animation.c_str())("Origin").at(anim.current_frame)("dx").toNumber() - abs.anim_vel_carried_motion_x) * SCALE_COEFFICIENT,
                                (ab.json->root(abs.current_animation.c_str())("Origin").at(anim.current_frame)("dy").toNumber() - abs.anim_vel_carried_motion_y) * SCALE_COEFFICIENT,
                                0.f, 1.f);
                        abs.anim_vel_x = new_vel.x;
                        abs.anim_vel_y = new_vel.y;
                        abs.anim_vel_carried_motion_x = 0.0f;
                        abs.anim_vel_carried_motion_y = 0.0f;
                        // std::cout << abs.anim_vel_x << ", " << abs.anim_vel_y << std::endl;
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

                //create the hitboxes if they don't exist yet
                if (abs.hitboxes.at(0) == entt::null)
                {
                        for (int i = 0; i < abs.hitboxes.size(); i++)
                        {
                                entt::entity hitbox = reg.create();
                                reg.emplace<C_Position>(hitbox);
                                reg.emplace<C_Hitbox>(hitbox);
                                reg.emplace<C_CircleCollider>(hitbox, 0.f);
                                reg.emplace<C_Child>(hitbox, Entity);
                                // if (reg.had<C_Team>(Entity))
                                reg.emplace<C_Team>(hitbox, reg.get<C_Team>(Entity).team);

                                //for debug rendering
                                // reg.emplace<C_Size>(hitbox, 0.f, 0.f);
                                // reg.emplace<C_Texture>(hitbox, TextureAllocator::Get("../assets/textures/circle.png"));
                                // reg.emplace<C_Tint>(hitbox, glm::vec4(1.0f, 0.4f, 0.4f, 0.6f));
                                // reg.emplace<C_DrawLayer>(hitbox, DrawLayer::sprite);
                                abs.hitboxes.at(i) = hitbox;
                        }
                }

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
                        lua_register(L, "SetMovementSpeed", lua_SetMovementSpeed);
                        lua_register(L, "AddTrauma", lua_AddTrauma);
                        lua_register(L, "PlaySound", lua_PlaySound);

                        put_assets_on_stack(L);
                        if (reg.has<C_PlayerInput>(Entity))
                        {
                                C_PlayerInput &pi = reg.get<C_PlayerInput>(Entity);
                                put_input_on_lua_stack(L, pi, true);
                        }
                        put_data_on_lua_stack(L, reg.get<C_Animation>(Entity), true);

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
        }
                );
        reg.view<C_Animation>().each([&reg, dt](auto Entity, C_Animation &animation) {
                if (animation.animating)
                {
                        auto &tileset = reg.get<C_Tileset>(Entity);
                        // animation.timer += dt;
                        // if (animation.timer >= animation.seconds_per_frame)
                        // {
                        //         animation.timer -= animation.seconds_per_frame;
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
                        // }
                        tileset.index = tileset.tiles_width * (animation.animation_line + animation.direction) + animation.current_frame;
                }
        });
}
