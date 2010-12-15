#include "mdns/Browser.h"

#include "lua_cpp_helper.h"


using namespace mdns;


/* ============================ Constructors     ====================== */

/** mdns::Browser::Browser(rubyk::Worker *worker, const char *service_type, int lua_func_idx)
 * include/mdns/Browser.h:47
 */
static int Browser_Browser(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    const char *service_type = luaL_checkstring(L, 2);
    
    luaL_checktype(L, 3, LUA_TFUNCTION);
    // push on top
    lua_pushvalue(L, 3);
    int lua_func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
    
    Browser * retval__ = new Browser(worker, service_type, lua_func_idx);
    lua_pushclass<Browser>(L, retval__, "mdns.Browser");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mdns.Browser.Browser: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mdns.Browser.Browser: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Browser_destructor(lua_State *L) {
  Browser **userdata = (Browser**)luaL_checkudata(L, 1, "mdns.Browser");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Browser__tostring(lua_State *L) {
  Browser **userdata = (Browser**)luaL_checkudata(L, 1, "mdns.Browser");
  
  lua_pushfstring(L, "<mdns.Browser: %p %s>", *userdata, (*userdata)->service_type());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** virtual void mdns::Browser::add_device(const Location &location)
 * include/mdns/Browser.h:57
 */
static int Browser_add_device(lua_State *L) {
  try {
    Browser *self__ = *((Browser**)luaL_checkudata(L, 1, "mdns.Browser"));
    lua_remove(L, 1);
    const Location *location = *((const Location **)luaL_checkudata(L, 1, "mdns.Location"));
    self__->add_device(*location);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mdns.Browser.add_device: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mdns.Browser.add_device: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** virtual void mdns::Browser::remove_device(const char *name)
 * include/mdns/Browser.h:72
 */
static int Browser_remove_device(lua_State *L) {
  try {
    Browser *self__ = *((Browser**)luaL_checkudata(L, 1, "mdns.Browser"));
    lua_remove(L, 1);
    const char *name = luaL_checkstring(L, 1);
    self__->remove_device(name);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mdns.Browser.remove_device: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mdns.Browser.remove_device: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Browser_member_methods[] = {
  {"add_device"        , Browser_add_device},
  {"remove_device"     , Browser_remove_device},
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

  // register class methods in a global namespace table
  luaL_register(L, "mdns", Browser_namespace_methods);


	return 1;
}
