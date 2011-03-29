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
#ifndef LUBYK_INCLUDE_RK_THREAD_H_
#define LUBYK_INCLUDE_RK_THREAD_H_

#include "lubyk.h"
#include "lubyk/thread.h"
#include "lubyk/worker.h"

namespace lk {
/** Starts a new OS Thread with a given function.
 * @dub lib_name:'Thread_core'
 */
class Thread : public lubyk::LuaCallback, public lubyk::Thread
{
public:
  Thread(lubyk::Worker *worker)
    : lubyk::LuaCallback(worker) {}

  ~Thread() {
    kill();
  }

  void quit() {
    lubyk::Thread::quit();
  }

  void kill() {
    lubyk::ScopedUnlock unlock(worker_);
    lubyk::Thread::kill();
  }

  void join() {
    lubyk::ScopedUnlock unlock(worker_);
    lubyk::Thread::join();
  }

  bool shouldRun() {
    return lubyk::Thread::should_run();
  }

  void start(lua_State *L) {
    set_lua_callback(L);
    startThread<Thread, &Thread::run>(this, NULL);
  }

private:
  void run(lubyk::Thread *runner) {

    runner->thread_ready();

    lubyk::ScopedLock lock(worker_);

    // we may get a kill before even starting
    if (!should_run()) return;

    push_lua_callback();

    // lua_ = LuaCallback's thread state
    // first argument is self
    int status = lua_pcall(lua_, 1, 0, 0);

    if (status) {
      printf("Error in thread function: %s\n", lua_tostring(lua_, -1));
    }
  }
};

} // lk
#endif // LUBYK_INCLUDE_RK_THREAD_H_