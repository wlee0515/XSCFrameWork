#ifndef XSC_T_DATA_HXX
#define XSC_T_DATA_HXX

#include "XSCBase/XSC_Base.hxx"
#include "XSCUtility/XSC_StringTools.h"

#include <sstream>
#include <cstring>

namespace XSC
{
  template<typename T>
  class XSC_TData : public XSC_Data
  {
  public:

    XSC_TData() 
    {
    }

    XSC_TData(const T& iData) : mData(iData)
    {

    }

    virtual void setValue(const std::string& iValue)
    {
      XSC::StrTools::setStringToValue(iValue, mData);
      return;
    }

    virtual std::string getValue() const
    {
      return XSC::StrTools::printValueToString(mData);
    }

    const T& getData() const
    {
      return mData;
    }

    operator T&()
    {
      return mData;
    }

    operator const T&() const
    {
      return mData;
    }

    XSC_TData& operator=(const T& a)
    {
      mData = a;
      return *this;
    }

    XSC_TData& operator++()
    {
      ++mData;
      return *this;
    }

    XSC_TData& operator--()
    {
      --mData;
      return *this;
    }

  private:
    T mData;
  };
  
}

template<typename T_Left, typename T_Right>
bool operator==(const T_Left& a, const XSC::XSC_TData<T_Right>& b)
{
  return a == b.getData();
}

template<typename T_Left, typename T_Right>
bool operator==(const XSC::XSC_TData<T_Right>& a, const T_Left& b)
{
  return a.getData() == b;
}

template<typename T_Left, typename T_Right>
bool operator<=(const T_Left& a, const XSC::XSC_TData<T_Right>& b)
{
  return a <= b.getData();
}

template<typename T_Left, typename T_Right>
bool operator<=(const XSC::XSC_TData<T_Right>& a, const T_Left& b)
{
  return a.getData() <= b;
}

template<typename T_Left, typename T_Right>
bool operator<(const T_Left& a, const XSC::XSC_TData<T_Right>& b)
{
  return a < b.getData();
}

template<typename T_Left, typename T_Right>
bool operator<(const XSC::XSC_TData<T_Right>& a, const T_Left& b)
{
  return a.getData() < b;
}

template<typename T_Left, typename T_Right>
bool operator>=(const T_Left& a, const XSC::XSC_TData<T_Right>& b)
{
  return a >= b.getData();
}

template<typename T_Left, typename T_Right>
bool operator>=(const XSC::XSC_TData<T_Right>& a, const T_Left& b)
{
  return a.getData() >= b;
}

template<typename T_Left, typename T_Right>
bool operator>(const T_Left& a, const XSC::XSC_TData<T_Right>& b)
{
  return a > b.getData();
}

template<typename T_Left, typename T_Right>
bool operator>(const XSC::XSC_TData<T_Right>& a, const T_Left& b)
{
  return a.getData() > b;
}

template<typename T_Left, typename T_Right>
bool operator&&(const T_Left& a, const XSC::XSC_TData<T_Right>& b)
{
  return a && b.getData();
}

template<typename T_Left, typename T_Right>
bool operator&&(const XSC::XSC_TData<T_Right>& a, const T_Left& b)
{
  return a.getData() && b;
}

template<typename T_Left, typename T_Right>
bool operator||(const T_Left& a, const XSC::XSC_TData<T_Right>& b)
{
  return a || b.getData();
}

template<typename T_Left, typename T_Right>
bool operator||(const XSC::XSC_TData<T_Right>& a, const T_Left& b)
{
  return a.getData() || b;
}

namespace XSC
{
  typedef XSC_TData<bool> XSC_Bool;

  typedef XSC_TData<char> XSC_Int8;
  typedef XSC_TData<short> XSC_Int16;
  typedef XSC_TData<int> XSC_Int32;
  typedef XSC_TData<long> XSC_Int64;

  typedef XSC_TData<unsigned char> XSC_UInt8;
  typedef XSC_TData<unsigned short> XSC_UInt16;
  typedef XSC_TData<unsigned int> XSC_UInt32;
  typedef XSC_TData<unsigned long> XSC_UInt64;

  typedef XSC_TData<float> XSC_Float32;
  typedef XSC_TData<double> XSC_Float64;

  typedef XSC_TData<std::string> XSC_String;
}
#endif