/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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
#ifndef RUBYK_INCLUDE_RK_TIMER_H_
#define RUBYK_INCLUDE_RK_TIMER_H_
#include "rubyk.h"
#include "rubyk/timer.h"
#include "rubyk/worker.h"

namespace rk {
/** Calls a lua function back at regular intervals. If the called function returns
 * a number, the number sets the new interval (0 = stop).
 * @dub string_format:'%%li'
 *      string_args:'(*userdata)->interval()'
 *      lib_name:'Timer_core'
 */
class Timer : public rubyk::LuaCallback
{
public:
  Timer(rubyk::Worker *worker, float interval, int lua_func_idx)
    : rubyk::LuaCallback(worker, lua_func_idx),
      timer_(this, interval) {}

  ~Timer() {}

  void stop() {
    timer_.stop();
  }

  void start() {
    timer_.start();
  }

  void join() {
    rubyk::ScopedUnlock unlock(worker_);
    timer_.join();
  }

  time_t interval() {
    return timer_.interval();
  }
private:
  void bang() {
    // L = LuaCallback's thread state

    // find function and call
    rubyk::ScopedLock lock(worker_);
    // FIXME: when all is clean, we should not need this
    lua_settop(L, 0);
    push_lua_callback();
    int status = lua_pcall(L, 0, 1, 0);
    if (status) {
      printf("Error triggering timer: %s\n", lua_tostring(L, -1));
    }

    if (lua_type(L, -1) == LUA_TNUMBER) {
      double interval = lua_tonumber(L, -1);
      if (interval == 0) {
        timer_.stop_from_loop();
      } else if (interval < 0) {
        luaL_error(L, "Timer interval must be a positive value (got %f)\n", interval);
      }
      timer_.set_interval_from_loop(interval);
    }

    // clear stack
    lua_settop(L, 0);
  }

  rubyk::Timer<rk::Timer, &rk::Timer::bang> timer_;
};

} // rk

#endif // RUBYK_INCLUDE_RK_TIMER_H_