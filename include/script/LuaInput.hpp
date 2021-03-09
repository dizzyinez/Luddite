#pragma once
#include "systems/Animation.hpp"
#include "components/GameComponents.hpp"
void put_input_on_lua_stack(lua_State* L, C_PlayerInput& pi, C_PlayerDeterministicInput& pdi, bool create = false)
{
        if (create)
                lua_newtable(L);
        else
                lua_getglobal(L, "Input");

        #define PUSH_INPUT_VAR_TO_LUA(var) \
        lua_pushstring(L, #var); \
        lua_pushnumber(L, pi.var()); \
        lua_settable(L, -3);
        PUSH_INPUT_VAR_TO_LUA(moveX)
        PUSH_INPUT_VAR_TO_LUA(moveY)
        #undef PUSH_INPUT_VAR_TO_LUA


        #define PUSH_INPUT_VAR_TO_LUA(var) \
        lua_pushstring(L, #var); \
        lua_pushboolean(L, pi.var()); \
        lua_settable(L, -3);
        PUSH_INPUT_VAR_TO_LUA(button1)
        PUSH_INPUT_VAR_TO_LUA(button2)
        PUSH_INPUT_VAR_TO_LUA(button3)
        PUSH_INPUT_VAR_TO_LUA(button4)
        #undef PUSH_INPUT_VAR_TO_LUA


        #define PUSH_INPUT_VAR_TO_LUA(var) \
        lua_pushstring(L, #var); \
        lua_pushnumber(L, pdi.var); \
        lua_settable(L, -3);
        PUSH_INPUT_VAR_TO_LUA(button1_press_age)
        PUSH_INPUT_VAR_TO_LUA(button2_press_age)
        PUSH_INPUT_VAR_TO_LUA(button3_press_age)
        PUSH_INPUT_VAR_TO_LUA(button4_press_age)
        PUSH_INPUT_VAR_TO_LUA(button1_release_age)
        PUSH_INPUT_VAR_TO_LUA(button2_release_age)
        PUSH_INPUT_VAR_TO_LUA(button3_release_age)
        PUSH_INPUT_VAR_TO_LUA(button4_release_age)
        #undef PUSH_INPUT_VAR_TO_LUA


        lua_setglobal(L, "Input");
}