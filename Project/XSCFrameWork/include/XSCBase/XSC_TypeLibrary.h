#ifndef XSC_TYPE_LIBRARY_H
#define XSC_TYPE_LIBRARY_H

#include "XSCBase/XSC_Base.hxx"
#include "XSCUtility/XSC_Logger.h"
#include "XSCUtility/XSC_Typename.h"

namespace XSC {

  class XSC_TypeLibrary : public XSC_Object
  {
  public:

    typedef XSC_Void* (*typeFactory_Function_Ptr)();
    template<typename T>
    static XSC_Void* typeFactory()
    {
      return new T();
    }

    template<typename T>
    bool addTypeToLibrary(std::string iTypeName, typeFactory_Function_Ptr iTypeFactory)
    {
      typeFactory_Function_Ptr wFactory = &XSC::XSC_TypeLibrary::typeFactory<T>;
      return addTypeToLibrary(iTypeName, wFactory);
    }

    bool addTypeToLibrary(std::string iTypeName, typeFactory_Function_Ptr iTypeFactory)
    {
      std::map<std::string, typeFactory_Function_Ptr>::const_iterator wTypeIt = mTypeLibrary.find(iTypeName);

      if (mTypeLibrary.end() != wTypeIt)
      {
        LOG_ERROR("Type name \"" << iTypeName << "\" already defined in type Library");
        return false;
      }

      mTypeLibrary[iTypeName] = iTypeFactory;
      return true;
    }

    XSC::XSC_Void* createObject(std::string iTypeName) const
    {
      std::map<std::string, typeFactory_Function_Ptr>::const_iterator wTypeIt = mTypeLibrary.find(iTypeName);

      if (mTypeLibrary.end() != wTypeIt)
      {
        return wTypeIt->second();
      }

      LOG_ERROR("Unable to find Type name \"" << iTypeName << "\" ");

      return nullptr;
    }

    template<typename T>
    std::string getTypeName() const
    {
      typeFactory_Function_Ptr wFactory = &XSC::XSC_TypeLibrary::typeFactory<T>;

      for (std::map<std::string, typeFactory_Function_Ptr>::const_iterator wIt = mTypeLibrary.begin(); wIt != mTypeLibrary.end(); ++wIt)
      {
        if (wIt->second == wFactory)
        {
          return wIt->first;
        }
      }

      LOG_ERROR("Unable to find Type name of \"" << XSC::tools::getTypeName<T>() << "\" ");

      return "";
    }

    static bool AddTypeToLibrary(std::string iTypeName, typeFactory_Function_Ptr iTypeFactory)
    {
      return  getGlobalTypeLibrary().addTypeToLibrary(iTypeName, iTypeFactory);
    }

    template<typename T>
    static bool AddTypeToLibrary(typeFactory_Function_Ptr iTypeFactory)
    {
      return  AddTypeToLibrary(XSC::tools::getTypeName<T>(), iTypeFactory);
    }

    template<typename T>
    static bool AddTypeToLibrary(std::string iTypeName)
    {
      return  getGlobalTypeLibrary().addTypeToLibrary<T>(iTypeName);
    }

    static XSC::XSC_Void* CreateObject(std::string iTypeName)
    {
      return  getGlobalTypeLibrary().createObject(iTypeName);
    }

    template<typename T>
    static std::string GetTypeName()
    {
      return  getGlobalTypeLibrary().getTypeName<T>();
    }

    static XSC_TypeLibrary& getGlobalTypeLibrary()
    {
      static XSC_TypeLibrary* wGlobal = nullptr;

      if (nullptr == wGlobal)
      {
        wGlobal = new XSC_TypeLibrary();
        wGlobal->SClassSetup();
      }

      return *wGlobal;
    }
  private:
    std::map<std::string, typeFactory_Function_Ptr> mTypeLibrary;
  };

}

#define ADD_TYPE_FACTORY(TYPE)                  \
namespace{                                      \
  bool ATTL_DECLARATION_##TYPE = XSC::XSC_TypeLibrary::AddTypeToLibrary( #TYPE, &XSC::XSC_TypeLibrary::typeFactory<TYPE>); \
}                                               \


#define ADD_TYPE_FACTORY_WITH_NAME(TYPE, NameStr)                  \
namespace {                                                        \
  bool ATTL_DECLARATION_##TYPE = XSC::XSC_TypeLibrary::AddTypeToLibrary(NameStr, &XSC::XSC_TypeLibrary::typeFactory<TYPE>);\
}                                                                  \


#endif