/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/
#ifndef LUBYK_INCLUDE_LUBYK_LUA_CALLBACK_H_
#define LUBYK_INCLUDE_LUBYK_LUA_CALLBACK_H_

#include "lubyk.h"

namespace lubyk {
/** Calls a lua function back.
 */
class LuaCallback2
{
public:
  // ... <self>   (self is a table used for callbacks)
  LuaCallback2(lua_State *L) :
    worker_(NULL),
    lua_(NULL) {
    worker_ = getWorker(L);
    // ... <self>

  }

  virtual ~LuaCallback2() {
  }

  /** Set a callback. The top of the stack should be
   * -2. userdata from lk.Thread / lk.Socket / mimas.Callback / etc
   * -1. function()
   *
   * Thanks to Robert G. Jakabosky for the idea to use lua_xmove
   * instead of weak tables to store the function reference.
   */
  void set_lua_callback(lua_State *L) {
    // ... <self> <func>
    luaL_checktype(L, -1, LUA_TFUNCTION);

    // ... <self> <func>
    lua_getfenv(L, -2);
    // ... <self> <func> <env>
    lua_pushstring(L, ".");
    // ... <self> <func> <env> "."
    lua_rawget(L, -2);
    if (!lua_rawequal(L, -1, -4)) {
      // ... <self> <func> <env> <nil>
      // does not have it's own env table
      lua_pop(L, 2);
      // ... <self> <func>
      // Create env table
      lua_newtable(L);
      // ... <self> <func> <env>
      lua_pushstring(L, ".");
      // ... <self> <func> <env> "."
      lua_pushvalue(L, -4);
      // ... <self> <func> <env> "." <self>
      lua_rawset(L, -3); // env["."] = self
      // ... <self> <func> <env>
      lua_pushvalue(L, -1);
      // ... <self> <func> <env> <env>
      if (!lua_setfenv(L, -4)) {
        throw Exception("Could not set userdata env on '%s'.", lua_typename(L, lua_type(L, -4)));
      }
      // ... <self> <func> <env>
    } else {
      // ... <self> <func> <env> <self>
      // has its own env table
      lua_pop(L, 1);
      // ... <self> <func> <env>
    }

    // ... <self> <func> <env>
    if (lua_) {
      // remove from env
      luaL_unref(L, -1, thread_in_env_idx_);
    }

    lua_ = lua_newthread(L);
    // ... <self> <func> <env> <thread>

    // Store the thread in the Thread/Socket's environment table so it is not GC too soon
    thread_in_env_idx_ = luaL_ref(L, -2);
    // ... <self> <func> <env>

    lua_pop(L, 1);
    // ... <self> <func>

    // Transfer copies of <self> and <func> to thread stack
    lua_pushvalue(L, -1);
    // ... <self> <func> <func>
    lua_pushvalue(L, -3);
    // ... <self> <func> <func> <self>
    lua_xmove(L, lua_, 2);
    // ... <self> <func>

    // lua_ stack is now
    // <func> <self>
  }

  /** The caller should lock before calling this.
   */
  void push_lua_callback(bool push_self = true) const {
    if (!callback_set()) throw Exception("Callback function not set.");

    // <func> <self>
    lua_pushvalue(lua_, 1);
    if (push_self) lua_pushvalue(lua_, 2);
  }

  lubyk::Worker *worker_;
  lua_State *lua_;

  bool callback_set() const {
    return lua_ != NULL;
  }
private:
  int thread_in_env_idx_;
};

} // lubyk

#endif // LUBYK_INCLUDE_LUBYK_LUA_CALLBACK_H_
