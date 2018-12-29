#ifndef XSC_CONNECTION_H
#define XSC_CONNECTION_H

#include "XSCConnect/XSC_ConnectionProxy.h"
#include "XSCEnum/XSC_ConnectionEnum.h"
#include "XSCBase/XSC_SafePointer.hxx"
#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod.h"
#include <string>

namespace XSC {

  class XSC_Connection : public XSC::XSC_ConnectionProxy
  {
  public:

    XSC_Connection();

    virtual void SClassSetup();

    virtual bool SClassStart();

    virtual bool SClassStop();

    void deleteConnection();

    void refreshConnection();

    XSC_ConnectionMethod& getConnectionMethod();

  protected:

    virtual bool processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest);
    virtual bool processConnectionRequest(const std::string& iRequest, std::vector<std::string>& oResponse);

  protected:
    XSC_TEnum<XSC::eConnectionType> mConnectionType; 
    XSC_ConnectionMethod::XSC_ConnectionSetup mConnectionSetup;
    XSC_SafeObjectPointer<XSC_ConnectionMethod, true> mConnectionMethod;
  };

}
#endif // !XSC_CONNECTION_H
