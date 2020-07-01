#include "ResourceLoader.h"

static lua_State *LuaState;

static uint8_t CheckLua(lua_State *state, int32_t code)
{
    if (code != LUA_OK)
	{
		const char *errorMessage = lua_tostring(state, -1);
		LOG_CLIENT_ERROR("Lua error %s\n", errorMessage);
		return 0;
	}
	return 1;
}

void SetupLoader(const char *file)
{
	LuaState = luaL_newstate();
	luaL_openlibs(LuaState);
	CheckLua(LuaState, luaL_loadfile(LuaState, file));
	
}
void LoadEngineConfig(uint32_t *width, uint32_t *height, double *version, char *title)
{
	lua_getglobal(LuaState, "Width");
	if (lua_isinteger(LuaState, -1)) *width = (int)lua_tointeger(LuaState, -1);

	lua_getglobal(LuaState, "Height");
	if (lua_isinteger(LuaState, -1)) *height = (int)lua_tointeger(LuaState, -1);

	lua_getglobal(LuaState, "Version");
	if (lua_isnumber(LuaState, -1)) *version = (double)lua_tonumber(LuaState, -1);

	lua_getglobal(LuaState, "Title");
	if (lua_isstring(LuaState, -1)) title = (char *)lua_tostring(LuaState, -1);

}
void LoadShaders(struct mageShader *shader, const uint32_t count)
{

}
