#include "planet.h"
#include "class_finder.h"
#include "node.h"
#include "text_command.h"

void Planet::init() {
  // build application methods
  //           /.inspect
  adopt(new TMethod<Planet, &Planet::inspect>(this, Url(INSPECT_URL).name(), StringIO("url", "Returns some information on the state of a node.")));
  //          /class
  classes_ = adopt(new ClassFinder(Url(CLASS_URL).name(), DEFAULT_OBJECTS_LIB_PATH));
  //          /rubyk
  Object * rubyk = adopt(new Object(Url(RUBYK_URL).name()));
  //          /rubyk/link [[["","source url"],["", "target url"]], "Create a link between two urls."]
  rubyk->adopt(new TMethod<Planet, &Planet::link>(this, Url(LINK_URL).name(), JsonValue("[['','', ''],'url','op','url','Update a link between the two provided urls. Operations are '=>' (link) '||' (unlink) or '?' (pending).']")));
  //          /rubyk/quit
  rubyk->adopt(new TMethod<Planet, &Planet::quit>(this, Url(QUIT_URL).name(), NoIO("Stop all operations and quit.")));
}

// typetag: "ss" (inlet, outlet)
const Value Planet::link(const Value &val) {
  // std::cout << "link: " << val << std::endl;
  if (val.is_nil()) {
    return create_pending_links();
  }
  
  Value error;
  Object *source = object_at(Url(val[0].str()), &error);
  if (error.is_error() || !object_at(Url(val[2].str()), &error)) {
    // not found
    if (val[1].str() == "=>") {
      Value params;
      params.copy(val);
      params[1].set("?");
      pending_links_.push_back(Call(LINK_URL, params));
      return params;
    }
    return val;
  }
  
  Slot   *slot = TYPE_CAST(Slot, source);
  Object *object;
  if (slot != NULL) {
    return val[1].str() == "||" ? slot->unlink(val[2]) : slot->link(val[2]);
  } else if ( (object = source->child("out")) && (object = object->first_child()) ) {
    // was a link default slots: /met/out --> /counter/in
    if ( (slot = TYPE_CAST(Slot, object)) ) {
      return val[1].str() == "||" ? slot->unlink(val[2]) : slot->link(val[2]);
    } else {
      return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(slot->url()).append("' does not support links (using first child of '").append(source->url()).append("')."));
    }
  } else {
    return Value(BAD_REQUEST_ERROR, std::string("Object at '").append(source->url()).append("' does not support links (not an Outlet, Inlet or Node)."));
  }
}

// FIXME: on node deletion/replacement, remove/move all pending links related to this node ?.
const Value Planet::create_pending_links() {
  std::list<Call>::iterator it  = pending_links_.begin();
  std::list<Call>::iterator end = pending_links_.end();
  
  Value res;
  Value list;
  
  while (it != end) {
    res = it->trigger(this);
    if ((res.type_id() == H("sss") && res[1].str() == "=>") || res.is_error()) {
      list.push_back(res);
      it = pending_links_.erase(it);  // call succeeded or definitely failed
    } else {
      ++it;
    }
  }
  // return list of created links
  return list;
}

