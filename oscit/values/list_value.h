#ifndef _LIST_VALUE_H_
#define _LIST_VALUE_H_
#include "oscit/value.h"
#include "oscit/values/list.h"

namespace oscit {

/** Value is the base type of all data transmitted between objects or used as parameters
and return values for osc messages.*/
class ListValue : public Value
{ 
  ListValue(const char * typeTag) : Value(ListValueType, typeTag) {}
};

} // oscit
#endif // _LIST_VALUE_H_