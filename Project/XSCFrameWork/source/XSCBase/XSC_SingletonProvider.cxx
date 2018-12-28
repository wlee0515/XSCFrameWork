#include "XSCBase\XSC_SingletonProvider.hxx"

namespace XSC {


  namespace Singleton
  {
    XSC_SingletonContainer<XSC_SingletonProvider>& getSingletonProviderContainer()
    {
      static XSC_SingletonContainer<XSC_SingletonProvider>* wContainer = nullptr;

      if (nullptr == wContainer)
      {
        wContainer = new XSC_SingletonContainer<XSC_SingletonProvider>();
      }

      return *wContainer;
    }


    XSC_SingletonProvider& getSingletonProvider()
    {
      return getSingletonProviderContainer().getSingletonRef();
    }

    void setSingletonProvider(XSC_SingletonProvider& iSingletonRef)
    {
      getSingletonProviderContainer().setSingletonRef(iSingletonRef);
    }
  }
}
