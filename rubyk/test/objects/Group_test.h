// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "test_helper.h"


class GroupTest : public NodeTestHelper
{
public:
  void test_new_inlet( void )
  {
    create("/one", "Number", "value: 1"    , "[5] \"/one\"");
    create("/grp", "Group",  ""            , "[9] \"/grp\"");
    create("/grp/two", "Number", "value: 2", "[15] \"/grp/two\"");
    create("/grp/val", "Inlet",  ""        , "[19] \"/grp/val\""); // /grp/val, /grp/in/val
    assert_call("/one/out/value/link", "/grp/in/val", "/grp/in/val");         // one ---> grp
    assert_call("/grp/val/out/port/link", "/grp/two/in/bang", "/grp/two/in/bang"); // val ---> two
    
    assert_call("/grp/two/value", "", "[11] 2.00");
    assert_call("/one/in/bang", "Bang!", "Nil"); // one ---> grp ---> Inlet (val) --> two
    assert_call("/grp/two/value", "", "[11] 1.00");
  }
  
  void test_new_outlet( void )
  {
    create("/one", "Number", "value: 1"    , "[5] \"/one\"");
    create("/grp", "Group",  ""            , "[9] \"/grp\"");
    create("/grp/two", "Number", "value: 2", "[15] \"/grp/two\"");
    create("/grp/res", "Outlet",  ""        , "[19] \"/grp/res\""); // /grp/res, /grp/out/res
    assert_call("/grp/two/out/value/link", "/grp/res/in/bang", "/grp/res/in/bang"); // /grp/two ---> /grp/res
    assert_call("/grp/out/res/link",       "/one/in/bang", "/one/in/bang");    // /grp/out/res ---> /one

    assert_call("/one/value", "", "[1] 1.00");
    assert_call("/grp/two/in/bang", "Bang!", "Nil"); // /grp/two ---> /grp/res ---> Outlet (res) --> /one
    assert_call("/one/value", "", "[1] 2.00");
  }
};