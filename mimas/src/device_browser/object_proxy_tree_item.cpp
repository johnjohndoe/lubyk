#include "mimas.h"
#include "object_proxy_tree_item.h"
#include "m_object_proxy.h"
#include "m_slider.h"

ObjectProxyTreeItem::ObjectProxyTreeItem(MObjectProxy *object) : subnodes_added_(false) {
  set_and_hold(&object_proxy_, object);
}

void ObjectProxyTreeItem::paintItem(Graphics& g, int width, int height) {
    // if this item is selected, fill it with a background colour..
  if (isSelected()) g.fillAll(Colours::blue.withAlpha (0.3f));
  // use a "colour" attribute in the xml tag for this node to set the text colour..
  g.setColour (Colours::black);

  g.setFont (height * 0.7f);

  // draw the xml element's tag name..
  g.drawText(String(object_proxy_->name().c_str()),
    4, 0, width - 4, height,
    Justification::centredLeft, true);
}

bool ObjectProxyTreeItem::mightContainSubItems() {
  return object_proxy_->children().size() > 0;
}

Component *ObjectProxyTreeItem::createItemComponent() {
  Value type = object_proxy_->type();
  if (type.type_id() == H("fffss")) {
    MSlider *slider;
    Component *component = new Component;
    for(int i = 0; i < 2; ++i) {
      slider = new MSlider(String(object_proxy_->url().c_str()));
      object_proxy_->connect(slider);
      slider->setBounds (100 + i * 250, 2, 250, 20);
      slider->setSliderStyle(i == 0 ? Slider::LinearHorizontal : Slider::Rotary);
      slider->setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);
      component->addAndMakeVisible(slider);
    }

    return component;
  } else {
    return 0; // use paintItem instead
  }
}

void ObjectProxyTreeItem::itemOpennessChanged(bool isNowOpen) {
  if (isNowOpen && !subnodes_added_) {
    // FIXME: make sure we do not accept changes to this tree now !
    const THash<std::string, Object *> children = object_proxy_->children();
    std::list<std::string>::const_iterator it = children.begin();
    std::list<std::string>::const_iterator end = children.end();
    for (; it != end; ++it) {
      std::cout << *it << " <<< added to object proxy\n";
      Object *object;
      assert(children.get(*it, &object));
      MObjectProxy *object_proxy = TYPE_CAST(MObjectProxy, object);
      assert(object_proxy);
      object_proxy->sync_children();
      addSubItem(object_proxy->object_proxy_view());
    }
    subnodes_added_ = true;
  }
}