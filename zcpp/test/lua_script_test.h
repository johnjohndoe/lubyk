/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include <stdio.h>
#include <string>

#include "test_helper.h"
#include "oscit/matrix.h"

class LuaScriptTest : public TestHelper {
public:
  virtual void setUp() {
    planet_ = new Planet();
    script_ = planet_->adopt(new LuaScript);
    // all this is done by Class normally
    script_->set_name("lua");
    script_->adopt(new Object("in"));
    script_->adopt(new Object(Rubyk::NODE_OUT_KEY));
    Value res = script_->init();
    if (res.is_error()) {
      std::cout << "Could not init LuaScript !\n" << res << "\n";
    }
  }

  void tearDown() {
    if (planet_) delete planet_;
    planet_ = NULL;
  }

  void test_view() {
    assert_equal("\"Script\"", script_->attributes()[Oscit::VIEW][Oscit::WIDGET].to_json());
  }

  void test_compile( void ) {
    Value res = parse("function foo()\nend");
    assert_true(res.is_string());
    assert_equal("function foo()\nend", res.str());
    assert_true(script_->is_ok());
  }

  void test_compilation_error( void ) {
    Value res = parse("function foo\nend");
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("[string \"lua\"]:2: \'(\' expected near \'end\'.", res.error_message());
    assert_false(script_->is_ok());
  }

  void test_runtime_error( void ) {
    Value res = parse("x = y + 3"); // y undefined
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("[string \"lua\"]:1: attempt to perform arithmetic on global 'y' (a nil value).", res.error_message());
    assert_false(script_->is_ok());
  }

  void test_exception_thrown_in_lua( void ) {
    Value res = parse("require('cv'); function foo()\n cv.subtract(cv.Mat(3,3,cv.CV_32FC1), cv.Mat(3,3,cv.CV_32FC2), cv.Mat(3,3,cv.CV_32FC2))\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", gNilValue);
    assert_true(res.is_error());
    assert_equal("Could not call foo(null): cv.subtract: failed (-215) size == src2.size() && type == src2.type() && func != 0 in function subtract\n", res.error_message());
  }

  // ------------------------------------------------------------------------  rk.

  void test_rk_call( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("function foo() return rk.call('/class/lib'); end");
    assert_true(res.is_string());
    res = script_->call_lua("foo", gNilValue);
    assert_equal("\"lib:~/rubyk/lib:/usr/local/lib/rubyk\"", res.to_json());
  }

  // ------------------------------------------------------------------------  Nil

  void test_call_lua_with_nil( void ) {
    Value res = parse("function foo()\n return 45\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", gNilValue);
    assert_equal(45.0, res.r);
  }

  // ------------------------------------------------------------------------  Real

  void test_add_inlet( void ) {
    Value res = parse("Inlet('tempo', RealIO('Main beat machine tempo.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/tempo", &inlet));
    assert_equal("\"f\"", inlet->attributes()[Oscit::TYPE][Oscit::SIGNATURE].to_json());
    assert_equal("\"Main beat machine tempo.\"", inlet->attributes()[Oscit::INFO].to_json());

