#include "XSCServiceProvider/XSC_HttpProvider.h"
#include <map>

namespace XSC
{
  ADD_TYPE_FACTORY_WITH_NAME(XSC_HttpProvider, "HttpProvider");

  XSC_HttpProvider::UriService::UriService() : mUriAddress("Server"), mReplyExtension(".txt")
  {
  }

  XSC_HttpProvider::UriService::UriService(const std::string& iUriAddress, const std::string& iExtension) : mUriAddress(iUriAddress), mReplyExtension(iExtension)
  {

  }

  XSC_HttpProvider::UriService::~UriService()
  {
    XSC_HttpProvider& wHttpProvider = XSC::Singleton::getSingleton<XSC::XSC_HttpProvider>();
    wHttpProvider.unsubscribeFromHttpProvider(*this);
  }

  void XSC_HttpProvider::UriService::SClassSetup()
  {
    XSC_Object::SClassSetup();

    addChild("UriAddress", mUriAddress);
    addChild("Extension", mReplyExtension);
  }

  bool XSC_HttpProvider::UriService::SClassStart()
  {
    bool wState = XSC_Object::SClassStart();

    XSC_HttpProvider& wHttpProvider = XSC::Singleton::getSingleton<XSC::XSC_HttpProvider>();
    wHttpProvider.subscribeToHttpProvider(mUriAddress, *this);
    return wState;
  }

  bool XSC_HttpProvider::UriService::SClassStop()
  {
    XSC_HttpProvider& wHttpProvider = XSC::Singleton::getSingleton<XSC::XSC_HttpProvider>();
    wHttpProvider.unsubscribeFromHttpProvider(*this);

    return XSC_Object::SClassStop();
  }

  std::string XSC_HttpProvider::UriService::getUriAddress() const
  {
    return mUriAddress;
  }

  std::string XSC_HttpProvider::UriService::getExtension() const
  {
    return mReplyExtension;
  }


  XSC_HttpProvider::XSC_HttpProvider()
    : mServerName("XSC_HTML_PROVIDER")
    , mPassword("vincent")
  {

  }

  void XSC_HttpProvider::SClassSetup()
  {
    addChild("ServerName", mServerName);
    addChild("Password", mPassword);
    addChild("HtmlRootFolder", mHTMLRootFolder);
    addChild("ConnectionProxy", mConnectionProxy);
    XSC_Object::SClassSetup();
  }

  bool XSC_HttpProvider::SClassStart()
  {
    bool wState = XSC_Object::SClassStart();

    reconnectService();

    mConnectionProxy.subscribeToProxy(*this);
    return wState;
  }

  bool XSC_HttpProvider::SClassStop()
  {
    mConnectionProxy.unsubscribeFromProxy(*this);
    return XSC_Object::SClassStop();
  }

  bool XSC_HttpProvider::subscribeToHttpProvider(const std::string& iUriAddress, UriService& iUriService)
  {
    std::string wUri = "/" + iUriAddress;
    std::map<std::string, UriService*>::iterator wIt = mUriServiceList.find(wUri);
    if (wIt != mUriServiceList.end())
    {
      if (&iUriService != wIt->second)
      {
        LOG_ERROR("Uri Address [" << iUriAddress << "] already in use");
        return false;
      }
      else
      {
        return true;
      }
    }
    mUriServiceList[wUri] = &iUriService;
    return true;
  }

  void XSC_HttpProvider::unsubscribeFromHttpProvider(UriService& iUriService)
  {
    for (std::map<std::string, UriService*>::iterator wIt = mUriServiceList.begin(); wIt != mUriServiceList.end(); )
    {
      if (wIt->second == &iUriService)
      {
        wIt = mUriServiceList.erase(wIt);
      }
      else
      {
        ++wIt;
      }
    }
  }

  void XSC_HttpProvider::reconnectService()
  {
    mConnectionProxy.clearConnectedProxy();
    XSC_ConnectionServer& wConnectionServer = XSC::Singleton::getSingleton<XSC::XSC_ConnectionServer>();
    wConnectionServer.connectProxy(mConnectionProxy);
  }


  bool XSC_HttpProvider::processConnectionRequest(const std::string& iRequest, std::string& oResponse)
  {
    if (0 != iRequest.size())
    {
      CHttpRequest wRequestForm;
      if (true == XSC::BClassHttpProcessor::generateRequestForm(iRequest, wRequestForm))
      {
        if (std::string::npos == wRequestForm.mRequestURL.find(".."))
        {
          CHttpResponse wResponse;
          processRequest(wRequestForm, wResponse);

          oResponse.clear();
          if (false == XSC::BClassHttpProcessor::generateReponse(wResponse, oResponse))
          {
            oResponse.clear();
          }
        }
      }
    }

    return true;
  }


