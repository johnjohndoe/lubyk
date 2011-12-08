/*
   QVariant variantFromLua(lua_State *L, int index);
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
#include "mimas/mimas.h"
#include "mimas/Callback.h"

#include <QtCore/QVariant>

// create value for Callback::Event type here
const QEvent::Type mimas::Callback::EventType = static_cast<QEvent::Type>(QEvent::registerEventType());


void mimas::setHue(QPalette &palette, float hue) {
  hue = (hue < 0 || hue >= 1.0) ? 0 : hue;

  //                              hue   sat   bri   alpha // FIXME: true == enabled?
  QColor border_color;
  border_color.setHsvF( hue, 1.0f, 1.0f, true ? 1.0f : 0.3f);
  QColor fill_color;
  fill_color.setHsvF(   hue, 0.5f, 0.5f, true ? 1.0f : 0.3f);

  //                   hue            sat   bri   alpha // FIXME: true == enabled?
  QColor text_color;
  text_color.setHsvF( hue, 1.0f, 1.0f, true ? 1.0f : 0.3f);

  // color = slider value (WindowText)
  // background = background (Window)
  // alternate-background-color = border color (AlternateBase)
  // selection-background-color: not used (Highlighted)
  // selection-color: slider button (HighlightedText)
  palette.setColor(QPalette::AlternateBase, border_color);
  palette.setColor(QPalette::Text, border_color);
  palette.setColor(QPalette::WindowText, fill_color);
}

QVariant mimas::variantFromLua(lua_State *L, int index) {
  int type = lua_type(L, index);
  switch (type) {
  case LUA_TNUMBER:
    return QVariant(lua_tonumber(L, index));
  case LUA_TBOOLEAN:
    return QVariant((bool)lua_toboolean(L, index));
  case LUA_TSTRING:
    return QVariant(QString::fromUtf8(lua_tostring(L, index)));
  // case LUA_TNIL:           // continue
  // case LUA_TTABLE:         // continue
  // case LUA_TUSERDATA:      // continue
  // case LUA_TUSERDATA:      // continue
  // case LUA_TTHREAD:        // continue
  // case LUA_TLIGHTUSERDATA: // continue
  default:
    return QVariant();
  }
}

// Register namespace
static const struct luaL_Reg lib_functions[] = {
  {NULL, NULL},
};

// Content generated by the bind.rb script
// [[DECLARE
  int luaload_mimas_Action(lua_State *L);
int luaload_mimas_Application_core(lua_State *L);
int luaload_mimas_Brush(lua_State *L);
int luaload_mimas_Button_core(lua_State *L);
int luaload_mimas_Callback_core(lua_State *L);
int luaload_mimas_Color(lua_State *L);
int luaload_mimas_DataSource(lua_State *L);
int luaload_mimas_FileObserver(lua_State *L);
int luaload_mimas_GLWidget(lua_State *L);
int luaload_mimas_GroupBox_core(lua_State *L);
int luaload_mimas_HBoxLayout_core(lua_State *L);
int luaload_mimas_Label_core(lua_State *L);
int luaload_mimas_LineEdit_core(lua_State *L);
int luaload_mimas_ListView(lua_State *L);
int luaload_mimas_MainWindow(lua_State *L);
int luaload_mimas_Menu(lua_State *L);
int luaload_mimas_MenuBar(lua_State *L);
int luaload_mimas_Painter(lua_State *L);
int luaload_mimas_Path(lua_State *L);
int luaload_mimas_Pen(lua_State *L);
int luaload_mimas_Slider_core(lua_State *L);
int luaload_mimas_SocketNotifier_core(lua_State *L);
int luaload_mimas_TableView(lua_State *L);
int luaload_mimas_Timer_core(lua_State *L);
int luaload_mimas_VBoxLayout_core(lua_State *L);
int luaload_mimas_Widget(lua_State *L);
int luaload_mimas_constants(lua_State *L);
  // ]]

extern "C" int luaopen_mimas_core(lua_State *L) {
  // declare metatables for cast signature mimas.QObject and mimas.QWidget
  luaL_newmetatable(L, "mimas.QObject");
  luaL_newmetatable(L, "mimas.QWidget");
  luaL_newmetatable(L, "mimas.QLayout");

  // register functions
  luaL_register(L, "mimas", lib_functions);

  // Content generated by the bind.rb script
  // [[LOAD
    luaload_mimas_Action(L);
  luaload_mimas_Application_core(L);
  luaload_mimas_Brush(L);
  luaload_mimas_Button_core(L);
  luaload_mimas_Callback_core(L);
  luaload_mimas_Color(L);
  luaload_mimas_DataSource(L);
  luaload_mimas_FileObserver(L);
  luaload_mimas_GLWidget(L);
  luaload_mimas_GroupBox_core(L);
  luaload_mimas_HBoxLayout_core(L);
  luaload_mimas_Label_core(L);
  luaload_mimas_LineEdit_core(L);
  luaload_mimas_ListView(L);
  luaload_mimas_MainWindow(L);
  luaload_mimas_Menu(L);
  luaload_mimas_MenuBar(L);
  luaload_mimas_Painter(L);
  luaload_mimas_Path(L);
  luaload_mimas_Pen(L);
  luaload_mimas_Slider_core(L);
  luaload_mimas_SocketNotifier_core(L);
  luaload_mimas_TableView(L);
  luaload_mimas_Timer_core(L);
  luaload_mimas_VBoxLayout_core(L);
  luaload_mimas_Widget(L);
  luaload_mimas_constants(L);
    // ]]

  return 0;
}
