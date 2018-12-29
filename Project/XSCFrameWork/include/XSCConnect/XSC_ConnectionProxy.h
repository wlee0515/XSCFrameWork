#ifndef XSC_CONNECTION_PROXY_H
#define XSC_CONNECTION_PROXY_H

#include "XSCBase/XSC_Common.hxx"

namespace XSC
{
  class XSC_ConnectionProxy : public XSC::XSC_Object
  {
  public:

    class ISubscriber
    {
    public:
      virtual bool processConnectionRequest(const std::string& iRequest, std::string& oResponse) = 0;
    };

    virtual void SClassSetup();

    const std::string& getConnectionKey() const;

    void connectProxy(XSC_ConnectionProxy& iConnection);

    void clearConnectedProxy();

    void subscribeToProxy(XSC_ConnectionProxy::ISubscriber& iSubscriber);

    void unsubscribeFromProxy(XSC_ConnectionProxy::ISubscriber& iSubscriber);

    bool sendClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

    bool sendConnectionRequest(const std::string& iRequest, std::vector<std::string>& oResponse);

  protected:

    virtual bool processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

    virtual bool processConnectionRequest(const std::string& iRequest, std::vector<std::string>& oResponse);

  protected:
    XSC::XSC_String mConnectionKey;
    XSC_SafeObjectPointer<XSC_ConnectionProxy, true> mConnectedProxy;
    std::set<XSC_ConnectionProxy::ISubscriber*> mSubscriberList;

  };
}

#endif // !XSC_CONNECTION_PROXY_H
