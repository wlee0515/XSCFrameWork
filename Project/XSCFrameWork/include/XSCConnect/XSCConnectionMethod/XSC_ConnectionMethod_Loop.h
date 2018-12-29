#ifndef XSC_CONNECTION_METHOD_LOOP_H
#define XSC_CONNECTION_METHOD_LOOP_H

#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"

namespace XSC {

  class XSC_ConnectionMethod_Loop : public XSC::XSC_ConnectionMethod
  {
  public:

    XSC_ConnectionMethod_Loop(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup);
    virtual eConnectionType getConnectionType() const;
    virtual void processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);

  protected:

  };
}

#endif