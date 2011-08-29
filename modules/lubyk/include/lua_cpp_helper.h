/*                                     
Copyright (c) 2011 by Gaspard Bucher (http://teti.ch).

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DOXY_GENERATOR_LIB_DOXY_GENERATOR_INCLUDE_LUA_DOXY_HELPER_H_
#define DOXY_GENERATOR_LIB_DOXY_GENERATOR_INCLUDE_LUA_DOXY_HELPER_H_

#include <stdlib.h> // malloc
#include <string> // std::string for Exception
#include <exception> // std::exception

#ifdef __cplusplus
extern "C" {
#endif
// We need C linkage because lua lib is compiled as C code
#include "lua.h"
#include "lauxlib.h"

#ifdef __cplusplus
}
#endif
/** Try/catch safe versions of luaL_checknumber, luaL_checkudata, .. */
                       
namespace dub {
class Exception : public std::exception
{
  std::string message_;
public:
  explicit Exception(const char *format, ...);

  ~Exception() throw();

  const char* what() const throw();
};

class TypeException : public Exception
{
public:
  explicit TypeException(lua_State *L, int narg, const char *type, bool is_super = false);
};

} // dub


// ================================================== dubL_check... try/catch safe
// these provide the same funcionality of their equivalent luaL_check... but they
// throw std::exception which can be caught (eventually to call lua_error)

lua_Number dubL_checknumber(lua_State *L, int narg) throw(dub::TypeException);

lua_Number dubL_checkint(lua_State *L, int narg) throw(dub::TypeException);

const char *dubL_checklstring(lua_State *L, int narg, size_t *len) throw(dub::TypeException);

lua_Integer dubL_checkinteger(lua_State *L, int narg) throw(dub::TypeException);

void *dubL_checkudata(lua_State *L, int ud, const char *tname) throw(dub::TypeException);

// super aware userdata calls
void *dubL_checksdata(lua_State *L, int ud, const char *tname) throw(dub::TypeException);
// does not throw exceptions (can be used with a #define for luaL_checksdata)
void *dubL_checksdata_n(lua_State *L, int ud, const char *tname) throw();

#define dubL_checkstring(L,n) (dubL_checklstring(L, (n), NULL))
#define dubL_checkint(L,n) ((int)dubL_checkinteger(L, (n)))
// super aware userdata check (does not throw)
#define luaL_checksdata(L, u, t) (dubL_checksdata_n(L, u, t))

/** ======================================== lua_pushclass          */

/** Push a custom type on the stack.
 * Since the value is passed as a pointer, we assume it has been created
 * using 'new' and Lua can safely call delete when it needs to garbage-
 * -collect it.
 */
template<class T>
void lua_pushclass(lua_State *L, T *ptr, const char *type_name) {
  T **userdata = (T**)lua_newuserdata(L, sizeof(T*));
  *userdata = ptr;

  // the userdata is now on top of the stack

  // set metatable (contains methods)
  luaL_getmetatable(L, type_name);
  lua_setmetatable(L, -2);
}

/** Push a custom type on the stack and give it the pointer to the userdata.
 * Passing the userdata enables early deletion from some other thread (GUI)
 * that safely invalidates the userdatum.
 */
template<class T>
void lua_pushclass2(lua_State *L, T *ptr, const char *type_name) {
  T **userdata = (T**)lua_newuserdata(L, sizeof(T*));
  *userdata = ptr;

  // store pointer in class so that it can set it to NULL on destroy with
  // *userdata = NULL
  ptr->setupDeletable((void**)userdata);

  // the userdata is now on top of the stack

  // set metatable (contains methods)
  luaL_getmetatable(L, type_name);
  lua_setmetatable(L, -2);
}

/** Classes that can be deleted out of Lua should inherit from this class.
 */
class DeletableOutOfLua {
  void **userdata_;
public:
  DeletableOutOfLua();

  virtual ~DeletableOutOfLua();

  virtual void luaDestroy();

  void setupDeletable(void **ptr);

protected:
  /** MUST be called from the custom destructor.
   */
  void luaCleanup();
};

/** Push a custom type on the stack.
 * Since the value is passed by value, we have to allocate a copy
 * using 'new' so that Lua can keep it.
 */
template<class T>
void lua_pushclass(lua_State *L, T &val, const char *type_name) {
  T *val_ptr = new T(val);
  lua_pushclass<T>(L, val_ptr, type_name);
}

/** ======================================== DubArgPointer */

/** This class is a helper to provide pointer to data from
 * Lua to C (but maybe it's not a good idea).
 */
template<class T>
class DubArgPointer {
public:
  DubArgPointer() : data(NULL) {}

  ~DubArgPointer() {
    if (data) free(data);
  }

  // TODO: we should have a hint on required sizes !
  T *operator()(lua_State *L, int index) {
    if (!lua_istable(L, index)) throw 1;

    size_t size = lua_objlen(L, index);
    if (size == 0) return NULL;

    data = (T*)malloc(size * sizeof(T));
    if (!data) throw 1;

    for(size_t i=0; i < size; ++i) {
      data[i] = get_value_at(L, index, i+1);
    }
    return data;
  }

private:
  T get_value_at(lua_State *L, int table_index, int index) {
    lua_pushinteger(L, index + 1);
    lua_gettable(L, index);
    T value = luaL_checknumber(L, -1);
    lua_pop(L, 1);
    return value;
  }

  T *data;
};

/** ======================================== is_userdata */

bool is_userdata(lua_State *L, int index, const char *tname);

/** ======================================== register_constants */

typedef struct lua_constants_Reg {
  const char *name;
  double constant;
} lua_constants_Reg;

int libsize (const lua_constants_Reg *l);

void register_constants(lua_State *L, const char *name_space, const lua_constants_Reg *l);

// The metatable lives in libname.ClassName_
void register_mt(lua_State *L, const char *libname, const char *class_name);

#endif // DOXY_GENERATOR_LIB_DOXY_GENERATOR_INCLUDE_LUA_DOXY_HELPER_H_
