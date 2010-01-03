#include "mimas.h"
#include "m_slider.h"

#include "m_object_proxy.h"
#include "mimas_window_content.h"

void MSlider::update(const Value &def) {
  Value err;

  // ========================================== x, y, width, height
  MComponent::update(def);
  if (getWidth() > getHeight()) {
    set_slider_type(MSlider::HorizontalSliderType);
  } else {
    set_slider_type(MSlider::VerticalSliderType);
  }

  // ========================================== connect

  if (!proxy_ && def.has_key("connect")) {
  // TODO: implement disconnect so that we can change connection...
    const Value connect_path = def["connect"];
    if (!connect_path.is_string()) {
      error("invalid 'connect' attribute in", def);
    } else {
      MObjectProxy *proxy = TYPE_CAST(MObjectProxy, root_proxy_->find_or_build_object_at(connect_path.str(), &err));
      if (!proxy) {
        error("could not connect", err);
        // this should never happen since find_or_build_object_at should build dummy
        // objects that will receive a "type_changed" when they are ready...
        add_callback(connect_path.str(), def);
      } else {
        proxy->connect(this);
      }
    }
  }
}


bool MSlider::is_connected() {
  return proxy_ != NULL && proxy_->is_connected();
}

void MSlider::mouseDown(const MouseEvent &e) {
  if (mimas_->action_mode() && is_connected()) {
    is_dragged_ = true;
    mouseDrag(e);
  } else if (mimas_->edit_mode()) {
    dragger_.startDraggingComponent(this, 0);
  }
}

void MSlider::mouseDrag(const MouseEvent &e) {
  if (mimas_->action_mode() && is_connected()) {
    if (slider_type_ == HorizontalSliderType) {
      set_scaled_value(e.x, getWidth());
    } else {
      set_scaled_value(getHeight() - e.y, getHeight());
    }
    proxy_->set_value(Value(value_));

  } else if (mimas_->edit_mode()) {
    dragger_.dragComponent(this, e);
  }
}

void MSlider::mouseUp(const MouseEvent &e) {
  if (!is_connected()) return;
  is_dragged_ = false;
  last_drag_ = proxy_->time_ref().elapsed();
}