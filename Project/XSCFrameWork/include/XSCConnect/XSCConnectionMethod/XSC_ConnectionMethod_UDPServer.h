#ifndef XSC_CONNECTION_METHOD_UDPSERVER_H
#define XSC_CONNECTION_METHOD_UDPSERVER_H

#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"
#include  <thread>

class UDPSocket;

namespace XSC {

  class XSC_ConnectionMethod_UDPServer : public XSC::XSC_ConnectionMethod
  {
  public:

    XSC_ConnectionMethod_UDPServer(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup);

    virtual eConnectionType getConnectionType() const;

    virtual void processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

    virtual void SClassSetup();

    virtual bool SClassStart();

    virtual bool SClassStop();

    void UDPServerThread();

  protected:

    UDPSocket* mSocket;
    unsigned short mPortNumber;
    std::thread mMainServerThread;
  };
}

#endif