    res = parse("Inlet('tempo', RealIO('Main beat machine tempo.'))");
    assert_true(res.is_string()); // no error
  }

  void test_add_inlet_RealIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("Inlet('tempo', RangeIO('Main beat machine tempo [bpm].', 0, 5000))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/tempo", &inlet));
    Value attrs(inlet->attributes());
    assert_equal("\"f\"", attrs[Oscit::TYPE][Oscit::SIGNATURE].to_json());
    assert_equal("0", attrs[Oscit::TYPE][Oscit::MIN].to_json());
    assert_equal("5000", attrs[Oscit::TYPE][Oscit::MAX].to_json());
    assert_equal("\"Main beat machine tempo [bpm].\"", attrs[Oscit::INFO].to_json());
  }

  void test_add_outlet_RealIO( void ) {
    Value res = parse("Outlet('force', RealIO('Dark Force.'))");
    assert_true(res.is_string());
    ObjectHandle outlet;
    assert_true(planet_->get_object_at("/lua/out/force", &outlet));
    assert_equal("\"f\"", outlet->attributes()[Oscit::TYPE][Oscit::SIGNATURE].to_json());
    assert_equal("\"Dark Force.\"", outlet->attributes()[Oscit::INFO].to_json());
  }

  // ------------------------------------------------------------------------  False

  void test_return_false( void ) {
    Value res = parse("function foo()\n return false\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_false());
  }

  // ------------------------------------------------------------------------  True (Bang)

  void test_return_true( void ) {
    Value res = parse("function foo()\n return true\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_true());
  }

  void test_receive_true( void ) {
    Value res = parse("function foo(v)\n return v == true\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", gTrueValue);
    assert_true(res.is_true());
  }

  void test_add_inlet_BangIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("Inlet('boom', BangIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/boom", &inlet));
    assert_equal("\"T\"", inlet->attributes()[Oscit::TYPE][Oscit::SIGNATURE].to_json());
    assert_equal("\"Ping pong.\"", inlet->attributes()[Oscit::INFO].to_json());
  }

  // ------------------------------------------------------------------------  Impulse
  // TODO: we could use a new type for impulse: Impulse
  // void test_return_true( void ) {
  //   Value res = parse("function foo()\n return Impulse\nend");
  //   assert_true(res.is_string());
  //   res = script_->call_lua("foo");
  //   assert_true(res.is_bang());
  // }
  //
  // void test_receive_true( void ) {
  //   Value res = parse("function foo(v)\n return v == Impulse\nend");
  //   assert_true(res.is_string());
  //   res = script_->call_lua("foo", gImpulseValue);
  //   assert_true(res.is_true());
  // }
  //
  // void test_add_inlet_ImpulseIO( void ) {
  //   // also tests loading of rubyk.lua
  //   Value res = parse("Inlet('boom', ImpulseIO('Ping pong.'))");
  //   assert_true(res.is_string());
  //   ObjectHandle inlet;
  //   assert_true(planet_->get_object_at("/lua/boom", &inlet));
  //   assert_equal("I", inlet->attributes()[Oscit::TYPE][Oscit::SIGNATURE].to_json());
  //   assert_equal("\"Ping pong.\"", inlet->attributes()[Oscit::INFO].to_json());
  // }

  // ------------------------------------------------------------------------  Matrix

  void test_create_matrix( void ) {
    Value matrix_value(Matrix(2,3,CV_32FC1));
    Value res = parse("require('cv'); function foo()\n return cv.Mat(2,3,cv.CV_32FC1)\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_matrix());
    assert_equal(matrix_value.matrix_->type(), res.matrix_->type());
  }

  void test_call_lua_with_matrix( void ) {
    Value matrix_value(Matrix(2,3,CV_32FC1));
    Value res = parse("require('cv'); function foo(m)\n return m\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", matrix_value);
    assert_true(res.is_matrix());
    assert_equal(matrix_value.matrix_->data, res.matrix_->data);
  }

  void test_add_inlet_MatrixIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("require('cv'); Inlet('boom', MatrixIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/boom", &inlet));
    assert_equal("\"M\"", inlet->attributes()[Oscit::TYPE][Oscit::SIGNATURE].to_json());
    assert_equal("\"Ping pong.\"", inlet->attributes()[Oscit::INFO].to_json());
  }

  // ------------------------------------------------------------------------  Hash

  void test_create_hash( void ) {
    Value res = parse("function foo()\n return {one=1, two=2}\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_hash());
    std::ostringstream oss;
    oss << res;
    assert_equal("{\"one\":1, \"two\":2}", oss.str());
  }

  // ------------------------------------------------------------------------  Midi

  void test_create_midi( void ) {
    Value res = parse("function foo()\n return oscit.MidiMessage_Note(65,68)\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_midi());
    std::ostringstream oss;
    oss << res;
    assert_equal("{\"=m\":[144, 65, 68, 500]}", oss.str());
  }

  void test_call_lua_with_MidiValue( void ) {
    MidiValue midi_value;
    midi_value.set_as_note(45, 78, 1000);
    Value res = parse("function foo(m)\n return m\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", midi_value);
    // moving the message into Lua makes a copy because we loose constness
    // if this is an important performance hit, we could discard const checking but
    // this could lead to other problems such as kept values in lua that keep changing
    // without proper mutex protections.
    assert_false(midi_value.midi_message_ == res.midi_message_);
  }

  void test_add_inlet_MidiIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("Inlet('boom', MidiIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/boom", &inlet));
    assert_equal("\"m\"", inlet->attributes()[Oscit::TYPE][Oscit::SIGNATURE].to_json());
    assert_equal("\"Ping pong.\"", inlet->attributes()[Oscit::INFO].to_json());
  }

  void test_add_outlet_MidiIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("Outlet('boom', MidiIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle outlet;
    assert_true(planet_->get_object_at("/lua/out/boom", &outlet));
    assert_equal("\"m\"", outlet->attributes()[Oscit::TYPE][Oscit::SIGNATURE].to_json());
    assert_equal("\"Ping pong.\"", outlet->attributes()[Oscit::INFO].to_json());
  }

  // ------------------------------------------------------------------------  List

  void test_call_lua( void ) {
    Value res = parse("function foo(x,y)\n return x+y\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", Value(5).push_back(9));
    assert_equal(14.0, res.r);
  }

  // sending tested in LuaTest

private:
  const Value parse(const char *string) {
    return script_->script(Value(string));
  }

  Planet    *planet_;
  LuaScript *script_;
};