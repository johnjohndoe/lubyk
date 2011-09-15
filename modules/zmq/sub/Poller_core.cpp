#include "zmq/Poller.h"

#include "lua_cpp_helper.h"





using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::Poller::Poller(int reserve=8)
 * include/zmq/Poller.h:97
 */
static int Poller_Poller(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Poller * retval__;
    if (top__ < 1) {
      retval__ = new Poller();
    } else {
      int reserve = dubL_checkint(L, 1);
      retval__ = new Poller(reserve);
    }
    lua_pushclass<Poller>(L, retval__, "zmq.Poller");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Poller: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Poller: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Poller_destructor(lua_State *L) {
  Poller **userdata = (Poller**)dubL_checksdata_n(L, 1, "zmq.Poller");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Poller__tostring(lua_State *L) {
  Poller **userdata = (Poller**)dubL_checksdata_n(L, 1, "zmq.Poller");
  
  
  lua_pushfstring(L, "<zmq.Poller: %p  (%i)>", *userdata, (*userdata)->count());
  
  return 1;
}

/* ============================ Member Methods   ====================== */



/** int zmq::Poller::add(int fd, int events)
 * include/zmq/Poller.h:170
 */
static int Poller_add1(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    int fd = dubL_checkint(L, 2);
    int events = dubL_checkint(L, 3);
    int  retval__ = self->add(fd, events);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "add: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "add: Unknown exception");
  }
  return lua_error(L);
}



/** int zmq::Poller::add(zmq::Socket *sock, int events)
 * include/zmq/Poller.h:174
 */
static int Poller_add2(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    zmq::Socket *sock = *((zmq::Socket **)dubL_checksdata(L, 2, "zmq.Socket"));
    int events = dubL_checkint(L, 3);
    int  retval__ = self->add(sock, events);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "add: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "add: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for add(...) */
static int Poller_add(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "zmq.Socket")) {
    return Poller_add2(L);
  } else if (type__ == LUA_TNUMBER) {
    return Poller_add1(L);
  } else {
    // use any to raise errors
    return Poller_add1(L);
  }
}


/** int zmq::Poller::count()
 * include/zmq/Poller.h:242
 */
static int Poller_count(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    int  retval__ = self->count();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "count: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "count: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize zmq::Poller::event(lua_State *L)
 * include/zmq/Poller.h:155
 */
static int Poller_event(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    
    LuaStackSize  retval__ = self->event(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "event: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "event: Unknown exception");
  }
  return lua_error(L);
}




/** void zmq::Poller::modify(int fd, int events)
 * include/zmq/Poller.h:178
 */
static int Poller_modify1(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    int fd = dubL_checkint(L, 2);
    int events = dubL_checkint(L, 3);
    self->modify(fd, events);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "modify: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "modify: Unknown exception");
  }
  return lua_error(L);
}



/** void zmq::Poller::modify(zmq::Socket *sock, int events)
 * include/zmq/Poller.h:187
 */
static int Poller_modify2(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    zmq::Socket *sock = *((zmq::Socket **)dubL_checksdata(L, 2, "zmq.Socket"));
    int events = dubL_checkint(L, 3);
    self->modify(sock, events);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "modify: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "modify: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for modify(...) */
static int Poller_modify(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "zmq.Socket")) {
    return Poller_modify2(L);
  } else if (type__ == LUA_TNUMBER) {
    return Poller_modify1(L);
  } else {
    // use any to raise errors
    return Poller_modify1(L);
  }
}


/** void zmq::Poller::modifyItem(size_t idx, int events)
 * include/zmq/Poller.h:199
 */
static int Poller_modifyItem(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    size_t idx = dubL_checkint(L, 2);
    int events = dubL_checkint(L, 3);
    self->modifyItem(idx, events);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "modifyItem: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "modifyItem: Unknown exception");
  }
  return lua_error(L);
}



/** int zmq::Poller::poll(float timeout)
 * include/zmq/Poller.h:129
 */
static int Poller_poll(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    float timeout = dubL_checknumber(L, 2);
    int  retval__ = self->poll(timeout);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "poll: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "poll: Unknown exception");
  }
  return lua_error(L);
}




/** void zmq::Poller::remove(int fd, int flags)
 * include/zmq/Poller.h:206
 */
static int Poller_remove1(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    int fd = dubL_checkint(L, 2);
    int flags = dubL_checkint(L, 3);
    self->remove(fd, flags);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "remove: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "remove: Unknown exception");
  }
  return lua_error(L);
}



/** void zmq::Poller::remove(zmq::Socket *sock, int flags)
 * include/zmq/Poller.h:215
 */
static int Poller_remove2(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    zmq::Socket *sock = *((zmq::Socket **)dubL_checksdata(L, 2, "zmq.Socket"));
    int flags = dubL_checkint(L, 3);
    self->remove(sock, flags);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "remove: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "remove: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for remove(...) */
static int Poller_remove(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "zmq.Socket")) {
    return Poller_remove2(L);
  } else if (type__ == LUA_TNUMBER) {
    return Poller_remove1(L);
  } else {
    // use any to raise errors
    return Poller_remove1(L);
  }
}


/** void zmq::Poller::removeItem(size_t idx)
 * include/zmq/Poller.h:226
 */
static int Poller_removeItem(lua_State *L) {
  try {
    Poller *self = *((Poller**)dubL_checksdata(L, 1, "zmq.Poller"));
    size_t idx = dubL_checkint(L, 2);
    self->removeItem(idx);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "removeItem: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "removeItem: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Poller_member_methods[] = {
  {"add"               , Poller_add},
  {"count"             , Poller_count},
  {"event"             , Poller_event},
  {"modify"            , Poller_modify},
  {"modifyItem"        , Poller_modifyItem},
  {"poll"              , Poller_poll},
  {"remove"            , Poller_remove},
  {"removeItem"        , Poller_removeItem},
  {"__tostring"        , Poller__tostring},
  {"__gc"              , Poller_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Poller_namespace_methods[] = {
  {"Poller"            , Poller_Poller},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_zmq_Poller_core(lua_State *L) {
#else
extern "C" int luaopen_zmq_Poller_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "zmq.Poller");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Poller_member_methods);
  // save meta-table in zmq.Poller_
  register_mt(L, "zmq", "Poller");

  // register class methods in a global namespace table
  luaL_register(L, "zmq", Poller_namespace_methods);


	return 1;
}
