#pragma once
#include "systems/Animation.hpp"
#include "components/GameComponents.hpp"
void put_data_on_lua_stack(lua_State* L, C_Animation& anim, bool create = false)
{
        if (create)
                lua_newtable(L);
        else
                lua_getglobal(L, "Data");

        lua_pushstring(L, "frame");
        lua_pushnumber(L, anim.current_frame);
        lua_settable(L, -3);

        lua_setglobal(L, "Data");
}