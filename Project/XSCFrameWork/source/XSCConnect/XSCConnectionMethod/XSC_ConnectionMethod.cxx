#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"
#include "XSCEnum/XSC_ConnectionEnum.h"
#include "XSCConnect/XSC_ConnectionProxy.h"


namespace XSC {

  XSC_ConnectionMethod::XSC_ConnectionSetup::XSC_ConnectionSetup()
    : mComNumber(-1)
    , mBaudRate(-1)
    , mIPAddress("")
    , mPortNumber(0)
  {
  }

  void XSC_ConnectionMethod::XSC_ConnectionSetup::SClassSetup()
  {
    addChild("COM_PORT", mComNumber);
    addChild("BaudRate", mBaudRate);
    addChild("IPAddress", mIPAddress);
    addChild("PortNumber", mPortNumber);

    XSC_Object::SClassSetup();
  }

  int XSC_ConnectionMethod::XSC_ConnectionSetup::getComNumber() const
  {
    return mComNumber;
  }

  int XSC_ConnectionMethod::XSC_ConnectionSetup::getBaudRate() const
  {
    return mBaudRate;
  }

  std::string XSC_ConnectionMethod::XSC_ConnectionSetup::getIPAddress() const
  {
    return mIPAddress;
  }

  unsigned short XSC_ConnectionMethod::XSC_ConnectionSetup::getPortNumber() const
  {
    return mPortNumber;
  }

  XSC_ConnectionMethod::XSC_ConnectionMethod(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup)
  {
  }

  eConnectionType XSC_ConnectionMethod::getConnectionType() const
  {
    return eNoConnection;
  }

  void XSC_ConnectionMethod::setConnectionProxy(XSC_ConnectionProxy& iConnectionProxy)
  {
    mConnectedProxy.setPtr(iConnectionProxy);
  }

  void XSC_ConnectionMethod::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {

  }
}
