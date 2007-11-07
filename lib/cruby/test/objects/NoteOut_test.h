#include "test_helper.h"

class TestNoteOut : public CxxTest::TestSuite, public ParseTest
{
public:

  void _defaults( void ) 
  { assert_result("no=NoteOut()\n","#<NoteOut:no  1:C0(80), 500>\n"); }

  void _note_out( void ) 
  { assert_prints("no=NoteOut()\nno.1 => 1.print\nno.bang\n","<note  1:C0(80)>\n"); }

  void test_note_on_off( void ) 
  { assert_run(100,"no=NoteOut(length:50 velocity:40)\nno.1 => 1.print\nno.bang\n","<note  1:C0(40)>\n<note -1:C0(40)>\n"); }
};