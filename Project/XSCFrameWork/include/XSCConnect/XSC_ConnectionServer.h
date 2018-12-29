#ifndef XSC_CONNECTION_SERVER_H
#define XSC_CONNECTION_SERVER_H

#include "XSCBase/XSC_Base.hxx"
#include "XSCConnect/XSC_ConnectionProxy.h"
#include "XSCConnect/XSC_Connection.h"

#include <string>

namespace XSC {

  class XSC_ConnectionServer : public XSC::XSC_TArray<XSC_Connection>
  {
  public:

    bool connectProxy(XSC_ConnectionProxy& iConnectionProxy);

  protected:

  };
}
#endif // !XSC_CONNECTION_SERVER_H
