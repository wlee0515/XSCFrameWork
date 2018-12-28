#ifndef BCLASS_TYPENAME_PTR_MAPPER_H
#define BCLASS_TYPENAME_PTR_MAPPER_H

#include "XSCUtility\XSC_StringTools.h"

namespace XSC
{
  template<typename T>
  class BClassTypenamePtrMapper
  {
  public:

    BClassTypenamePtrMapper() : mDataPtr(&mData)
    {
    }

    BClassTypenamePtrMapper(const BClassTypenamePtrMapper& iValue) : mDataPtr(&mData)
    {
      *mDataPtr - iValue.getData();
    }

    void setStringValue(const std::string& iValue)
    {
      T wData;
      if (true == StrTools::setStringToValue(iValue, wData))
      {
        getData() = wData;
      }
    }

    void getStringValue(std::string& oValue) const
    {
      oValue = StrTools::printValueToString(getData());
    }

    void setPtr(T* iValue)
    {
      if (nullptr == iValue)
      {
        mDataPtr = &mData;
      }
      else
      {
        mDataPtr = iValue;
      }
    }

    void setData(const T& iValue)
    {
      *mDataPtr = iValue;
    }

    T& getData()
    {
      return *mDataPtr;
    }

    const T& getData() const
    {
      return *mDataPtr;
    }

    operator T&()
    {
      return *mDataPtr;
    }

    operator const T&() const
    {
      return *mDataPtr;
    }

    T& operator=(const T& iValue)
    {
      *mDataPtr = iValue;
      return *mDataPtr;
    }

    template<typename TInput>
    T& operator=(TInput iInput)
    {
      *mDataPtr = iInput;
      return *this;
    }

    T& operator=(const T iValue)
    {
      *mDataPtr = iValue;
      return *mDataPtr;
    }

    T& operator+=(const T& iValue)
    {
      *mDataPtr += iValue;
      return *mDataPtr;
    }

    T& operator+=(const T iValue)
    {
      *mDataPtr += iValue;
      return *mDataPtr;
    }

    T& operator-=(const T& iValue)
    {
      *mDataPtr -= iValue;
      return *mDataPtr;
    }

    T& operator-=(const T iValue)
    {
      *mDataPtr -= iValue;
      return *mDataPtr;
    }

    T& operator*=(const T& iValue)
    {
      *mDataPtr *= iValue;
      return *mDataPtr;
    }

    T& operator*=(const T iValue)
    {
      *mDataPtr *= iValue;
      return *mDataPtr;
    }

    T& operator/=(const T& iValue)
    {
      *mDataPtr /= iValue;
      return *mDataPtr;
    }

    T& operator/=(const T iValue)
    {
      *mDataPtr /= iValue;
      return *mDataPtr;
    }

    T& operator%=(const T& iValue)
    {
      *mDataPtr %= iValue;
      return *mDataPtr;
    }

    T& operator%=(const T iValue)
    {
      *mDataPtr %= iValue;
      return *mDataPtr;
    }

    T& operator++()
    {
      ++(*mDataPtr);
      return *mDataPtr;
    }

    const T operator++(int)
    {
      T wVal = *mDataPtr;
      ++(*mDataPtr);
      return wVal;
    }

    T& operator--()
    {
      --(*mDataPtr);
      return *mDataPtr;
    }

    const T operator--(int)
    {
      T wVal = *mDataPtr;
      --(*mDataPtr);
      return wVal;
    }

  protected:
    T mData;
    T* mDataPtr;
  };
}


#endif