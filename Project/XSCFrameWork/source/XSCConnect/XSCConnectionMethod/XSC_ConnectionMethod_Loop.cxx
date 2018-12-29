#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_Loop.h"
#include "XSCEnum/XSC_ConnectionEnum.h"

namespace XSC {

  XSC_ConnectionMethod_Loop::XSC_ConnectionMethod_Loop(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup)
    : XSC_ConnectionMethod(iConnectionSetup)
  {
  }

  eConnectionType XSC_ConnectionMethod_Loop::getConnectionType() const
  {
    return eLoop;
  }

  void XSC_ConnectionMethod_Loop::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    std::string wResponse;
    iClient.processConnectionRequest(iRequest, wResponse);
  }
}
