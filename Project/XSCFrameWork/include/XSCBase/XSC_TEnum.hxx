#ifndef XSC_TENUM_HXX
#define XSC_TENUM_HXX

#include "XSC_Base.hxx"

#include <map>
#include <string>

namespace XSC {
  namespace EnumMap {
    template <typename TEnumType>
    inline const std::map<TEnumType, std::string>& getEnumConversionTable(const TEnumType&)
    {
      std::map<TEnumType, std::string>* gTable = XSC::EnumTableProvider::getGlobalTable<TEnumType>();
      if (nullptr == gTable)
      {
        gTable = EnumTableProvider::createGlobalTable<TEnumType>();
      }
      return *gTable;
    }
  }
}


#define ENUM_DEFINITION_START(TEnumType )                                                    \
namespace XSC {  namespace EnumMap {                                                         \
inline const std::map<TEnumType, std::string>& getEnumConversionTable(TEnumType&)            \
{                                                                                            \
  std::map<TEnumType, std::string>* gTable = XSC::EnumTableProvider::getGlobalTable<TEnumType>(); \
  if (nullptr == gTable)                                                                     \
  {                                                                                          \
     gTable = EnumTableProvider::createGlobalTable<TEnumType>();                             \


#define ENUM_DEFINITION_DECLARE( TEnumValue , TStringValue)                                  \
    gTable->operator[](TEnumValue) = TStringValue;                                           \


#define ENUM_DEFINITION_END                                                                  \
  }                                                                                          \
  return *gTable;                                                                            \
}}}                                                                                          \

namespace XSC
{
  class EnumTableProvider
  {
  public:

    EnumTableProvider();
    virtual ~EnumTableProvider();
    static EnumTableProvider& getGlobal();

    template <typename TEnum>
    static std::map<TEnum, std::string>* getGlobalTable()
    {
      return getGlobal().getTable<TEnum>();
    }

    template <typename TEnum>
    static std::map<TEnum, std::string>* createGlobalTable()
    {
      return getGlobal().createTable<TEnum>();
    }

    template <typename TEnum>
    std::map<TEnum, std::string>* getTable() const
    {
      std::string wClassName = typeid(TEnum).name();
      std::map<std::string, void*>::const_iterator wIt = mEnumTableList.find(wClassName);
      if (mEnumTableList.end() != wIt)
      {
        return reinterpret_cast<std::map<TEnum, std::string>*>(wIt->second);
      }
      return nullptr;
    }

    template <typename TEnum>
    std::map<TEnum, std::string>* createTable()
    {
      std::map<TEnum, std::string>* wTable = getTable<TEnum>();
      if (nullptr == wTable)
      {
        std::string wClassName = typeid(TEnum).name();
        mEnumTableList[wClassName] = wTable = new std::map<TEnum, std::string>();
      }

      return wTable;
    }

    std::map<std::string, void*> mEnumTableList;

  };

  template <typename TEnum>
  class XSC_TEnum : public XSC::XSC_Data
  {
  public:

    XSC_TEnum()
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
      TEnum wEnum;
      const std::map<TEnum, std::string> wTable = XSC::EnumMap::getEnumConversionTable(wEnum);

      std::map<TEnum, std::string>::const_iterator wIt = wTable.find(iEnumValue);

      if (wTable.end() != wIt)
      {
        oStringValue = wIt->second;
        return true;
      }

      return false;
    }

    static bool convertStringToEnum(const std::string& iStringValue, TEnum& oEnumValue)
    {
      TEnum wEnum;
      const std::map<TEnum, std::string>& wTable = XSC::EnumMap::getEnumConversionTable(wEnum);

      for (std::map<TEnum, std::string>::const_iterator wIt = wTable.begin(); wIt != wTable.end(); ++wIt)
      {
        if (wIt->second == iStringValue)
        {
          oEnumValue = wIt->first;
          return true;
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

ENUM_DEFINITION_START(XSC::XSC_Action)
ENUM_DEFINITION_DECLARE(XSC::eXSC_Setup, "Setup");
ENUM_DEFINITION_DECLARE(XSC::eXSC_Start, "Start");
ENUM_DEFINITION_DECLARE(XSC::eXSC_Stop, "Stop");
ENUM_DEFINITION_END

#endif