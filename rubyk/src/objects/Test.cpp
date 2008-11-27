#include "class.h"
#include <iostream>

class Test : public Node
{
public:
  
  bool set (const Value& p)
  {
    mMessage = p.val("message", "Hello World !", true);
    if (mMessage == "is output ok?")
      *mOutput << "Output set" << std::endl;
      
    mCounter = p.val("counter", 0);
    return true;
  }
  
  static void hello(std::ostream * pOutput, const Value& p)
  {
    *pOutput << "Hello World!\n";
  }
  
  void spy() 
  { bprint(mSpy, mSpySize,"'%s' %i", mMessage.c_str(), mCounter); }
  
  void help()
  { *mOutput << "Don't hit me!\n"; }

  // inlet 1
  void bang(const Value& sig)
  {
    sig.get(&mCounter);
    send(++mCounter);
    
    *mOutput << "sending nil=>";
    send(2, gNilValue);
    *mOutput << "<=done.\n";
  }
  
  void info(const Value& p)
  {
    *mOutput << p << std::endl;
  }
  
  void test_fail(const Value& p)
  {
    if (!test_try()) {
      *mOutput << "try failed";
    }
  }
  
  
private:
  bool test_try() {
    Matrix m,n;
    m.set_sizes(5,1);
    m.fill(2.0);
    TRY(n, copy(m, 10, 10));
    return true;
  }
  
  /* data */
  std::string mMessage;
  int mCounter;
};


extern "C" void init()
{ 
  CLASS (Test)
  OUTLET(Test, increment_counter)
  OUTLET(Test, send_nil)
  METHOD(Test, info)
  METHOD(Test, test_fail)
  CLASS_METHOD(Test, hello)
}
