#ifndef _ERROR_VALUE_H_
#define _ERROR_VALUE_H_
#include "string_value.h"

class Error;

/* Holds the actual data of the Error class. This is a sub-class of StringData / std::string. */
class ErrorData : public StringData
{
public:
  DATA_METHODS(ErrorData, ErrorValue)
  
  ErrorData(const std::string& s) : StringData(s) {}

  ErrorData(const char * s) : StringData(s) {}
  
  // copy constructor
  ErrorData(const ErrorData& v) : StringData(v) {}
  
  virtual ~ErrorData() {}
  
  virtual void to_stream(std::ostream& pStream) const
  {
#ifdef _TESTING_
    pStream << "[" << mId << "]" << " #\"" << mString << "\"";
#else
    pStream << "#\"" << mString << "\"";
#endif
  }
private:
  friend class Error;
};

/** Value class to hold an error message (sub-class of String). */
class Error : public String
{
public:
  VALUE_METHODS(Error, ErrorData, ErrorValue, String)
  Error(const std::string& s) : String(new ErrorData(s)) {}
  Error(const char* s) : String(new ErrorData(std::string(s))) {}
  
  const char * message() const
  {
    if (mPtr) {
      return data_pointer()->mString.c_str();
    } else {
      return "";
    }
  }
};

#endif // _ERROR_VALUE_H_