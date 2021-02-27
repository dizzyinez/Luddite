#pragma once
#include "systems/Animation.hpp"
#include "components/GameComponents.hpp"
#include "data/Assets.hpp"
//I don't think the create variable is necessary but idk
void put_assets_on_stack(lua_State* L, bool create = true)
{
        if (create)
                lua_newtable(L);
        else
                lua_getglobal(L, "Sounds");

        for (int i = 0; i < Sounds::Sound_Ids.size(); i++)//(std::string& sounds_name : Sounds::Sound_Ids)
        {
                lua_pushstring(L, Sounds::Sound_Ids[i].c_str());
                lua_pushnumber(L, i);
                lua_settable(L, -3);
        }

        lua_setglobal(L, "Sounds");
}