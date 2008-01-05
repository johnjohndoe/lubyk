#include "test_helper.h"

class LuaTest : public CxxTest::TestSuite, public ParseTest
{
public:

  void test_return( void ) 
  { assert_print("n=Lua(\"function bang()\nreturn 77\nend\")\nn=>p\np=Print()\nn.bang\n","77.00\n"); }
  
  void test_send( void ) 
  { assert_print("n=Lua(\"function bang()\nsend(1,{1,2,3})\nend\")\np=Print()\nn=>p\nn.bang\n","<Matrix [  1.00  2.00  3.00 ], 1x3>\n"); }

  void test_create_load( void ) 
  { assert_print("n=Lua(load:\"test/fixtures/test.lua\")\nn.script\n", "n: script loaded.\nfunction bang()\n\treturn in1+12\nend\n\n"); }

  void test_load( void ) 
  { assert_print("n=Lua()\nn.load(\"test/fixtures/test.lua\")\nn.script\n", "n: script loaded.\nfunction bang()\n\treturn in1+12\nend\n\n"); }

  void test_input_matrix( void ) 
  { 
    setup_with_print("n=Lua(\"function bang()\nsend(1,{in1[1],in1[3]})\nend\")\n");
    
    assert_print("n.bang(1,2,3,4)\n",   "<Matrix [  1.00  3.00 ], 1x2>\n");
    assert_print("n.bang(-1,5,0)\n",    "<Matrix [ -1.00  0.00 ], 1x2>\n");
  }
  
  void test_send_note( void ) 
  { 
    setup_with_print("n=Lua(\"function bang()\nsend_note(1,in1,66,500,1,3)\nend\")\n");
    
    assert_print("n.bang(60)\n",   "<Midi +1:C3(66), 3/500>\n");
    assert_print("n.bang(61)\n",   "<Midi +1:C#3(66), 3/500>\n");
  }
  
  void test_send_chord( void ) 
  { 
    setup_with_print("n=Lua(\"function bang()\nsend_note(1,{in1,in1+7},66,500,1,3)\nend\")\n");

    assert_print("n.bang(60)\n",   "<Midi +1:C3(66), 3/500>\n<Midi +1:G3(66), 3/500>\n");
    assert_print("n.bang(61)\n",   "<Midi +1:C#3(66), 3/500>\n<Midi +1:G#3(66), 3/500>\n");
  }
  
  void test_send_ctrl( void ) 
  { 
    setup_with_print("n=Lua(\"function bang()\nsend_ctrl(1,20,in1,7,5)\nend\")\n");

    assert_print("n.bang(60)\n",   "<Midi ~7:20(60), 5>\n");
    assert_print("n.bang(61)\n",   "<Midi ~7:20(61), 5>\n");
  }
  
  
  void test_inputs( void ) 
  { 
    setup_with_print("n=Lua(\"function bang()\nsend(1,{in1 or 0,in2 or 0,in3 or 0,in4 or 0,in5 or 0,in6 or 0,in7 or 0,in8 or 0,in9 or 0,in10 or 0})\nend\")\n");
    
    assert_print("n.b(11)\n",              "<Matrix [  11.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in2(22)\nn.b(11)\n",   "<Matrix [  11.00  22.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in3(33)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in4(44)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in5(55)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in6(66)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in7(77)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in8(88)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in9(9.9)\nn.b(11)\n",  "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  9.90  0.00 ], 1x10>\n");
    assert_print("n.in10(1.1)\nn.b(11)\n", "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  9.90  1.10 ], 1x10>\n");
  }
  
  void test_outputs( void ) 
  { 
    setup_with_print("n=Lua(\"function bang()\nsend(6,in1)\nend\")\nn.6 => v\nv = Value()\nn // p\nv => p\n");
    
    assert_print("n.b(123.34)\n",              "123.34\n");
  }
};