#ifndef XSC_ENUM_TABLE_PROVIDER_H
#define XSC_ENUM_TABLE_PROVIDER_H

#include <map>
#include <string>

namespace XSC
{
  class EnumTableProvider
  {
  public:

    class TableEntry 
    {
    public:
      template<typename TEnumType>
      TableEntry(TEnumType iEnumValue, const std::string& iStringValue)
      {
        EnumTableProvider::setGlobalEnumString(iEnumValue, iStringValue);
      }
    };

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
    static bool setGlobalEnumString(const TEnum& iEnum, const std::string& iStringValue)
    {
      return getGlobal().setEnumString<TEnum>(iEnum, iStringValue);
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

    template <typename TEnum>
    bool setEnumString(const TEnum& iEnum, const std::string& iStringValue )
    {
      std::map<TEnum, std::string>* wTable = createTable<TEnum>();
      if (nullptr != wTable)
      {
        wTable->operator[](iEnum) = iStringValue;
      }

      return true;
    }

  private:
    std::map<std::string, void*> mEnumTableList;

  };
}

#define ENUM_DEFINITION_DECLARE( Namespace, TEnumValue , TStringValue)                                      \
namespace{                                                                                                  \
    const XSC::EnumTableProvider::TableEntry TEnumValue##_EnumMapConv(Namespace::TEnumValue, TStringValue); \
}                                                                                                           \

#endif