#include "stdafx.h"


namespace LSB
{

#define bufname "sharedbuf"
    void* sharedbuf;

    static int bufgetindex(lua_State* L)
    {
        char** parray = (char**)luaL_checkudata(L, 1, bufname);
        int index = luaL_checkinteger(L, 2);
        lua_pushnumber(L, (*parray)[index - 1]);
        return 1;
    }

    static int bufsetindex(lua_State* L)
    {
        char** parray = (char**)luaL_checkudata(L, 1, bufname);
        int index = luaL_checkinteger(L, 2);
        int value = luaL_checkinteger(L, 3);
        (*parray)[index - 1] = value;
        return 0;
    }

    static int luasetbuf(lua_State* L)
    {
        *((void**)lua_newuserdata(L, sizeof(sharedbuf))) = sharedbuf;
        luaL_getmetatable(L, "sharedbuf");
        lua_setmetatable(L, -2);
        return 1;
    }

    void Init(lua_State* L, void* buf)
    {
        sharedbuf = buf;

        static const struct luaL_Reg sharedbuf[] =
        {
           { "__index",  bufgetindex  },
           { "__newindex",  bufsetindex  },
           NULL, NULL
        };
        luaL_newmetatable(L, bufname);
        luaL_setfuncs(L, sharedbuf, 0);

        lua_register(L, bufname, luasetbuf);
    }


}
