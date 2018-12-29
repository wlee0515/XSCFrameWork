#ifndef XSC_CONNECTION_METHOD_TCPCLIENT_H
#define XSC_CONNECTION_METHOD_TCPCLIENT_H

#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"
#include  <thread>

class TCPServerSocket;
class TCPSocket;

namespace XSC {

  class XSC_ConnectionMethod_TCPClient : public XSC::XSC_ConnectionMethod
  {
  public:

    XSC_ConnectionMethod_TCPClient(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup);

    virtual eConnectionType getConnectionType() const;

    virtual void processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

    virtual void SClassSetup();

    virtual bool SClassStart();

    virtual bool SClassStop();

    void TCPClientThread(TCPSocket * iClientSocket, XSC_ConnectionProxy::ISubscriber* iClient);

  protected:

    unsigned short mPortNumber;
    std::string mServerAddress;
    std::map<TCPSocket *, std::thread> mThreadList;
  };
}

#endif