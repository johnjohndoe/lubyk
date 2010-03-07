#include "opencv/cv.h"

#include "lua_cpp_helper.h"


using namespace cv;


/* ============================ Constructors     ====================== */


/** cv::Size2i::Size2i()
 * include/opencv/cxcore.hpp:348
 */
static int Size_Size1(lua_State *L) {
  Size * retval__ = new Size();
  lua_pushclass<Size>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(_Tp _width, _Tp _height)
 * include/opencv/cxcore.hpp:349
 */
static int Size_Size2(lua_State *L) {
  int _width = luaL_checkint(L, 1);
  int _height = luaL_checkint(L, 2);
  Size * retval__ = new Size(_width, _height);
  lua_pushclass<Size>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(const Size2i &sz)
 * include/opencv/cxcore.hpp:350
 */
static int Size_Size3(lua_State *L) {
  const Size2i *sz = *((const Size2i **)luaL_checkudata(L, 1, "cv.Size2i"));
  Size * retval__ = new Size(*sz);
  lua_pushclass<Size>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(const CvSize &sz)
 * include/opencv/cxcore.hpp:351
 */
static int Size_Size4(lua_State *L) {
  const CvSize *sz = *((const CvSize **)luaL_checkudata(L, 1, "cv.CvSize"));
  Size * retval__ = new Size(*sz);
  lua_pushclass<Size>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(const CvSize2D32f &sz)
 * include/opencv/cxcore.hpp:352
 */
static int Size_Size5(lua_State *L) {
  const CvSize2D32f *sz = *((const CvSize2D32f **)luaL_checkudata(L, 1, "cv.CvSize2D32f"));
  Size * retval__ = new Size(*sz);
  lua_pushclass<Size>(L, retval__, "cv.Size");
  return 1;
}



/** Overloaded function chooser for Size(...) */
static int Size_Size(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TNUMBER) {
    return Size_Size2(L);
  } else if (top__ < 1) {
    return Size_Size1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvSize")) {
    return Size_Size4(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvSize2D32f")) {
    return Size_Size5(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size2i")) {
    return Size_Size3(L);
  } else {
    // use any to raise errors
    return Size_Size3(L);
  }
}

/* ============================ Destructor       ====================== */

static int Size_destructor(lua_State *L) {
  Size **userdata = (Size**)luaL_checkudata(L, 1, "cv.Size");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Size__tostring(lua_State *L) {
  Size **userdata = (Size**)luaL_checkudata(L, 1, "cv.Size");
  
  lua_pushfstring(L, "<cv.Size: %p %dx%d>", *userdata, (*userdata)->width, (*userdata)->height);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** _Tp cv::Size2i::area() const 
 * include/opencv/cxcore.hpp:355
 */
static int Size_area(lua_State *L) {
  Size *self__ = *((Size**)luaL_checkudata(L, 1, "cv.Size"));
  lua_remove(L, 1);
  int  retval__ = self__->area();
  lua_pushnumber(L, retval__);
  return 1;
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Size_member_methods[] = {
  {"area"              , Size_area},
  {"__tostring"        , Size__tostring},
  {"__gc"              , Size_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Size_namespace_methods[] = {
  {"Size"              , Size_Size},
  {"Size2i"            , Size_Size},
  {NULL, NULL},
};



void luaopen_cv_Size(lua_State *L) {
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "cv.Size");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Size_member_methods);

  // register class methods in a global table like "dub"
  luaL_register(L, "cv", Size_namespace_methods);


}
