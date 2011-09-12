#include "lk/Socket.h"

#include "lua_cpp_helper.h"





using namespace lk;


/* ============================ Constructors     ====================== */

/** lk::Socket::Socket(int socket_type)
 * include/lk/Socket.h:106
 */
static int Socket_Socket(lua_State *L) {
  try {
    int socket_type = dubL_checkint(L, 1);
    Socket * retval__ = new Socket(socket_type);
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "lk.Socket");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Socket: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Socket: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Socket_destructor(lua_State *L) {
  Socket **userdata = (Socket**)dubL_checksdata_n(L, 1, "lk.Socket");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Socket__tostring(lua_State *L) {
  Socket **userdata = (Socket**)dubL_checksdata_n(L, 1, "lk.Socket");
  
  
  lua_pushfstring(L, "<lk.Socket: %p %s:%d --> %s:%d>", *userdata, (*userdata)->localHost(), (*userdata)->localPort(), (*userdata)->remoteHost(), (*userdata)->remotePort());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** LuaStackSize lk::Socket::accept(lua_State *L)
 * include/lk/Socket.h:135
 */
static int Socket_accept(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    
    LuaStackSize  retval__ = self->accept(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "accept: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "accept: Unknown exception");
  }
  return lua_error(L);
}



/** int lk::Socket::bind(const char *localhost=NULL, int port=0)
 * include/lk/Socket.h:124
 */
static int Socket_bind(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    int top__ = lua_gettop(L);
    int  retval__;
    if (top__ < 2) {
      retval__ = self->bind();
    } else {
      const char *localhost = dubL_checkstring(L, 2);
      if (top__ < 3) {
        retval__ = self->bind(localhost);
      } else {
        int port = dubL_checkint(L, 3);
        retval__ = self->bind(localhost, port);
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "bind: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "bind: Unknown exception");
  }
  return lua_error(L);
}



/** void lk::Socket::close()
 * include/lk/Socket.h:113
 */
static int Socket_close(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    self->close();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "close: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "close: Unknown exception");
  }
  return lua_error(L);
}



/** void lk::Socket::connect(const char *host, int port)
 * include/lk/Socket.h:126
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    const char *host = dubL_checkstring(L, 2);
    int port = dubL_checkint(L, 3);
    self->connect(host, port);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "connect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "connect: Unknown exception");
  }
  return lua_error(L);
}



/** int lk::Socket::fd() const 
 * include/lk/Socket.h:207
 */
static int Socket_fd(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    int  retval__ = self->fd();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fd: Unknown exception");
  }
  return lua_error(L);
}



/** void lk::Socket::listen()
 * include/lk/Socket.h:130
 */
static int Socket_listen(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    self->listen();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "listen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "listen: Unknown exception");
  }
  return lua_error(L);
}



/** const char* lk::Socket::localHost() const 
 * include/lk/Socket.h:187
 */
static int Socket_localHost(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    const char * retval__ = self->localHost();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "localHost: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "localHost: Unknown exception");
  }
  return lua_error(L);
}



/** int lk::Socket::localPort() const 
 * include/lk/Socket.h:191
 */
static int Socket_localPort(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    int  retval__ = self->localPort();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "localPort: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "localPort: Unknown exception");
  }
  return lua_error(L);
}



/** int lk::Socket::port() const 
 * include/lk/Socket.h:203
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    int  retval__ = self->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "port: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize lk::Socket::recv(lua_State *L)
 * include/lk/Socket.h:166
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    
    LuaStackSize  retval__ = self->recv(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "recv: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "recv: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize lk::Socket::recvMsg(lua_State *L)
 * include/lk/Socket.h:160
 */
static int Socket_recvMsg(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    
    LuaStackSize  retval__ = self->recvMsg(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "recvMsg: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "recvMsg: Unknown exception");
  }
  return lua_error(L);
}



/** const char* lk::Socket::remoteHost() const 
 * include/lk/Socket.h:195
 */
static int Socket_remoteHost(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    const char * retval__ = self->remoteHost();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "remoteHost: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "remoteHost: Unknown exception");
  }
  return lua_error(L);
}



/** int lk::Socket::remotePort() const 
 * include/lk/Socket.h:199
 */
static int Socket_remotePort(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    int  retval__ = self->remotePort();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "remotePort: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "remotePort: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize lk::Socket::request(lua_State *L)
 * include/lk/Socket.h:182
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    
    LuaStackSize  retval__ = self->request(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "request: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "request: Unknown exception");
  }
  return lua_error(L);
}



/** int lk::Socket::send(lua_State *L)
 * include/lk/Socket.h:172
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    
    int  retval__ = self->send(L);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "send: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "send: Unknown exception");
  }
  return lua_error(L);
}



/** void lk::Socket::sendMsg(lua_State *L)
 * include/lk/Socket.h:177
 */
static int Socket_sendMsg(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    
    self->sendMsg(L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "sendMsg: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "sendMsg: Unknown exception");
  }
  return lua_error(L);
}



/** void lk::Socket::setNonBlocking()
 * include/lk/Socket.h:145
 */
static int Socket_setNonBlocking(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    self->setNonBlocking();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setNonBlocking: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setNonBlocking: Unknown exception");
  }
  return lua_error(L);
}



/** void lk::Socket::setRecvTimeout(int timeout)
 * include/lk/Socket.h:137
 */
static int Socket_setRecvTimeout(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    int timeout = dubL_checkint(L, 2);
    self->setRecvTimeout(timeout);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setRecvTimeout: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setRecvTimeout: Unknown exception");
  }
  return lua_error(L);
}



/** void lk::Socket::setSendTimeout(int timeout)
 * include/lk/Socket.h:141
 */
static int Socket_setSendTimeout(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "lk.Socket"));
    int timeout = dubL_checkint(L, 2);
    self->setSendTimeout(timeout);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSendTimeout: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSendTimeout: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Socket_member_methods[] = {
  {"accept"            , Socket_accept},
  {"bind"              , Socket_bind},
  {"close"             , Socket_close},
  {"connect"           , Socket_connect},
  {"fd"                , Socket_fd},
  {"listen"            , Socket_listen},
  {"localHost"         , Socket_localHost},
  {"localPort"         , Socket_localPort},
  {"port"              , Socket_port},
  {"recv"              , Socket_recv},
  {"recvMsg"           , Socket_recvMsg},
  {"remoteHost"        , Socket_remoteHost},
  {"remotePort"        , Socket_remotePort},
  {"request"           , Socket_request},
  {"send"              , Socket_send},
  {"sendMsg"           , Socket_sendMsg},
  {"setNonBlocking"    , Socket_setNonBlocking},
  {"setRecvTimeout"    , Socket_setRecvTimeout},
  {"setSendTimeout"    , Socket_setSendTimeout},
  {"__tostring"        , Socket__tostring},
  {"__gc"              , Socket_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Socket_namespace_methods[] = {
  {"Socket"            , Socket_Socket},
  {NULL, NULL},
};


static const struct lua_constants_Reg Socket_namespace_constants[] = {
  {"TCP"               , lk::Socket::TCP},
  {"UDP"               , lk::Socket::UDP},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_lk_Socket_core(lua_State *L) {
#else
extern "C" int luaopen_lk_Socket_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "lk.Socket");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Socket_member_methods);
  // save meta-table in lk.Socket_
  register_mt(L, "lk", "Socket");

  // register class methods in a global namespace table
  luaL_register(L, "lk", Socket_namespace_methods);


  // register class enums
  register_constants(L, "lk.Socket_const", Socket_namespace_constants);

	return 1;
}
