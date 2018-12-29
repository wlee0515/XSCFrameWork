#ifndef XSC_CONNECTION_METHOD_RS232_H
#define XSC_CONNECTION_METHOD_RS232_H

#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"
#include "XSCBase/XSC_Scheduler.h"

class UDPSocket;

namespace XSC {

  class XSC_ConnectionMethod_RS232 : public XSC::XSC_ConnectionMethod
    , public virtual XSC::IEntryPoint
  {
  public:

    XSC_ConnectionMethod_RS232(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup);

    virtual eConnectionType getConnectionType() const;

    virtual void processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

    virtual void SClassSetup();

    virtual bool SClassStart();

    virtual bool SClassStop();

    virtual void entrypoint(double iDt);

  protected:


    int mCOMPortNumber;
    int mBaudRate;
    XSC::XSC_Bool mCOMPortIsOpen;
    std::vector<XSC_String> mCarriedOverMsg;
  };
}

#endif