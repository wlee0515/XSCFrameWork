#ifndef XSC_CONNECTION_METHOD_TCPSERVER_H
#define XSC_CONNECTION_METHOD_TCPSERVER_H

#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"
#include  <thread>

class TCPServerSocket;
class TCPSocket;

namespace XSC {

  class XSC_ConnectionMethod_TCPServer : public XSC::XSC_ConnectionMethod
  {
  public:

    XSC_ConnectionMethod_TCPServer(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup);

    virtual eConnectionType getConnectionType() const;

    virtual void processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

    virtual void SClassSetup();

    virtual bool SClassStart();

    virtual bool SClassStop();

    void TCPServerThread();
    void TCPClientThread(TCPSocket * iClientSocket);

  protected:

    TCPServerSocket* mSocket;
    unsigned short mPortNumber;
    std::thread mMainServerThread;
    bool mMainThreadRun;
  };
}

#endif