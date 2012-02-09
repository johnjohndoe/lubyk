#include "mimas/GroupBox.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::GroupBox::GroupBox(const char *title=NULL, QWidget *parent=NULL)
 * include/mimas/GroupBox.h:55
 */
static int GroupBox_GroupBox(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    GroupBox * retval__;
    if (top__ < 1) {
      retval__ = new GroupBox();
    } else {
      const char *title = dubL_checkstring(L, 1);
      if (top__ < 2) {
        retval__ = new GroupBox(title);
      } else {
        QWidget *parent = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
        retval__ = new GroupBox(title, parent);
      }
    }
    return retval__->luaInit(L, retval__, "mimas.GroupBox");
  } catch (std::exception &e) {
    lua_pushfstring(L, "GroupBox: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "GroupBox: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int GroupBox_destructor(lua_State *L) {
  GroupBox **userdata = (GroupBox**)dubL_checksdata_n(L, 1, "mimas.GroupBox");

  
  // custom destructor
  GroupBox *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int GroupBox_deleted(lua_State *L) {
  GroupBox **userdata = (GroupBox**)dubL_checksdata_n(L, 1, "mimas.GroupBox");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int GroupBox__tostring(lua_State *L) {
  GroupBox **userdata = (GroupBox**)dubL_checksdata_n(L, 1, "mimas.GroupBox");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.GroupBox: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.GroupBox: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::QWidget::activateWindow()
 * mimas/bind/QWidget.h:33
 */
static int QWidget_activateWindow(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in activateWindow");
    self->activateWindow();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "activateWindow: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "activateWindow: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::addAction(Action *action)
 * mimas/bind/QWidget.h:35
 */
static int QWidget_addAction(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in addAction");
    Action *action = *((Action **)dubL_checksdata(L, 2, "mimas.Action"));
    self->addAction(action);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addAction: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addAction: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::addWidget(QWidget *widget)
 * mimas/bind/QWidget.h:45
 */
static int QWidget_addWidget(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in addWidget");
    QWidget *widget = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    widget->setParent(self);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addWidget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addWidget: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::adjustSize()
 * mimas/bind/QWidget.h:21
 */
static int QWidget_adjustSize(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in adjustSize");
    self->adjustSize();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "adjustSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "adjustSize: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QWidget::close()
 * mimas/bind/QWidget.h:27
 */
static int QWidget_close(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in close");
    bool  retval__ = self->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "close: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "close: Unknown exception");
  }
  return lua_error(L);
}



/** QString mimas::GroupBox::cssClass() const 
 * include/mimas/GroupBox.h:63
 */
static int GroupBox_cssClass(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in cssClass");
    QString  retval__ = self->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "cssClass: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "cssClass: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::globalMove(float x, float y)
 * mimas/bind/QWidget.h:84
 */
static int QWidget_globalMove(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in globalMove");
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    self->move(
      self->mapToParent(
        self->mapFromGlobal(QPoint(x, y))
      )
    );
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "globalMove: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "globalMove: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::QWidget::globalPosition()
 * mimas/bind/QWidget.h:75
 */
static int QWidget_globalPosition(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in globalPosition");
    QPoint pt = self->mapToGlobal(QPoint(0, 0));
    lua_pushnumber(L, pt.x());
    lua_pushnumber(L, pt.y());
    return 2;
  } catch (std::exception &e) {
    lua_pushfstring(L, "globalPosition: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "globalPosition: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::QWidget::height()
 * mimas/bind/QWidget.h:18
 */
static int QWidget_height(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in height");
    int  retval__ = self->height();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "height: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "height: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::hide()
 * mimas/bind/QWidget.h:30
 */
static int QWidget_hide(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in hide");
    self->hide();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "hide: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "hide: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QWidget::isFullScreen()
 * mimas/bind/QWidget.h:34
 */
static int QWidget_isFullScreen(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in isFullScreen");
    bool  retval__ = self->isFullScreen();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QWidget::isVisible()
 * mimas/bind/QWidget.h:28
 */
static int QWidget_isVisible(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in isVisible");
    bool  retval__ = self->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isVisible: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isVisible: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::lower()
 * mimas/bind/QWidget.h:31
 */
static int QWidget_lower(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in lower");
    self->lower();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lower: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lower: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::move(int x, int y)
 * mimas/bind/QWidget.h:13
 */
static int QWidget_move(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in move");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    self->move(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "move: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "move: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::name()
 * mimas/bind/QObject.h:12
 */
static int QObject_name(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in name");
    lua_pushstring(L, self->objectName().toUtf8().data());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::object()
 * mimas/bind/QObject.h:11
 */
static int QObject_object(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in object");
    QObject * retval__ = self;
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "object: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::QWidget::position()
 * mimas/bind/QWidget.h:80
 */
static int QWidget_position(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in position");
    lua_pushnumber(L, self->x());
    lua_pushnumber(L, self->y());
    return 2;
  } catch (std::exception &e) {
    lua_pushfstring(L, "position: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "position: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::raise()
 * mimas/bind/QWidget.h:32
 */
static int QWidget_raise(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in raise");
    self->raise();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "raise: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "raise: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::resize(int w, int h)
 * mimas/bind/QWidget.h:14
 */
static int QWidget_resize(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in resize");
    int w = dubL_checkint(L, 2);
    int h = dubL_checkint(L, 3);
    self->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "resize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "resize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setMinimumSize(float w, float h)
 * mimas/bind/QWidget.h:23
 */
static int QWidget_setMinimumSize(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setMinimumSize");
    float w = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    self->setMinimumSize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setMinimumSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setMinimumSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setMouseTracking(bool enable)
 * mimas/bind/QWidget.h:26
 */
static int QWidget_setMouseTracking(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setMouseTracking");
    bool enable = lua_toboolean(L, 2);
    self->setMouseTracking(enable);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setMouseTracking: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setMouseTracking: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::setName(const char *name)
 * mimas/bind/QObject.h:13
 */
static int QObject_setName(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setName");
    const char *name = dubL_checkstring(L, 2);
    self->setObjectName(QString(name));
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setParent(QWidget *parent)
 * mimas/bind/QWidget.h:19
 */
static int QWidget_setParent(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setParent");
    QWidget *parent = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    self->setParent(parent);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setParent: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setParent: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setSizeHint(float w, float h)
 * mimas/bind/QWidget.h:61
 */
static int QWidget_setSizeHint(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setSizeHint");
    float w = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    self->size_hint_ = QSize(w, h);
    self->updateGeometry();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSizeHint: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSizeHint: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setSizePolicy(int horizontal, int vertical)
 * mimas/bind/QWidget.h:65
 */
static int QWidget_setSizePolicy(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setSizePolicy");
    int horizontal = dubL_checkint(L, 2);
    int vertical = dubL_checkint(L, 3);
    self->setSizePolicy((QSizePolicy::Policy)horizontal, (QSizePolicy::Policy)vertical);
    self->updateGeometry();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSizePolicy: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSizePolicy: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setStyle(const char *text)
 * mimas/bind/QWidget.h:53
 */
static int QWidget_setStyle(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setStyle");
    const char *text = dubL_checkstring(L, 2);
    self->setStyleSheet(QString(".%1 { %2 }").arg(self->cssClass()).arg(text));
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setStyleSheet(const char *text)
 * mimas/bind/QWidget.h:54
 */
static int QWidget_setStyleSheet(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setStyleSheet");
    const char *text = dubL_checkstring(L, 2);
    self->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyleSheet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyleSheet: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setWindowTitle(const char *text)
 * mimas/bind/QWidget.h:38
 */
static int QWidget_setWindowTitle(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in setWindowTitle");
    const char *text = dubL_checkstring(L, 2);
    self->setWindowTitle(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setWindowTitle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setWindowTitle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::show()
 * mimas/bind/QWidget.h:29
 */
static int QWidget_show(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in show");
    self->show();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "show: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "show: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::showFullScreen(bool enable=true)
 * mimas/bind/QWidget.h:67
 */
static int QWidget_showFullScreen(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in showFullScreen");
    int top__ = lua_gettop(L);
    bool enable;
    if (top__ >= 2) {
      enable = lua_toboolean(L, 2);
    } else {
      enable = true;
    }
    if (enable) {
      self->showFullScreen();
    } else {
      self->showNormal();
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "showFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "showFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::QWidget::size()
 * mimas/bind/QWidget.h:52
 */
static int QWidget_size(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in size");
    QRect rect = self->geometry();
    lua_pushnumber(L, rect.width());
    lua_pushnumber(L, rect.height());
    return 2;
  } catch (std::exception &e) {
    lua_pushfstring(L, "size: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "size: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::swapFullScreen()
 * mimas/bind/QWidget.h:71
 */
static int QWidget_swapFullScreen(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in swapFullScreen");
    if (!self->isFullScreen()) {
      self->showFullScreen();
    } else {
      self->showNormal();
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "swapFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "swapFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::textSize(const char *text)
 * mimas/bind/QWidget.h:57
 */
static int QWidget_textSize(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in textSize");
    const char *text = dubL_checkstring(L, 2);
    lua_pushnumber(L, self->fontMetrics().width(text));
    lua_pushnumber(L, self->fontMetrics().height());
    return 2;
  } catch (std::exception &e) {
    lua_pushfstring(L, "textSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "textSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::update()
 * mimas/bind/QWidget.h:20
 */
static int QWidget_update(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in update");
    self->update();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "update: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "update: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::widget()
 * mimas/bind/QWidget.h:44
 */
static int QWidget_widget(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in widget");
    QWidget* retval__ = self;
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "widget: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::QWidget::width()
 * mimas/bind/QWidget.h:17
 */
static int QWidget_width(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in width");
    int  retval__ = self->width();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "width: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "width: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::QWidget::windowTitle()
 * mimas/bind/QWidget.h:48
 */
static int QWidget_windowTitle(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in windowTitle");
    lua_pushstring(L, self->windowTitle().toUtf8());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "windowTitle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "windowTitle: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::QWidget::x()
 * mimas/bind/QWidget.h:15
 */
static int QWidget_x(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in x");
    int  retval__ = self->x();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "x: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "x: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::QWidget::y()
 * mimas/bind/QWidget.h:16
 */
static int QWidget_y(lua_State *L) {
  try {
    GroupBox *self = *((GroupBox**)dubL_checksdata(L, 1, "mimas.GroupBox"));
    if (!self) throw dub::Exception("Using deleted mimas.GroupBox in y");
    int  retval__ = self->y();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "y: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "y: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg GroupBox_member_methods[] = {
  {"activateWindow"    , QWidget_activateWindow},
  {"addAction"         , QWidget_addAction},
  {"addWidget"         , QWidget_addWidget},
  {"adjustSize"        , QWidget_adjustSize},
  {"close"             , QWidget_close},
  {"cssClass"          , GroupBox_cssClass},
  {"globalMove"        , QWidget_globalMove},
  {"globalPosition"    , QWidget_globalPosition},
  {"height"            , QWidget_height},
  {"hide"              , QWidget_hide},
  {"isFullScreen"      , QWidget_isFullScreen},
  {"isVisible"         , QWidget_isVisible},
  {"lower"             , QWidget_lower},
  {"move"              , QWidget_move},
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"position"          , QWidget_position},
  {"raise"             , QWidget_raise},
  {"resize"            , QWidget_resize},
  {"setMinimumSize"    , QWidget_setMinimumSize},
  {"setMouseTracking"  , QWidget_setMouseTracking},
  {"setName"           , QObject_setName},
  {"setParent"         , QWidget_setParent},
  {"setSizeHint"       , QWidget_setSizeHint},
  {"setSizePolicy"     , QWidget_setSizePolicy},
  {"setStyle"          , QWidget_setStyle},
  {"setStyleSheet"     , QWidget_setStyleSheet},
  {"setWindowTitle"    , QWidget_setWindowTitle},
  {"show"              , QWidget_show},
  {"showFullScreen"    , QWidget_showFullScreen},
  {"size"              , QWidget_size},
  {"swapFullScreen"    , QWidget_swapFullScreen},
  {"textSize"          , QWidget_textSize},
  {"update"            , QWidget_update},
  {"widget"            , QWidget_widget},
  {"width"             , QWidget_width},
  {"windowTitle"       , QWidget_windowTitle},
  {"x"                 , QWidget_x},
  {"y"                 , QWidget_y},
  {"__tostring"        , GroupBox__tostring},
  {"__gc"              , GroupBox_destructor},
  {"deleted"           , GroupBox_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg GroupBox_namespace_methods[] = {
  {"GroupBox"          , GroupBox_GroupBox},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_GroupBox_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_GroupBox_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.GroupBox");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, GroupBox_member_methods);
  // save meta-table in mimas.GroupBox_
  register_mt(L, "mimas", "GroupBox");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", GroupBox_namespace_methods);


	return 1;
}
