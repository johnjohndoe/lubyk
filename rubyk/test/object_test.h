// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "object.h"
#include "test_helper.h"

class DummyObject : public Object
{
public:
  DummyObject(const char * pName, int pCounter) : Object(pName), mCounter(pCounter) {}
  
  virtual const Value inspect(const Value& val)
  {
    std::ostringstream os(std::ostringstream::out);
    os << mName << ": " << mCounter;
    return String(os.str());
  }
private:
  int mCounter;
};

class ObjectTest : public ValueTestHelper
{
public:
  void test_root( void )
  {
    Object * root = NULL;
    Object a("a"); // automatically adopted by sRoot
    Object * a2   = NULL;
    TS_ASSERT( Object::get(&root, "") );
    TS_ASSERT_EQUALS( root, &Object::sRoot );
    TS_ASSERT( Object::get(&a2, "/a") );
    TS_ASSERT_EQUALS( a2, &a );
  }
  void test_adopt( void )
  {
    Object root("root");
    Object * one = root.adopt(new Object("one")); // This is the prefered way of creating sub-objects.
    Object * sub = one->adopt(new Object("sub"));
  
    TS_ASSERT_EQUALS( root.url(),     std::string("/root") );
    TS_ASSERT_EQUALS( one->url(),     std::string("/root/one") );
    TS_ASSERT_EQUALS( sub->url(),     std::string("/root/one/sub") );
    
    TS_ASSERT_EQUALS( root.child("one"), one);
    TS_ASSERT_EQUALS( root.child("foo"), (Object*)NULL);
    
    one->set_name("foo");
    TS_ASSERT_EQUALS( root.child("one"), (Object*)NULL);
    TS_ASSERT_EQUALS( root.child("foo"), one);
    TS_ASSERT_EQUALS( Object::find("/root/one/sub"), (Object*)NULL);
    TS_ASSERT_EQUALS( Object::find("/root/foo/sub"), sub);
  }
  
  void test_set_parent( void )
  {
    Object root("root");
    Object * child1 = new Object("foo");
    Object * child2 = new Object("bar");
    Object * child3 = new Object("foo");
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( child1->url(),  std::string("/foo") );
    TS_ASSERT_EQUALS( child2->url(),  std::string("/bar") );
    TS_ASSERT_EQUALS( child3->url(),  std::string("/foo-1") );
    
    child1->set_parent(root);
    child3->set_name("foo");
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( child1->url(),  std::string("/root/foo") );
    TS_ASSERT_EQUALS( child2->url(),  std::string("/bar") );
    TS_ASSERT_EQUALS( child3->url(),  std::string("/foo") );
    
    child2->set_parent(child1);
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( child1->url(),  std::string("/root/foo") );
    TS_ASSERT_EQUALS( child2->url(),  std::string("/root/foo/bar") );
    TS_ASSERT_EQUALS( child3->url(),  std::string("/foo") );
    
    child3->set_parent(root);
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( child1->url(),  std::string("/root/foo") );
    TS_ASSERT_EQUALS( child2->url(),  std::string("/root/foo/bar") );
    TS_ASSERT_EQUALS( child3->url(),  std::string("/root/foo-1") );
  }
  
  void test_rename( void )
  {
    Object root("root");
    Object * foo = root.adopt(new Object("foo"));
    Object * bar = foo->adopt(new Object("bar"));
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( foo->url(),     std::string("/root/foo") );
    TS_ASSERT_EQUALS( bar->url(),     std::string("/root/foo/bar") );
    
    foo->set_name("super");
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( foo->url(),     std::string("/root/super") );
    TS_ASSERT_EQUALS( bar->url(),     std::string("/root/super/bar") );
  }
  
