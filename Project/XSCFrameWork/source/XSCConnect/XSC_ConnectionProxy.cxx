#include "XSCConnect/XSC_ConnectionProxy.h"

namespace XSC
{
  void XSC_ConnectionProxy::SClassSetup()
  {
    addChild("ConnectionKey", mConnectionKey);

    XSC_Object::SClassSetup();
  }

  const std::string& XSC_ConnectionProxy::getConnectionKey() const
  {
    return mConnectionKey.getData();
  }

  void XSC_ConnectionProxy::connectProxy(XSC_ConnectionProxy& iConnection)
  {
    clearConnectedProxy();

    mConnectedProxy.setPtr(iConnection);
    iConnection.mConnectedProxy.setPtr(*this);
  }

  void XSC_ConnectionProxy::clearConnectedProxy()
  {
    XSC_ConnectionProxy* wPtr = mConnectedProxy.getPtr();

    if (nullptr != wPtr)
    {
      if (this == wPtr->mConnectedProxy.getPtr())
      {
        wPtr->mConnectedProxy.setNull();
      }

      mConnectedProxy.setNull();
    }
  }

  void XSC_ConnectionProxy::subscribeToProxy(XSC_ConnectionProxy::ISubscriber& iSubscriber)
  {
    mSubscriberList.insert(&iSubscriber);
  }

  void XSC_ConnectionProxy::unsubscribeFromProxy(XSC_ConnectionProxy::ISubscriber& iSubscriber)
  {
    mSubscriberList.erase(&iSubscriber);
  }

  bool XSC_ConnectionProxy::sendClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    XSC_ConnectionProxy* wPtr = mConnectedProxy.getPtr();

    if (nullptr != wPtr)
    {
      return wPtr->processClientRequest(iClient, iRequest);
    }

    return false;
  }

  bool XSC_ConnectionProxy::sendConnectionRequest(const std::string& iRequest, std::vector<std::string>& oResponse)
  {
    XSC_ConnectionProxy* wPtr = mConnectedProxy.getPtr();

    if (nullptr != wPtr)
    {
      return wPtr->processConnectionRequest(iRequest, oResponse);
    }

    return false;
  }

  bool XSC_ConnectionProxy::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    return false;
  }

  bool XSC_ConnectionProxy::processConnectionRequest(const std::string& iRequest, std::vector<std::string>& oResponse)
  {
    oResponse.clear();
    oResponse.reserve(mSubscriberList.size());
    bool wReturn = true;
    for (std::set<XSC_ConnectionProxy::ISubscriber*>::iterator wIt = mSubscriberList.begin(); wIt != mSubscriberList.end(); ++wIt)
    {
      if (nullptr != *wIt)
      {
        std::string wReponse("");
        if (false == (*wIt)->processConnectionRequest(iRequest, wReponse))
        {
          wReturn = false;
        }

        if (0 != wReponse.length())
        {
          oResponse.push_back(wReponse);
        }
      }
    }

    return wReturn;
  }
}
