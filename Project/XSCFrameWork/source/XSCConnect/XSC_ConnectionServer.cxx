#include "XSCConnect/XSC_ConnectionServer.h"
#include "XSCConnect/XSC_ConnectionProxy.h"
#include "XSCConnect/XSC_Connection.h"

#include <string>
#include "XSCBase/XSC_TypeLibrary.h"

namespace XSC {

  ADD_TYPE_FACTORY_WITH_NAME(XSC_ConnectionServer, "ConnectionServer");

  bool XSC_ConnectionServer::connectProxy(XSC_ConnectionProxy& iConnectionProxy)
  {
    for (XSC::XSC_TArray<XSC_Connection>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
    {
      if (iConnectionProxy.getConnectionKey() == wIt->getConnectionKey())
      {
        iConnectionProxy.connectProxy(*wIt);
        return true;
      }
    }
    return false;
  }
}