  void test_get_object( void )
  {
    Object root("root");
    Object * foo = root.adopt(new Object("foo"));
    Object * bar = foo->adopt(new Object("bar"));
    Object * res;
  
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( foo->url(),     std::string("/root/foo") );
    TS_ASSERT_EQUALS( bar->url(),     std::string("/root/foo/bar") );
  
    TS_ASSERT(Object::get(&res, "/root"));
    TS_ASSERT_EQUALS( res, &root );
    
    TS_ASSERT(Object::get(&res, "/root/foo"));
    TS_ASSERT_EQUALS( res, foo );
    
    TS_ASSERT(Object::get(&res, "/root/foo/bar"));
    TS_ASSERT_EQUALS( res, bar );
    
    
    TS_ASSERT( ! Object::get(&res, "/root/super"));
    TS_ASSERT( ! Object::get(&res, "/root/super/bar"));
    
    foo->set_name("super");
    
    TS_ASSERT(Object::get(&res, "/root/super"));
    TS_ASSERT_EQUALS( res, foo );
    
    TS_ASSERT(Object::get(&res, "/root/super/bar"));
    TS_ASSERT_EQUALS( res, bar );
    
    TS_ASSERT( ! Object::get(&res, "/root/foo"));
    TS_ASSERT( ! Object::get(&res, "/root/foo/bar"));
  }
  
  void test_call( void )
  {
    Object root("root");
    Object * one = root.adopt(new Object("one"));
    Object * two = root.adopt(new Object("two"));
    Object * sub = two->adopt(new Object("sub"));
    Value res, param;
    String str;
  
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( one->url(),     std::string("/root/one") );
    TS_ASSERT_EQUALS( two->url(),     std::string("/root/two") );
    TS_ASSERT_EQUALS( sub->url(),     std::string("/root/two/sub") );
    
    res = Object::call("/root",param);
    TS_ASSERT(!res.is_error());
    TS_ASSERT(res.set(str));
    TS_ASSERT( str == "one,two/" );
    
    res = Object::call("/root/one",param);
    TS_ASSERT(res.is_nil());
    
    res = Object::call("/root/two",param);
    TS_ASSERT(!res.is_error());
    TS_ASSERT(res.set(str));
    TS_ASSERT( str == "sub" );
  }
  
  void test_child( void )
  {
    Object root("root");
    Object * one = root.adopt(new Object("one"));
    Object * sub = one->adopt(new Object("sub"));
  
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( one->url(),     std::string("/root/one") );
    TS_ASSERT_EQUALS( sub->url(),     std::string("/root/one/sub") );
    
    TS_ASSERT_EQUALS( root.child("one"), one);
    TS_ASSERT_EQUALS( root.child("foo"), (Object*)NULL);
    
    one->set_name("foo");
    TS_ASSERT_EQUALS( root.child("one"), (Object*)NULL);
    TS_ASSERT_EQUALS( root.child("foo"), one);
    TS_ASSERT_EQUALS( Object::find("/root/one/sub"), (Object*)NULL);
    TS_ASSERT_EQUALS( Object::find("/root/foo/sub"), sub);
  }
  
  void test_call_bad_object( void )
  {
    Object root("root");
    Value res, param;
    
    Number n1(4);
    assert_id(n1, 1);
    assert_id(res, 0);
    
    res = Object::call("/foo",param);
    assert_id(res, 2);
    
    TS_ASSERT(res.is_error());
    TS_ASSERT_EQUALS( Error(res).message(), std::string("Object '/foo' not found.") );
  }
  
  void test_get_info( void )
  {
    Object root("root");
    Object no_info("foo");
    Value res, param;
    
    root.set_info("This is the root node.");
    res = Object::call("/root/#info",param);
    TS_ASSERT_EQUALS( res.to_string(), "[1] \"This is the root node.\"");
    res = Object::call("/foo/#info",param);
    TS_ASSERT_EQUALS( res.to_string(), "[2] \"\"");
    
    res = Object::call("/blah/#info",param);
    TS_ASSERT_EQUALS( Error(res).message(), std::string("Object '/blah/#info' not found.") );
  }
  
  void test_inspect( void )
  {
    Object root("root");
    DummyObject insp("foo", 23);
    Value res;
    
    root.set_info("This is the root node.");
    res = Object::call("/root/#inspect");
    TS_ASSERT_EQUALS( res.to_string(), "[1] \"This is the root node.\"");
    res = Object::call("/foo/#inspect");
    TS_ASSERT_EQUALS( res.to_string(), "[2] \"foo: 23\"");
    
    res = Object::call("/blah/#inspect");
    TS_ASSERT_EQUALS( Error(res).message(), std::string("Object '/blah/#inspect' not found.") );
  }
};