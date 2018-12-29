#ifndef XSC_CONNECTION_METHOD_H
#define XSC_CONNECTION_METHOD_H

#include "XSCBase/XSC_Common.hxx"
#include "XSCConnect/XSC_ConnectionProxy.h"
#include "XSCEnum/XSC_ConnectionEnum.h"

namespace XSC {

  class XSC_ConnectionMethod : public XSC::XSC_Object
  {
  public:

    class XSC_ConnectionSetup : public XSC::XSC_Object
    {
    public:
      XSC_ConnectionSetup();
      virtual void SClassSetup();

      int getComNumber() const;
      int getBaudRate() const;
      std::string getIPAddress() const;
      unsigned short getPortNumber() const;

    protected:

      XSC::XSC_Int32 mComNumber;
      XSC::XSC_Int32 mBaudRate;
      XSC::XSC_String mIPAddress;
      XSC::XSC_UInt16 mPortNumber;
    };

    XSC_ConnectionMethod(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup);
    virtual eConnectionType getConnectionType() const;
    void setConnectionProxy(XSC_ConnectionProxy& iConnectionProxy);
    virtual void processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

  protected:

    XSC_SafeObjectPointer<XSC_ConnectionProxy, true> mConnectedProxy;

  };
}

#endif