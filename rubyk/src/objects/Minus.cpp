#include "class.h"

class Minus : public Node
{
public:
  bool set(const Value& p)
  {
    mValue = p.val("minus", 1.00, true);
    return true;
  }
  
  // inlet 1
  void bang(const Value& sig)
  {  
    real_t d;
    if (sig.type == MatrixValue) {
      mBuffer.copy(sig);
      mBuffer -= mValue;
      send(mBuffer);
    } else if (sig.get(&d)) {
      // single value
      d -= mValue;
      send(d);
    } else {
      // pass through
      send(sig);
    }
  }
  
  // inlet 2
  void set_minus(const Value& sig)
  {
    sig.get(&mValue);
  }
  
  virtual void spy()
  { bprint(mSpy, mSpySize,"-%.2f", mValue );  }
  
  
private:
  Matrix mBuffer;
  real_t mValue;
};

extern "C" void init()
{
  CLASS( Minus)
  INLET( Minus, set_minus)
  OUTLET(Minus,bang)
}