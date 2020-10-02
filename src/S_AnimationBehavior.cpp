#include "systems/AnimationBehavior.hpp"
#include "components/AnimationBehavior.hpp"
#include "components/Animation.hpp"

#include <string>




void S_AnimationBehavior::update(float deltaTime, entt::registry &reg)
{
        // reg.view<C_AnimationBehavior>().each([&reg](auto Entity, auto &ab) {
        //         animation = &reg.get<C_Animation>(Entity);
        //         anim_behavior = &ab;
        //         if (ab.L == nullptr)
        //         {
        //                 ab.L = luaL_newstate();
        //                 luaaa::LuaModule Animation(ab.L, "Animation");
        //                 Animation.fun("PlayAnimation", lua_PlayAnimation);
        //                 if (CheckLua(ab.L, luaL_dofile(ab.L, ab.lua_path.c_str())))
        //                 {
        //                         lua_getglobal(ab.L, "OnCreate");
        //                         if (lua_isfunction(ab.L, -1))
        //                         {
        //                                 if (CheckLua(ab.L, lua_pcall(ab.L, 0, 0, 0)))
        //                                 {
        //                                 }
        //                         }
        //                 }
        //         }
        // });
}
