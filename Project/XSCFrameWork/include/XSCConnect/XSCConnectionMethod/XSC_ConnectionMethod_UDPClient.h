#ifndef XSC_CONNECTION_METHOD_UDPCLIENT_H
#define XSC_CONNECTION_METHOD_UDPCLIENT_H

#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"
#include  <thread>

class UDPSocket;

namespace XSC {

  class XSC_ConnectionMethod_UDPClient : public XSC::XSC_ConnectionMethod
  {
  public:

    XSC_ConnectionMethod_UDPClient(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup);

    virtual eConnectionType getConnectionType() const;

    virtual void processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

    virtual void SClassSetup();

    virtual bool SClassStart();

    virtual bool SClassStop();

    void UDPClientThread(UDPSocket * iClientSocket, XSC_ConnectionProxy::ISubscriber* iClient);

  protected:

    unsigned short mPortNumber;
    std::string mServerAddress;
    std::map<UDPSocket *, std::thread> mThreadList;
  };
}

#endif