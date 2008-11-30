#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include "rubyk_types.h"
#include "thash.h"
#include "value.h"
#include <string>

class Hash;

/* Holds the actual data of the Hash class. This is a wrapper around a THash. */
class HashData : public Data
{
public:
  HashData (const std::string& p) : mParameters(20) { build_hash(p); }
  HashData (const char * p)  : mParameters(20)      { build_hash( std::string(p) ); }
  HashData () : mParameters(20) {}
  HashData (unsigned int size) : mParameters(size)  {}
  
  virtual Data * clone()
  { return new HashData(*this); }
  virtual value_t type() const
  { return HashValue; }

  // copy constructor
  HashData(const HashData& pOther) : mParameters(pOther.storage_size())
  {
    mParameters = pOther.mParameters;
    mListValues = pOther.mListValues; // FIXME: what is this ?
  }

  virtual ~HashData() {}
  
  /** Display dictionary inside stream. */
  virtual void to_stream(std::ostream& pStream) const
  { 
    std::vector<std::string>::const_iterator it;
    std::vector<std::string>::const_iterator end = mParameters.end();
    std::string value;
#ifdef _TESTING_
    pStream << "<" << type_name() << "[" << mId << "] ";
#else
    pStream << "<" << type_name() << " ";
#endif
    pStream << *this << ">"; // FIXME: replace by << mParameters << ">";
  }
  
  void rebuild(const std::string& s)
  {
    clear();
    build_hash(s);
  }
  
  template<class T>
  T val(const char * pKey, T pDefault, bool pUseAnon = false) const
  {
    std::string value;
    if (mParameters.get(&value, std::string(pKey)))
      return cast_param<T>(value);
    else if (pUseAnon) {
      T value;
      if (get(&value)) return value;
      else return pDefault;
    } else
      return pDefault;
  }
  
  /** Try to get a parameter from a given key. Returns false if the key is not found. If the key is NULL, get the default value. */
  template<class T>
  bool get(T* pResult, const char * pKey, bool pUseAnon) const
  {
    std::string value;
    if (pKey == NULL) {
      if (mListValues.size() == 1) {
        // 'default' parameter
        *pResult = cast_param<T>(mListValues[0]);
        return true;
      } else
        return false;
    }
    if (mParameters.get(&value, std::string(pKey))) {
      *pResult = cast_param<T>(value);
      return true;
    } else if (pUseAnon) {
      return get(pResult, NULL, false);
    } else
      return false;
  }
  
  /** Try to get an anonymous parameter from it's index. Returns false if the key is out of range. */
  template<class T>
  bool get(T* pResult, size_t pIndex) const
  {
    std::string value;
    if (pIndex < mListValues.size()) {
      *pResult = cast_param<T>(mListValues[pIndex]);
      return true;
    } else
      return false;
  }
  
  /** Try to get an anonymous parameter from it's index. Returns false if the key is out of range. */
  template<class T>
  bool get(T* pResult, int pIndex) const
  {
    std::string value;
    if ((size_t)pIndex < mListValues.size()) {
      *pResult = cast_param<T>(mListValues[pIndex]);
      return true;
    } else
      return false;
  }
  
  /** Get default value. Return false if none found. */
  template<class T>
  bool get(T* pResult) const
  {
    return get(pResult, (size_t)0);
  }
  
  /** Try to get a parameter from a given key. Returns false if the key is not found (default value not used). If the key is NULL, get the default value. */
  template<class T>
  bool get(T* pResult, const char * pKey) const
  {
    return get(pResult, pKey, false);
  }
  // FIX: remove
  template<class T>
  T cast_param(const std::string& value) const
  {
    return (T)value;
  }
  
  void add (const char * str)
  {
    add(std::string(str));
  }
  
  void add (const std::string& pValue) {
    mListValues.push_back(pValue);
  }

  void set (const std::string& pKey, const std::string& pValue) {
    mParameters.set(pKey,pValue);
  }
  void set (const std::string& pKey, const char *       pValue) {
    mParameters.set(pKey, std::string(pValue));
  }
  void set (const char *       pKey, const std::string& pValue) {
    mParameters.set(std::string(pKey), pValue);
  }
  void set (const char *       pKey, const char *       pValue) {
    mParameters.set(std::string(pKey), std::string(pValue));
  }
  
  void clear () 
  { mParameters.clear(); mListValues.clear();  }
  
  //FIX size_t size () const
  //FIX { return mListValues.size(); }
  
  unsigned int storage_size() const
  {
    return mParameters.storage_size();
  }
  
  friend std::ostream& operator<< (std::ostream& pStream, const HashData& p);
  
private:  
  friend class Hash;
  THash<std::string,std::string>  mParameters; /** Hash of std::string => Value. */
  
  std::vector<std::string>  mListValues; //TODO: remove this and store values in a matrix as first element in dictionary.
  
  void build_hash (const std::string& p);
};

/** Value class to hold a single number (real_t). */
class Hash : public Value
{
public:
  VALUE_METHODS(Hash, HashData, HashValue, Value)
  
  Hash(const std::string& s) : Value(new HashData(s)) {}
  
  /** Set Hash from a string. */
  Hash& operator= (const std::string& s)
  {
    if (!mPtr) {
      mPtr = new Ptr(new HashData(s));
    } else {
      copy_if_shared();
      *(data_pointer()) = s;
    }
    return *this;
  }
  
  /** Rebuild the whole dictionary from a single string. */
  void rebuild(const std::string& s)
  {
    mutable_data()->rebuild(s);
  }
  
  size_t size() const
  {
    return mPtr ? data_pointer()->mParameters.size() : 0;
  }
  
  /** Return a Value or Nil from a string key. */
  const Value operator[] (const std::string& pKey) const;
  
  const Value operator[] (const char * c) const
  {
    return (*this)[std::string(c)];
  }
  // ADD PROXY METHODS HERE...
  
  template<class T>
  static T cast_param(const std::string& value)
  {
    return (T)value;
  }
};

template<>
int Hash::cast_param (const std::string& value);

template<>
bool Hash::cast_param (const std::string& value);

template<>
unsigned int Hash::cast_param (const std::string& value);

template<>
unsigned char Hash::cast_param (const std::string& value);

template<>
time_t Hash::cast_param (const std::string& value);

template<>
size_t Hash::cast_param (const std::string& value);

template<>
real_t Hash::cast_param (const std::string& value);

template<>
float Hash::cast_param (const std::string& value);

template<>
const char * Hash::cast_param (const std::string& value);

template<>
char Hash::cast_param (const std::string& value);

std::ostream& operator<< (std::ostream& pStream, const HashData& p);

#endif // _DICTIONARY_H_