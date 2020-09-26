#pragma once
#include "data/resources.hpp"
#include <string>
extern "C"
{
  #include "lua/lua.h"
  #include "lua/lauxlib.h"
  #include "lua/lualib.h"
}

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

int lua_PlayAnimation(lua_State* L)
{
        if (lua_gettop(L) != 1)
                return -1;
        if (lua_isstring(L, 1))
        {
                std::string anim = lua_tostring(L, 1);
                std::cout << "animation to play: " << anim << std::endl;
        }
        return 0;
}

struct C_AnimationBehavior
{
        C_AnimationBehavior(std::string json_file_path, std::string lua_file_path)
        {
                json = json_manager.Get(json_file_path);

                lua_State* L = luaL_newstate();
                lua_register(L, "PlayAnimation", lua_PlayAnimation);


                if (CheckLua(L, luaL_dofile(L, lua_file_path.c_str())))
                {
                        lua_getglobal(L, "OnCreate");
                        if (lua_isfunction(L, -1))
                        {
                                if (CheckLua(L, lua_pcall(L, 0, 0, 0)))
                                {
                                }
                        }
                }

                lua_close(L);
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
        lua_State* L = nullptr;
        std::string current_animation;
        std::shared_ptr<Json> json;
};
