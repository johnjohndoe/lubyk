#include "mimas/Path.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::Path::Path()
 * include/mimas/Path.h:46
 */
static int Path_Path1(lua_State *L) {
  try {
    Path * retval__ = new Path();
    lua_pushclass<Path>(L, retval__, "mimas.Path");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Path.Path: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Path.Path: Unknown exception");
  }
  return lua_error(L);
}



/** mimas::Path::Path(const QPainterPath &path)
 * include/mimas/Path.h:50
 */
static int Path_Path2(lua_State *L) {
  try {
    const QPainterPath *path = *((const QPainterPath **)dubL_checksdata(L, 1, "mimas.QPainterPath"));
    Path * retval__ = new Path(*path);
    lua_pushclass<Path>(L, retval__, "mimas.Path");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Path.Path: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Path.Path: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for Path(...) */
static int Path_Path(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (top__ < 1) {
    return Path_Path1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "mimas.QPainterPath")) {
    return Path_Path2(L);
  } else {
    // use any to raise errors
    return Path_Path2(L);
  }
}


/* ============================ Destructor       ====================== */

static int Path_destructor(lua_State *L) {
  Path **userdata = (Path**)dubL_checksdata_n(L, 1, "mimas.Path");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Path__tostring(lua_State *L) {
  Path **userdata = (Path**)dubL_checksdata_n(L, 1, "mimas.Path");
  
  
  lua_pushfstring(L, "<mimas.Path: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Path::addRect(float x, float y, float w, float h)
 * include/mimas/Path.h:69
 */
static int Path_addRect(lua_State *L) {
  try {
    Path *self__ = *((Path**)dubL_checksdata(L, 1, "mimas.Path"));
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    float w = dubL_checknumber(L, 4);
    float h = dubL_checknumber(L, 5);
    self__->addRect(x, y, w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Path.addRect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Path.addRect: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::Path::contains(float x, float y)
 * include/mimas/Path.h:93
 */
static int Path_contains(lua_State *L) {
  try {
    Path *self__ = *((Path**)dubL_checksdata(L, 1, "mimas.Path"));
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    bool  retval__ = self__->contains(x, y);
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Path.contains: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Path.contains: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Path::cubicTo(float c1X, float c1Y, float c2X, float c2Y, float endPointX, float endPointY)
 * include/mimas/Path.h:65
 */
static int Path_cubicTo(lua_State *L) {
  try {
    Path *self__ = *((Path**)dubL_checksdata(L, 1, "mimas.Path"));
    float c1X = dubL_checknumber(L, 2);
    float c1Y = dubL_checknumber(L, 3);
    float c2X = dubL_checknumber(L, 4);
    float c2Y = dubL_checknumber(L, 5);
    float endPointX = dubL_checknumber(L, 6);
    float endPointY = dubL_checknumber(L, 7);
    self__->cubicTo(c1X, c1Y, c2X, c2Y, endPointX, endPointY);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Path.cubicTo: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Path.cubicTo: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Path::lineTo(float x, float y)
 * include/mimas/Path.h:73
 */
static int Path_lineTo(lua_State *L) {
  try {
    Path *self__ = *((Path**)dubL_checksdata(L, 1, "mimas.Path"));
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    self__->lineTo(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Path.lineTo: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Path.lineTo: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Path::moveTo(float x, float y)
 * include/mimas/Path.h:59
 */
static int Path_moveTo(lua_State *L) {
  try {
    Path *self__ = *((Path**)dubL_checksdata(L, 1, "mimas.Path"));
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    self__->moveTo(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Path.moveTo: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Path.moveTo: Unknown exception");
  }
  return lua_error(L);
}



/** Path mimas::Path::outlineForWidth(float width)
 * include/mimas/Path.h:81
 */
static int Path_outlineForWidth(lua_State *L) {
  try {
    Path *self__ = *((Path**)dubL_checksdata(L, 1, "mimas.Path"));
    float width = dubL_checknumber(L, 2);
    Path  retval__ = self__->outlineForWidth(width);
    lua_pushclass<Path>(L, retval__, "mimas.Path");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Path.outlineForWidth: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Path.outlineForWidth: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Path_member_methods[] = {
  {"addRect"           , Path_addRect},
  {"contains"          , Path_contains},
  {"cubicTo"           , Path_cubicTo},
  {"lineTo"            , Path_lineTo},
  {"moveTo"            , Path_moveTo},
  {"outlineForWidth"   , Path_outlineForWidth},
  {"__tostring"        , Path__tostring},
  {"__gc"              , Path_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Path_namespace_methods[] = {
  {"Path"              , Path_Path},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Path(lua_State *L) {
#else
extern "C" int luaopen_mimas_Path(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Path");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Path_member_methods);
  // save meta-table in mimas.Path_
  register_mt(L, "mimas", "Path");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Path_namespace_methods);


	return 1;
}
