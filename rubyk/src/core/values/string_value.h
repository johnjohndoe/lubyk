#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "value.h"
#include <string>

class String;

/* Holds the actual data of the StringValue class. This is a wrapper around an std::string. */
class StringData : public Data
{
public:
  DATA_METHODS(StringData, StringValue)
  
  StringData(const std::string& s) : mString(s) {}
  
  StringData(const char * s) : mString(s) {}
  
  // copy constructor
  StringData(const StringData& v) : mString(v.mString) {}
  
  virtual ~StringData() {}
  
  virtual void to_stream(std::ostream& pStream) const
  {
#ifdef _TESTING_
    pStream << "[" << mId << "]" << " \"" << mString << "\"";
#else
    pStream << "\"" << mString << "\"";
#endif
  }
  
protected:
  friend class String;
  std::string mString;
};

#define GET_DATA(x) const StringData * x ## _data = x.data(); \
  if (!x ## _data) return false;

#define GET_THIS_DATA() const StringData * this_data = data(); \
    if (!this_data) return false;
  
/** Value class to hold a single number (real_t). */
class String : public Value
{
public:
  VALUE_METHODS(String, StringData, StringValue, Value)
  
  String(const std::string& s) : Value(new StringData(s)) {}
  
  String(const char * s)       : Value(new StringData(s)) {}
  
  /** Set String from std::string. */
  const std::string& operator= (const std::string& s)
  {
    mutable_data()->mString = s;
    return s;
  }
  
  /** Set String from const char *. */
  const char * operator= (const char * s)
  {
    mutable_data()->mString = s;
    return s;
  }
  
  void append(const std::string& s)
  {
    mutable_data()->mString.append(s);
  }
  
  void append(const char * s)
  {
    mutable_data()->mString.append(s);
  }
  
  bool operator==(const char * s)
  {
    GET_THIS_DATA()
    return this_data->mString == s;    
  }
  
  bool operator==(const std::string& s)
  {
    GET_THIS_DATA()
    return this_data->mString == s;    
  }
  
  std::string * string() // FIXME: maybe we should return a reference...
  {
    return mPtr ? &data_pointer()->mString : NULL; // FIXME: shouldn't we use mutable_data() here ?
  }
  
  const std::string * string() const
  {
    return mPtr ? &data_pointer()->mString : NULL;
  }
};

#undef GET_THIS_DATA
#undef GET_DATA
#endif // _STRING_VALUE_H_