  void XSC_HttpProvider::processRequest(const CHttpRequest& iRequest, CHttpResponse& iSendCmd)
  {

    iSendCmd.mHTTPVersion = iRequest.mHTTPVersion;
    iSendCmd.mHeaders["Server"] = mServerName;
    iSendCmd.mResponseCode = -1;

    switch (iRequest.mMethod)
    {
    case eGET:
      processGET(iRequest, iSendCmd);
      break;
    case eHEAD:
      processHEAD(iRequest, iSendCmd);
      break;
    case ePOST:
      processPOST(iRequest, iSendCmd);
      break;
    case ePUT:
      processPUT(iRequest, iSendCmd);
      break;
    case eDELETE:
      processDELETE(iRequest, iSendCmd);
      break;
    case eTRACE:
      processTRACE(iRequest, iSendCmd);
      break;
    case eOPTIONS:
      processOPTIONS(iRequest, iSendCmd);
      break;
    case eCONNECT:
      processCONNECT(iRequest, iSendCmd);
      break;
    case ePATCH:
      processPATCH(iRequest, iSendCmd);
      break;
    default:
      break;
    }

    iSendCmd.mHeaders["Connection"] = "close";
  }
  void XSC_HttpProvider::processGET(const CHttpRequest& iRequest, CHttpResponse& oResponse)
  {
    if ("/" == iRequest.mRequestURL)
    {
      std::unordered_map<std::string, std::string>::const_iterator wIt;

      wIt = iRequest.mHeaders.find("Connection");

      if (wIt != iRequest.mHeaders.end())
      {
        if ("upgrade" == wIt->second)
        {
          processWebSocketRequest(iRequest, oResponse);
        }
        else
        {
          std::string wResourcePath = mHTMLRootFolder;
          wResourcePath += "/index.html";
          sendFile(wResourcePath, oResponse);
        }
      }
    }
    else if (0 < iRequest.mRequestURL.length())
    {
      unsigned int wExtensionStart = static_cast<unsigned int>(iRequest.mRequestURL.find_last_of('.'));
      if (wExtensionStart != iRequest.mRequestURL.npos)
      {
        if (5 >= (static_cast<int>(iRequest.mRequestURL.length()) - static_cast<int>(wExtensionStart)))
        {
          std::string wResourcePath = mHTMLRootFolder;
          wResourcePath += iRequest.mRequestURL;
          sendFile(wResourcePath, oResponse);
        }
      }
    }
  }

  void XSC_HttpProvider::processHEAD(const CHttpRequest& iRequest, CHttpResponse& oResponse)
  {
    processGET(iRequest, oResponse);
    oResponse.mContent.clear();
  }

  void XSC_HttpProvider::processPOST(const CHttpRequest& iRequest, CHttpResponse& oResponse)
  {
    std::map<std::string, UriService*>::iterator wIt = mUriServiceList.find(iRequest.mRequestURL);

    if (wIt != mUriServiceList.end())
    {
      if (NULL != wIt->second)
      {
        oResponse.mResponseCode = 200;
        std::stringstream wNumber;
        if (false == wIt->second->processHttpPOSTMethod(iRequest.mRequestURL, iRequest.mContent, oResponse.mContent))
        {
          oResponse.mContent.clear();
          oResponse.mResponseCode = 400;
        }

        wNumber << oResponse.mContent.size();
        oResponse.mHeaders["Content-Length"] = wNumber.str();
        oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(wIt->second->getExtension());
        oResponse.mHeaders["Connection"] = "close";
      }
    }
    else
    {
      oResponse.mResponseCode = 404;
      oResponse.mContent = "<html><body><h1>URI Not Found</h1></body></html>";
      std::stringstream wNumber;
      wNumber << oResponse.mContent.size();
      oResponse.mHeaders["Content-Length"] = wNumber.str();
      oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(".html");
      oResponse.mHeaders["Connection"] = "close";
    }
  }

