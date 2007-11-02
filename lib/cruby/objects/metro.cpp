#include "node.h"
#include <cstdio>

class Metro : public Node
{
public:
  bool init(const Params& p)
  {
    mTempo = p.get("metro", 0.5); // 120bmp default
    make_inlet<Metro,&Metro::set_tempo>();
    make_outlet<Metro,&Metro::tic>();
    tic();
    return true;
  }

  void set_tempo(const Signal& sig)
  { SET_FLOAT(mTempo, sig) }
  
  virtual void spy()
  { spy_print("%.2f", mTempo );  }
  
  void tic(Signal& sig)
  {
    if (mTempo != 0) {
      bang_me_in(1.0 / mTempo);
      SEND_BANG(sig);
    }
  }
  
private:
  float mTempo;
};

extern "C" void init()
{
  Node::declare<Metro>("Metro");
}