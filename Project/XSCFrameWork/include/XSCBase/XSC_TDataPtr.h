#ifndef XSC_T_DATAPTR_H
#define XSC_T_DATAPTR_H

#include "XSCBase\XSC_Base.hxx"
#include "XSCUtility\XSC_StringTools.h"

#include <sstream>

namespace XSC
{
  template<typename T>
  class XSC_TDataPtr : public XSC_Data
  {
  public:

    XSC_TDataPtr() : mDataPtr(nullptr)
    {
    }

    XSC_TDataPtr(T* iDataPtr) : mDataPtr(iDataPtr)
    {

    }

    virtual void setValue(const std::string& iValue)
    {
      if (nullptr != mDataPtr)
      {
        XSC::StrTools::setStringToValue(iValue, *mDataPtr);
      }
      return;
    }

    virtual std::string getValue() const
    {
      if (nullptr != mDataPtr)
      {
        return XSC::StrTools::printValueToString(*mDataPtr);
      }
      return "";
    }

    void setDataPtr(T& iData) const
    {
      mDataPtr = &iData;
    }

    const T* getDataPtr() const
    {
      return mDataPtr;
    }

    operator T*()
    {
      return mDataPtr;
    }

    operator const T*() const
    {
      return mDataPtr;
    }

  private:
    T* mDataPtr;
  };
  
}

template<typename T_Left, typename T_Right>
bool operator==(const T_Left& a, const XSC::XSC_TDataPtr<T_Right>& b)
{
  return a == b.getDataPtr();
}

template<typename T_Left, typename T_Right>
bool operator==(const XSC::XSC_TDataPtr<T_Right>& a, const T_Left& b)
{
  return a.getDataPtr() == b;
}

template<typename T_Left, typename T_Right>
bool operator<=(const T_Left& a, const XSC::XSC_TDataPtr<T_Right>& b)
{
  return a <= b.getDataPtr();
}

template<typename T_Left, typename T_Right>
bool operator<=(const XSC::XSC_TDataPtr<T_Right>& a, const T_Left& b)
{
  return a.getDataPtr() <= b;
}

template<typename T_Left, typename T_Right>
bool operator<(const T_Left& a, const XSC::XSC_TDataPtr<T_Right>& b)
{
  return a < b.getDataPtr();
}

template<typename T_Left, typename T_Right>
bool operator<(const XSC::XSC_TDataPtr<T_Right>& a, const T_Left& b)
{
  return a.getDataPtr() < b;
}

template<typename T_Left, typename T_Right>
bool operator>=(const T_Left& a, const XSC::XSC_TDataPtr<T_Right>& b)
{
  return a >= b.getDataPtr();
}

template<typename T_Left, typename T_Right>
bool operator>=(const XSC::XSC_TDataPtr<T_Right>& a, const T_Left& b)
{
  return a.getDataPtr() >= b;
}

template<typename T_Left, typename T_Right>
bool operator>(const T_Left& a, const XSC::XSC_TDataPtr<T_Right>& b)
{
  return a > b.getDataPtr();
}

template<typename T_Left, typename T_Right>
bool operator>(const XSC::XSC_TDataPtr<T_Right>& a, const T_Left& b)
{
  return a.getDataPtr() > b;
}

template<typename T_Left, typename T_Right>
bool operator&&(const T_Left& a, const XSC::XSC_TDataPtr<T_Right>& b)
{
  return a && b.getDataPtr();
}

template<typename T_Left, typename T_Right>
bool operator&&(const XSC::XSC_TDataPtr<T_Right>& a, const T_Left& b)
{
  return a.getDataPtr() && b;
}

template<typename T_Left, typename T_Right>
bool operator||(const T_Left& a, const XSC::XSC_TDataPtr<T_Right>& b)
{
  return a || b.getDataPtr();
}

template<typename T_Left, typename T_Right>
bool operator||(const XSC::XSC_TDataPtr<T_Right>& a, const T_Left& b)
{
  return a.getDataPtr() || b;
}

namespace XSC
{
  typedef XSC_TDataPtr<bool> XSC_BoolPtr;

  typedef XSC_TDataPtr<char> XSC_Int8Ptr;
  typedef XSC_TDataPtr<short> XSC_Int16Ptr;
  typedef XSC_TDataPtr<int> XSC_Int32Ptr;
  typedef XSC_TDataPtr<long> XSC_Int64Ptr;

  typedef XSC_TDataPtr<unsigned char> XSC_UInt8Ptr;
  typedef XSC_TDataPtr<unsigned short> XSC_UInt16Ptr;
  typedef XSC_TDataPtr<unsigned int> XSC_UInt32Ptr;
  typedef XSC_TDataPtr<unsigned long> XSC_UInt64Ptr;

  typedef XSC_TDataPtr<float> XSC_Float32Ptr;
  typedef XSC_TDataPtr<double> XSC_Float64Ptr;

  typedef XSC_TDataPtr<std::string> XSC_StringPtr;
}
#endif