  void XSC_HttpProvider::processPUT(const CHttpRequest& iRequest, CHttpResponse& oResponse)
  {
    if (("/" != iRequest.mRequestURL) && (0 < iRequest.mRequestURL.length()))
    {
      unsigned int wExtensionStart = static_cast<unsigned int>(iRequest.mRequestURL.find_last_of('.'));
      if (wExtensionStart != iRequest.mRequestURL.npos)
      {
        if (5 >= (static_cast<int>(iRequest.mRequestURL.length()) - static_cast<int>(wExtensionStart)))
        {
          std::unordered_map<std::string, std::string>::const_iterator wIt;
          wIt = iRequest.mHeaders.find("Password");
          if (wIt != iRequest.mHeaders.end())
          {
            if (wIt->second == mPassword)
            {
              std::string wResourcePath = mHTMLRootFolder;
              wResourcePath += iRequest.mRequestURL;

              std::ifstream wFile(wResourcePath);
              if (wFile.good())
              {
                wFile.close();

                oResponse.mResponseCode = 409;
                oResponse.mContent = "<html><body><h1>URL Already Exist</h1></body></html>";
                std::stringstream wNumber;
                wNumber << oResponse.mContent.size();
                oResponse.mHeaders["Content-Length"] = wNumber.str();
                oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(".html");
                oResponse.mHeaders["Connection"] = "close";

              }
              else
              {
                wFile.close();

                std::ofstream wFileOut(wResourcePath);
                for (unsigned int wi = 0; wi < iRequest.mContent.size(); ++wi)
                {
                  wFileOut << static_cast<char>(iRequest.mContent[wi]);
                }

                wFileOut.close();

                oResponse.mResponseCode = 200;
                oResponse.mContent = "<html><body><h1>URL Created.</h1></body></html>";
                std::stringstream wNumber;
                wNumber << oResponse.mContent.size();
                oResponse.mHeaders["Content-Length"] = wNumber.str();
                oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(".html");
                oResponse.mHeaders["Connection"] = "close";
                return;
              }
            }
          }
        }
      }
    }
    oResponse.mResponseCode = 401;
    oResponse.mContent = "<html><body><h1>Access Denied</h1></body></html>";
    std::stringstream wNumber;
    wNumber << oResponse.mContent.size();
    oResponse.mHeaders["Content-Length"] = wNumber.str();
    oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(".html");
    oResponse.mHeaders["Connection"] = "close";

    return;
  }

  void XSC_HttpProvider::processDELETE(const CHttpRequest& iRequest, CHttpResponse& oResponse)

  {
    if (("/" != iRequest.mRequestURL) && (0 < iRequest.mRequestURL.length()))
    {
      unsigned int wExtensionStart = static_cast<unsigned int>(iRequest.mRequestURL.find_last_of('.'));
      if (wExtensionStart != iRequest.mRequestURL.npos)
      {
        if (5 >= (static_cast<int>(iRequest.mRequestURL.length()) - static_cast<int>(wExtensionStart)))
        {
          std::unordered_map<std::string, std::string>::const_iterator wIt;
          wIt = iRequest.mHeaders.find("Password");
          if (wIt != iRequest.mHeaders.end())
          {
            if (wIt->second == mPassword)
            {
              std::string wResourcePath = mHTMLRootFolder;
              wResourcePath += iRequest.mRequestURL;

              if (remove(wResourcePath.c_str()) == 0)
              {
                oResponse.mResponseCode = 200;
                oResponse.mContent = "<html><body><h1>URL deleted.</h1></body></html>";
                std::stringstream wNumber;
                wNumber << oResponse.mContent.size();
                oResponse.mHeaders["Content-Length"] = wNumber.str();
                oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(".html");
                oResponse.mHeaders["Connection"] = "close";

                return;
              }
              else
              {
                oResponse.mResponseCode = 404;
                oResponse.mContent = "<html><body><h1>Unable to delete URL</h1></body></html>";
                std::stringstream wNumber;
                wNumber << oResponse.mContent.size();
                oResponse.mHeaders["Content-Length"] = wNumber.str();
                oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(".html");
                oResponse.mHeaders["Connection"] = "close";
                return;
              }
            }
          }
        }
      }
    }
    oResponse.mResponseCode = 401;
    oResponse.mContent = "<html><body><h1>Access Denied</h1></body></html>";
    std::stringstream wNumber;
    wNumber << oResponse.mContent.size();
    oResponse.mHeaders["Content-Length"] = wNumber.str();
    oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(".html");
    oResponse.mHeaders["Connection"] = "close";

    return;
  }

  void XSC_HttpProvider::processOPTIONS(const CHttpRequest& iRequest, CHttpResponse& oResponse)

