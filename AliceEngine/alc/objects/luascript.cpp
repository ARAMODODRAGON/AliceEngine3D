#include "luascript.hpp"
#include <lua.hpp>

static std::unique_ptr<lua_State, decltype(&lua_close)> g_state(nullptr, lua_close);
static lua_State* _create_state() {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);
	return state;
}
static lua_State* get_state() {
	if (g_state.get() == nullptr)
		g_state.reset(_create_state());
	return g_state.get();
}

namespace alc {

	luascript::luascript() { }

	luascript::~luascript() { }

	bool luascript::load(const std::string& filepath) {
		auto* state = get_state();

		// try load file
		if (luaL_dofile(state, filepath.c_str()) == LUA_OK) {
			// success
			lua_pop(state, lua_gettop(state));
			return true;
		}

		// failed
		ALC_DEBUG_ERROR(lua_tostring(state, lua_gettop(state)));
		lua_pop(state, lua_gettop(state));

		return false;
	}

	void luascript::message(object* self, const std::string& function) { }

}