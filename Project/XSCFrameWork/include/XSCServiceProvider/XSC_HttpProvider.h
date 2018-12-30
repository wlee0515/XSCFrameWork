#ifndef XSC_HTTP_PROVIDER_H
#define XSC_HTTP_PROVIDER_H

#include "XSCBase/XSC_Common.hxx"
#include "XSCConnect/XSC_ConnectionProxy.h"
#include "XSCConnect/XSC_ConnectionServer.h"
#include "BClass/BClass_HttpProcessor.h"

#include <map>

namespace XSC
{
  class XSC_HttpProvider : public XSC_Object
                        , public virtual XSC_ConnectionProxy::ISubscriber
  {
  public:

    typedef XSC::BClassHttpProcessor::CRequest CHttpRequest;
    typedef XSC::BClassHttpProcessor::CResponse CHttpResponse;

    class UriService : public XSC_Object
    {
    public:

      UriService();
      UriService(const std::string& iUriAddress, const std::string& iExtension);
      virtual ~UriService();

      virtual void SClassSetup();
      virtual bool SClassStart();

      virtual bool SClassStop();
      std::string getUriAddress() const;
      std::string getExtension() const;

      virtual bool processHttpPOSTMethod(const std::string& iURI, const std::string& iRequest, std::string& oResponse) = 0;

    protected:
      XSC_String mUriAddress;
      XSC_String mReplyExtension;
    };

    XSC_HttpProvider();

    virtual void SClassSetup();
    virtual bool SClassStart();
    virtual bool SClassStop();

    bool subscribeToHttpProvider(const std::string& iUriAddress, UriService& iUriService);
    void unsubscribeFromHttpProvider(UriService& iUriService);
    void reconnectService();


    virtual bool processConnectionRequest(const std::string& iRequest, std::string& oResponse);
    virtual void processRequest(const CHttpRequest& iRequest, CHttpResponse& iSendCmd);

  protected:

    virtual void processGET(const CHttpRequest& iRequest, CHttpResponse& oResponse);
    virtual void processHEAD(const CHttpRequest& iRequest, CHttpResponse& oResponse);
    virtual void processPOST(const CHttpRequest& iRequest, CHttpResponse& oResponse);
    virtual void processPUT(const CHttpRequest& iRequest, CHttpResponse& oResponse);
    virtual void processDELETE(const CHttpRequest& iRequest, CHttpResponse& oResponse);
    virtual void processOPTIONS(const CHttpRequest& iRequest, CHttpResponse& oResponse);
    virtual void processTRACE(const CHttpRequest& iRequest, CHttpResponse& oResponse);
    virtual void processCONNECT(const CHttpRequest& iRequest, CHttpResponse& oResponse);
    virtual void processPATCH(const CHttpRequest& iRequest, CHttpResponse& oResponse);

    virtual bool sendFile(const std::string& iFilename, CHttpResponse& oResponse);
    virtual bool processWebSocketRequest(const CHttpRequest& iRequest, CHttpResponse& oResponse);

  private:

    XSC_String mServerName;
    XSC_String mPassword;
    XSC_String mHTMLRootFolder;
    XSC::XSC_ConnectionProxy           mConnectionProxy;
    std::map<std::string, UriService*> mUriServiceList;

  };
}
#endif // !XSC_HTTP_PROVIDER_H