  {
    /*
    std::set<eHttpMethodEnum> wFullSet;
    wFullSet.insert(eGET);
    wFullSet.insert(eHEAD);
    wFullSet.insert(ePOST);
    wFullSet.insert(ePUT);
    wFullSet.insert(eDELETE);
    wFullSet.insert(eOPTIONS);
    wFullSet.insert(eTRACE);
    wFullSet.insert(eCONNECT);
    wFullSet.insert(ePATCH);

    SClassHttpMethodEnum::Set::const_iterator wIt = mSuppressedMethodList.begin();
    for (wIt = mSuppressedMethodList.begin(); wIt != mSuppressedMethodList.end(); ++wIt)
    {
      wFullSet.erase(*wIt);
    }

    std::string wSupportedMethods;
    std::set<eHttpMethodEnum>::const_iterator wIt2 = wFullSet.begin();
    for (wIt2 = wFullSet.begin(); wIt2 != wFullSet.end(); ++wIt2)
    {
      if ("" != wSupportedMethods)
      {
        wSupportedMethods += ",";
      }
      wSupportedMethods += SClassHttpMethodEnum::getEnumStringValue(*wIt2);
    }

    oResponse.mResponseCode = 200;
    oResponse.mHeaders["Allow"] = wSupportedMethods;
    oResponse.mHeaders["Content-Type"] = "httpd/unix-directory";
    oResponse.mHeaders["Connection"] = "close";

    */
  }

  void XSC_HttpProvider::processTRACE(const CHttpRequest& iRequest, CHttpResponse& oResponse)
  {
    oResponse.mResponseCode = 200;

    oResponse.mContent = iRequest.mRequestLine;

    std::unordered_map<std::string, std::string>::const_iterator wIt;

    for (wIt = iRequest.mHeaders.begin(); wIt != iRequest.mHeaders.end(); ++wIt)
    {
      oResponse.mContent += "\r\n ";
      oResponse.mContent += wIt->first;
      oResponse.mContent += ": ";
      oResponse.mContent += wIt->second;
    }

    if (0 != oResponse.mContent.size())
    {
      oResponse.mContent += "\r\n\r\n";
      oResponse.mContent += oResponse.mContent;
    }
    std::stringstream wNumber;
    wNumber << oResponse.mContent.size();
    oResponse.mHeaders["Content-Length"] = wNumber.str();
    oResponse.mHeaders["Content-Type"] = "message/http";
    oResponse.mHeaders["Connection"] = "close";
  }

  void XSC_HttpProvider::processCONNECT(const CHttpRequest& iRequest, CHttpResponse& oResponse)
  {

  }

  void XSC_HttpProvider::processPATCH(const CHttpRequest& iRequest, CHttpResponse& oResponse)
  {

  }

  bool XSC_HttpProvider::sendFile(const std::string& iFilename, CHttpResponse& oResponse)
  {
    LOG_TRACE("Sending File [" << iFilename << "]");
    std::fstream wFile;
    wFile.open(iFilename, std::ofstream::in | std::ofstream::binary);
    if (true == wFile.good())
    {
      oResponse.mResponseCode = 200;

      //remove "UTF-8 Byte Order Mark (BOM)"
      char test[4] = { 0 };
      wFile.read(test, 3);
      if (strcmp(test, "\xEF\xBB\xBF") != 0)
      {
        wFile.seekg(0);

      }

      oResponse.mContent.assign((std::istreambuf_iterator<char>(wFile)),
        (std::istreambuf_iterator<char>()));

      std::string wExtention = ".txt";

      unsigned int wExtensionStart = static_cast<unsigned int>(iFilename.find_last_of('.'));
      if (wExtensionStart != iFilename.npos)
      {
        wExtention = iFilename.substr(wExtensionStart, iFilename.length());
      }

      std::stringstream wNumber;
      wNumber << oResponse.mContent.size();
      oResponse.mHeaders["Content-Length"] = wNumber.str();
      oResponse.mHeaders["Content-Type"] = BClassHttpProcessor::getContentType(wExtention);
    }
    else
    {
      oResponse.mResponseCode = 404;
      LOG_ERROR("File not availble [" << iFilename << "]");
    }

    oResponse.mHeaders["Connection"] = "close";

    wFile.close();

    return false;
  }

  bool XSC_HttpProvider::processWebSocketRequest(const CHttpRequest& iRequest, CHttpResponse& oResponse)
  {
    std::unordered_map<std::string, std::string>::const_iterator wIt;

    wIt = iRequest.mHeaders.find("Sec-WebSocket-Key");

    if (wIt != iRequest.mHeaders.end())
    {
      std::string wKey = wIt->second + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
      oResponse.mResponseCode = 101;
      oResponse.mHeaders["Upgrade"] = "websocket";
      oResponse.mHeaders["Connection"] = "Upgrade";
      oResponse.mHeaders["Sec-WebSocket-Accept"] = wKey;
      return true;
    }

    return false;
  }
}
