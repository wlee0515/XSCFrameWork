#ifndef XSC_SINGLETON_PROVIDER_HXX
#define XSC_SINGLETON_PROVIDER_HXX

#include "XSCBase/XSC_Base.hxx"
#include "XSCBase/XSC_SafePointer.hxx"
#include "XSCUtility/XSC_Typename.h"
#include "XSCBase/XSC_TypeLibrary.h"

namespace XSC {

  template <typename T_Singleton>
  class XSC_SingletonContainer : public XSC_SafeObjectPointer<T_Singleton, false>
  {
  public:

    T_Singleton& getSingletonRef()
    {
      if (nullptr == XSC_SafeObjectPointer<T_Singleton, false>::getPtr())
      {
        T_Singleton* wSingleton = new T_Singleton();
        wSingleton->SClassSetup();
        mSelfInstantiate = true;
        XSC_SafeObjectPointer<T_Singleton, false>::setPtr(*wSingleton);
      }
      return *XSC_SafeObjectPointer<T_Singleton, false>::getPtr();
    }

    void setSingletonRef(T_Singleton& iSingleton)
    {
      if (true == mSelfInstantiate)
      {
        if (nullptr != XSC_SafeObjectPointer<T_Singleton, false>::getPtr())
        {
          delete XSC_SafeObjectPointer<T_Singleton, false>::getPtr();
          mSelfInstantiate = false;
        }
      }

      XSC_SafeObjectPointer<T_Singleton, false>::setPtr(iSingleton);
    }

    static XSC_SingletonContainer<T_Singleton>* createContainer()
    {
      XSC_SingletonContainer<T_Singleton>* wSingletonContainer = new XSC_SingletonContainer<T_Singleton>();
      wSingletonContainer->SClassSetup();
      return wSingletonContainer;
    }

  private:
    mutable bool mSelfInstantiate;
  };

  class XSC_SingletonProvider : public XSC::XSC_Object
  {
  public:

    template <typename T_Singleton>
    T_Singleton& getSingleton()
    {
      return getSingletonContainer<T_Singleton>().getSingletonRef();
    }

    template <typename T_Singleton>
    void setSingleton(T_Singleton& iSingletonRef)
    {
       getSingletonContainer<T_Singleton>().setSingletonRef(iSingletonRef);
    }

  private:

    template <typename T_Singleton>
    XSC_SingletonContainer<T_Singleton>& getSingletonContainer()
    {
      std::map<void*, void*>::iterator wSingletonPair = mSingletonContainerList.find(static_cast<void*>(XSC_SingletonContainer<T_Singleton>::createContainer));

      if (wSingletonPair != mSingletonContainerList.end())
      {
        return *(reinterpret_cast<XSC_SingletonContainer<T_Singleton>*>(wSingletonPair->second));
      }

      XSC_SingletonContainer<T_Singleton>* wNewContainer = XSC_SingletonContainer<T_Singleton>::createContainer();
      mSingletonContainerList[static_cast<void*>(&XSC_SingletonContainer<T_Singleton>::createContainer)] = static_cast<void*>(wNewContainer);

      std::string wName;
      wName = XSC::XSC_TypeLibrary::GetTypeName<T_Singleton>();

      if ("" == wName)
      {
        std::stringstream wContainerName;
        wContainerName << "SingletonContainer_";
        wContainerName << mSingletonContainerList.size();
        wName = wContainerName.str();
      }

      addChild(wName, *wNewContainer);
      return *wNewContainer;
    }

    std::map<void*, void*> mSingletonContainerList;
  };

  namespace Singleton
  {
    XSC_SingletonContainer<XSC_SingletonProvider>& getSingletonProviderContainer();
    XSC_SingletonProvider& getSingletonProvider();
    void setSingletonProvider(XSC_SingletonProvider& iSingletonRef);

    template <typename T_Singleton>
    T_Singleton& getSingleton()
    {
      return  getSingletonProvider().getSingleton<T_Singleton>();
    }

    template <typename T_Singleton>
    void setSingleton(T_Singleton& iSingletonRef)
    {
      getSingletonProvider().setSingleton(iSingletonRef);
    }
  }
}

#endif