#ifndef XSC_TENUM_HXX
#define XSC_TENUM_HXX

#include "XSC_Base.hxx"
#include "XSC_EnumTableProvider.h"

#include <map>
#include <string>

namespace XSC
{
  template <typename TEnum>
  class XSC_TEnum : public XSC::XSC_Data
  {
  public:

    XSC_TEnum() : mData(static_cast<TEnum>(0))
    {
    }

    XSC_TEnum(TEnum iEnum) : mData(iEnum)
    {
    }

    virtual void setValue(const std::string& iValue)
    {
      TEnum wData;
      if (true == convertStringToEnum(iValue, wData))
      {
        mData = wData;
      }
      return;
    }

    virtual std::string getValue() const
    {
      std::string wString;
      if (true == convertEnumToString(mData, wString))
      {
        return wString;
      }
      return "";
    }

    static bool convertEnumToString(const TEnum& iEnumValue, std::string& oStringValue)
    {
      const std::map<TEnum, std::string>* wTable = XSC::EnumTableProvider::getGlobalTable<TEnum>();

      if (nullptr != wTable)
      {
        typename std::map<TEnum, std::string>::const_iterator wIt = wTable->find(iEnumValue);

        if (wTable->end() != wIt)
        {
          oStringValue = wIt->second;
          return true;
        }
      }

      return false;
    }

    static bool convertStringToEnum(const std::string& iStringValue, TEnum& oEnumValue)
    {
      const std::map<TEnum, std::string>* wTable = XSC::EnumTableProvider::getGlobalTable<TEnum>();

      if (nullptr != wTable)
      {
        for (typename std::map<TEnum, std::string>::const_iterator wIt = wTable->begin(); wIt != wTable->end(); ++wIt)
        {
          if (wIt->second == iStringValue)
          {
            oEnumValue = wIt->first;
            return true;
          }
        }
      }

      return false;
    }

    operator TEnum&()
    {
      return mData;
    }

    operator const TEnum&() const
    {
      return mData;
    }

  private:
    TEnum mData;
  };
}


#endif