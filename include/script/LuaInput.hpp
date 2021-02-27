#pragma once
#include "systems/Animation.hpp"
#include "components/GameComponents.hpp"
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