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
#ifndef LUBYK_INCLUDE_MIMAS_APPLICATION_H_
#define LUBYK_INCLUDE_MIMAS_APPLICATION_H_

#include "mimas/mimas.h"
#include "mimas/Callback.h"

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtCore/QTimer>

#include <iostream>

using namespace lubyk;

namespace mimas {

/** Application (starts the GUI and manages the event loop). We do not
 * really need to use DeletableOutOfLua here but we never know.
 *
 * @dub lib_name:'Application_core'
 *      constructor: 'MakeApplication'
 *      destructor: 'luaDestroy'
 *      ignore: 'event'
 *      super: 'QObject'
 */
class Application : public QApplication, public ThreadedLuaObject {
  Q_OBJECT
public:
  /** Private constructor. Use MakeApplication instead.
   */
  Application();

  enum ApplicationType {
    Desktop,
    Plugin,
  };

  /** Custom constructor so that we can define some constants needed before
   * the application object is built (type of application).
   */
  static LuaStackSize MakeApplication(lua_State *L) {
    // <'type'>
    ApplicationType type = Desktop;
    if (lua_isstring(L, 1)) {
      std::string str(lua_tostring(L, 1));
      if (str == "plugin") {
        type = Plugin;
      }
    }

    if (type == Plugin) {
      // avoid qt_menu.nib loading
      QApplication::setAttribute(Qt::AA_MacPluginApplication, true);
    }

    Application *app = new Application();
    app->luaInit(L, app, "mimas.Application");
    // <app>
    return 1;
  }

  ~Application() {
    MIMAS_DEBUG_GC
  }

  // Destructor method called when the object goes out of Lua scope
  virtual void dub_destroy() {
    luaCleanup();
    deleteLater();
  }

  /** Start event loop.
   */
  int exec();

  /** Process events from an external event loop
   * FIXME: check deffered delete....
   */
  void processEvents(int maxtime);

  // Maybe this could be used for automated testing.
  // void postEvent(QObject *receiver, QEvent *event) {
  //   QApplication::postEvent(receiver, event);
  // }
  

  /** Key to retrieve 'this' value from a running thread.
   */
  static pthread_key_t sAppKey;

  /** Thread should stop. */
  static void terminate(int sig);

  void quit() {
    QApplication::quit();
  }

  void setStyleSheet(const char *text) {
    QApplication::setStyleSheet(QString(text));
  }

  /** Return the size of the desktop as a pair (width, height).
   */
  LuaStackSize screenSize(lua_State *L) {
    QRect rect = desktop()->geometry();
    lua_pushnumber(L, rect.width());
    lua_pushnumber(L, rect.height());
    return 2;
  }

  virtual bool event(QEvent *event);
  
  /** Calls a slot in msec milliseconds. Used by worker to handle scheduling.
   */
  void singleShot(int msec, QObject *receiver, const char *member) {
    QTimer::singleShot(msec, receiver, member);
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_APPLICATION_H_
