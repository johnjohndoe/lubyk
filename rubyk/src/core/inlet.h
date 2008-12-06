#ifndef _INLET_H_
#define _INLET_H_
#include "slot.h"
#include "values.h"

typedef void(*inlet_method_t)(void * node, const Value& val);

/** Prototype constructor for Inlets. */
struct InletPrototype
{
  InletPrototype(const char * pName, uint pType, inlet_method_t pMethod, const char * pInfo) : mName(pName), mType(pType), mMethod(pMethod), mInfo(pInfo) {}
  const char *   mName;
  uint           mType;
  inlet_method_t mMethod;
  const char *   mInfo;
};

class Inlet : public Slot {
public:
  /** Constructor used for testing. */
  Inlet (void* node, inlet_method_t pMethod, uint pType) : Slot(node, pType), mMethod(pMethod) {}
  
  Inlet (void* node, inlet_method_t pMethod) : Slot(node), mMethod(pMethod) {}
  
  /** Prototype based constructor. */
  Inlet (void * pNode, const InletPrototype& pProto) : Slot(pProto.mName, pNode, pProto.mType), mMethod(pProto.mMethod)
  {
    set_info(pProto.mInfo);
  }
  
  /** Receive a value. */
  void receive (const Value& sig);
  
  /** Create a callback for an inlet. */
  template <class T, void(T::*Tmethod)(const Value& val)>
  static void cast_method (void * receiver, const Value& val)
  {
    (((T*)receiver)->*Tmethod)(val);
  }
  
private:
  inlet_method_t mMethod;        /**< Method to set a new value. */
};

#endif