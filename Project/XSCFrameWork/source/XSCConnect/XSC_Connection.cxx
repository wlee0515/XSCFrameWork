#include "XSCConnect/XSC_Connection.h"

#include "XSCEnum/XSC_ConnectionEnum.h"
#include "XSCConnect/XSC_ConnectionProxy.h"

#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"
#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_Loop.h"
#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_RS232.h"
#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_TCPClient.h"
#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_TCPServer.h"
#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_UDPClient.h"
#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_UDPServer.h"

#include <string>

namespace XSC {

  XSC_Connection::XSC_Connection() : mConnectionType(eNoConnection)
  {

  }

  void XSC_Connection::SClassSetup()
  {
    addChild("ConnectionType", mConnectionType);
    addChild("ConnectionSetup", mConnectionSetup);
    addChild("ConnectionMethod", mConnectionMethod);

    XSC_ConnectionProxy::SClassSetup();
  }

  bool XSC_Connection::SClassStart()
  {
    refreshConnection();
    return XSC_ConnectionProxy::SClassStart();
  }

  bool XSC_Connection::SClassStop()
  {
    deleteConnection();
    return XSC_ConnectionProxy::SClassStop();
  }

  void XSC_Connection::deleteConnection()
  {
    XSC_ConnectionMethod* wMethod = mConnectionMethod.getPtr();
    if (nullptr != wMethod)
    {
      wMethod->SClassStop();

      delete wMethod;
      wMethod = nullptr;
    }
  }

  void XSC_Connection::refreshConnection()
  {
    deleteConnection();
    XSC_ConnectionMethod* wMethod = nullptr;
    switch (mConnectionType)
    {
    case eLoop:
      wMethod = new XSC_ConnectionMethod_Loop(mConnectionSetup);
      break;
    case eRS232:
      wMethod = new XSC_ConnectionMethod_RS232(mConnectionSetup);
      break;
    case eTCPClient:
      wMethod = new XSC_ConnectionMethod_TCPClient(mConnectionSetup);
      break;
    case eTCPServer:
      wMethod = new XSC_ConnectionMethod_TCPServer(mConnectionSetup);
      break;
    case eUDPClient:
      wMethod = new XSC_ConnectionMethod_UDPClient(mConnectionSetup);
      break;
    case eUDPServer:
      wMethod = new XSC_ConnectionMethod_UDPServer(mConnectionSetup);
      break;
    case eNoConnection:
    default:
      wMethod = new XSC_ConnectionMethod(mConnectionSetup);
      break;
    }

    if (nullptr != wMethod)
    {
      wMethod->SClassSetup();
      wMethod->SClassStart();
      wMethod->setConnectionProxy(*this);
      mConnectionMethod.setPtr(*wMethod);
    }
  }

  XSC_ConnectionMethod& XSC_Connection::getConnectionMethod()
  {
    bool wMakeInstance = false;
    if (nullptr == mConnectionMethod.getPtr())
    {
      wMakeInstance = true;
    }
    else if (mConnectionType != mConnectionMethod.getPtr()->getConnectionType())
    {
      wMakeInstance = true;
    }

    if (true == wMakeInstance)
    {
      refreshConnection();
    }

    return *mConnectionMethod.getPtr();
  }

  bool XSC_Connection::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    XSC_ConnectionMethod& wConnectionMethod = getConnectionMethod();
    wConnectionMethod.processClientRequest(iClient, iRequest);
    return true;
  }

  bool XSC_Connection::processConnectionRequest(const std::string& iRequest, std::vector<std::string>& oResponse)
  {
    return false;
  }
}
