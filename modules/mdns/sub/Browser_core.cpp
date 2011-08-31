#include "mdns/Browser.h"

#include "lua_cpp_helper.h"


using namespace mdns;


/* ============================ Constructors     ====================== */

/** mdns::Browser::Browser(const char *service_type)
 * include/mdns/browser.h:50
 */
static int Browser_Browser(lua_State *L) {
  try {
    const char *service_type = dubL_checkstring(L, 1);
    Browser * retval__ = new Browser(service_type);
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mdns.Browser");
  } catch (std::exception &e) {
    lua_pushfstring(L, "mdns.Browser.Browser: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mdns.Browser.Browser: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Browser_destructor(lua_State *L) {
  Browser **userdata = (Browser**)dubL_checksdata_n(L, 1, "mdns.Browser");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Browser__tostring(lua_State *L) {
  Browser **userdata = (Browser**)dubL_checksdata_n(L, 1, "mdns.Browser");
  
  
  lua_pushfstring(L, "<mdns.Browser: %p %s>", *userdata, (*userdata)->serviceType());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** const char* mdns::Browser::serviceType()
 * include/mdns/browser.h:62
 */
static int Browser_serviceType(lua_State *L) {
  try {
    Browser *self__ = *((Browser**)dubL_checksdata(L, 1, "mdns.Browser"));
    const char * retval__ = self__->serviceType();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mdns.Browser.serviceType: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mdns.Browser.serviceType: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Browser_member_methods[] = {
  {"serviceType"       , Browser_serviceType},
  {"__tostring"        , Browser__tostring},
  {"__gc"              , Browser_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Browser_namespace_methods[] = {
  {"Browser"           , Browser_Browser},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mdns_Browser_core(lua_State *L) {
#else
extern "C" int luaopen_mdns_Browser_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mdns.Browser");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Browser_member_methods);
  // save meta-table in mdns.Browser_
  register_mt(L, "mdns", "Browser");

  // register class methods in a global namespace table
  luaL_register(L, "mdns", Browser_namespace_methods);


	return 1;
}
