#ifndef BCLASS_TYPENAME_MAPPER_H
#define BCLASS_TYPENAME_MAPPER_H

#include "XSCUtility\XSC_StringTools.h"
#include <string>

namespace XSC
{
  template<typename T>
  class BClassTypenameMapper
  {
  public:

    void setStringValue(const std::string& iValue)
    {
      T wData;
      if (true == StrTools::setStringToValue(iValue, wData))
      {
        mData = wData;
      }
    }

    void getStringValue( std::string& oValue) const
    {
      oValue = StrTools::printValueToString(mData);
    }

    void setData(const T& iValue)
    {
      mData = iValue;
    }

    T& getData()
    {
      return mData;
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

    T& operator=(const T& iValue)
    {
      mData = iValue;
      return mData;
    }

    template<typename TInput>
    T& operator=(TInput iInput)
    {
      mData = iInput;
      return *this;
    }

    T& operator=(const T iValue)
    {
      mData = iValue;
      return mData;
    }

    T& operator+=(const T& iValue)
    {
      mData += iValue;
      return mData;
    }

    T& operator+=(const T iValue)
    {
      mData += iValue;
      return mData;
    }

    T& operator-=(const T& iValue)
    {
      mData -= iValue;
      return mData;
    }

    T& operator-=(const T iValue)
    {
      mData -= iValue;
      return mData;
    }

    T& operator*=(const T& iValue)
    {
      mData *= iValue;
      return mData;
    }

    T& operator*=(const T iValue)
    {
      mData *= iValue;
      return mData;
    }

    T& operator/=(const T& iValue)
    {
      mData /= iValue;
      return mData;
    }

    T& operator/=(const T iValue)
    {
      mData /= iValue;
      return mData;
    }

    T& operator%=(const T& iValue)
    {
      mData %= iValue;
      return mData;
    }

    T& operator%=(const T iValue)
    {
      mData %= iValue;
      return mData;
    }

    T& operator++()
    {
      ++mData;
      return mData;
    }

    const T operator++(int)
    {
      T wVal = mData;
      ++mData;
      return wVal;
    }

    T& operator--()
    {
      --mData;
      return mData;
    }

    const T operator--(int)
    {
      T wVal = mData;
      --mData;
      return wVal;
    }

  protected:
    T mData;
  };
}


#endif