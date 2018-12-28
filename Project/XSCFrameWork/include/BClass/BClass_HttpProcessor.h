#ifndef BCLASS_HTTP_PROCESSOR_H
#define BCLASS_HTTP_PROCESSOR_H

#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <vector>

namespace XSC {

  enum eHttpMethodEnum
  {
    eGET,
    eHEAD,
    ePOST,
    ePUT,
    eDELETE,
    eTRACE,
    eOPTIONS,
    eCONNECT,
    ePATCH,
    eERROR
  };

  class BClassHttpProcessor
  {
  public:
   class CRequest
    {
    public:
      std::string mRequestLine;
      std::string mHTTPVersion;
      std::string mRequestURL;
      eHttpMethodEnum mMethod;
      std::string mParameters;
      std::unordered_map<std::string, std::string> mHeaders;
      std::string mContent;
    };

    class CResponse
    {
    public:
      int mResponseCode;
      std::string mHTTPVersion;
      std::unordered_map<std::string, std::string> mHeaders;
      std::string mContent;
    };

    static void startup();
    static bool generateReponse(const CResponse& iResponse, std::string& oResponseString);
    static bool generateRequestForm(const std::string& iRawRequest, CRequest& oRequestForm);
    static const std::string& getReponseCode(int iRespose);
    static const std::string& getContentType(std::string iExtension);

  private:
    static void extractParameters(const std::string& iString, std::unordered_map<std::string, std::string>& oMap);

  };

  inline void BClassHttpProcessor::startup()
  {
    std::string wResponseCode = getReponseCode(0);
    std::string wContentType = getContentType(".txt");
  }

  inline bool BClassHttpProcessor::generateReponse(const CResponse& iResponse, std::string& oResponseString)
  {
    oResponseString = "";
    std::stringstream wReponseStream;
    oResponseString += iResponse.mHTTPVersion;
    oResponseString += " ";
    oResponseString += getReponseCode(iResponse.mResponseCode);

    std::unordered_map<std::string, std::string>::const_iterator wIt;

    for (wIt = iResponse.mHeaders.begin(); wIt != iResponse.mHeaders.end(); ++wIt)
    {
      oResponseString += "\r\n ";
      oResponseString += wIt->first;
      oResponseString += ": ";
      oResponseString += wIt->second;
    }

    if (0 != iResponse.mContent.size())
    {
      oResponseString += "\r\n\r\n";
      oResponseString += iResponse.mContent;
    }

    return true;
  }

  inline bool BClassHttpProcessor::generateRequestForm(const std::string& iRawRequest, CRequest& oRequestForm)
  {
    if (0 >= iRawRequest.size())
    {
      return false;
    }

    std::string cDeliminator("\r\n\r\n");

    unsigned int wBodyStart = iRawRequest.find(cDeliminator, 0);

    std::string wRequesStringHeader;

    if (wBodyStart != iRawRequest.size())
    {
      wRequesStringHeader = iRawRequest.substr(0, wBodyStart);
      wBodyStart += static_cast<unsigned int>(cDeliminator.size());

      oRequestForm.mContent = iRawRequest.substr(wBodyStart, iRawRequest.size() - wBodyStart);
    }
    else
    {
      wRequesStringHeader = iRawRequest;
    }

    std::stringstream wRequestStream(wRequesStringHeader);

    std::getline(wRequestStream, oRequestForm.mRequestLine);

    while (true != wRequestStream.eof())
    {
      std::string wHeader;
      std::getline(wRequestStream, wHeader, ':');

      if ("" != wHeader)
      {
        std::string wContent;
        std::getline(wRequestStream, wContent);

        if ("" != wContent)
        {
          oRequestForm.mHeaders[wHeader] = wContent;
        }
      }
    }

    std::stringstream wRequestlineStream(oRequestForm.mRequestLine);

    bool wMethodNotSet = true;
    bool wLastMethodSetFlag = wMethodNotSet;
    while (true != wRequestlineStream.eof())
    {
      std::string wArgument;
      wRequestlineStream >> wArgument;

      if (true != wRequestlineStream.fail())
      {
        if ("" != wArgument)
        {
          wLastMethodSetFlag = wMethodNotSet;
          if (true == wMethodNotSet)
          {
            if ("GET" == wArgument)
            {
              oRequestForm.mMethod = eGET;
              wMethodNotSet = false;
            }
            else if ("HEAD" == wArgument)
            {
              oRequestForm.mMethod = eHEAD;
              wMethodNotSet = false;
            }
            else if ("POST" == wArgument)
            {
              oRequestForm.mMethod = ePOST;
              wMethodNotSet = false;
            }
            else if ("PUT" == wArgument)
            {
              oRequestForm.mMethod = ePUT;
              wMethodNotSet = false;
            }
            else if ("DELETE" == wArgument)
            {
              oRequestForm.mMethod = eDELETE;
              wMethodNotSet = false;
            }
            else if ("TRACE" == wArgument)
            {
              oRequestForm.mMethod = eTRACE;
              wMethodNotSet = false;
            }
            else if ("OPTIONS" == wArgument)
            {
              oRequestForm.mMethod = eOPTIONS;
              wMethodNotSet = false;
            }
            else if ("CONNECT" == wArgument)
            {
              oRequestForm.mMethod = eCONNECT;
              wMethodNotSet = false;
            }
            else if ("PATCH" == wArgument)
            {
              oRequestForm.mMethod = ePATCH;
              wMethodNotSet = false;
            }
          }

          if ((wLastMethodSetFlag == false) || (wLastMethodSetFlag == wMethodNotSet))
          {
            if ("" == oRequestForm.mHTTPVersion)
            {
              std::stringstream wArgStream(wArgument);
              std::string wSubArg;
              std::getline(wArgStream, wSubArg, '/');
              if ("HTTP" == wSubArg)
              {
                oRequestForm.mHTTPVersion = wArgument;
                continue;
              }
            }

            if (false == wMethodNotSet)
            {

              oRequestForm.mRequestURL = wArgument;
              std::stringstream wArgStream(wArgument);
              getline(wArgStream, oRequestForm.mRequestURL, '?');

              std::string wSubParameters;
              getline(wArgStream, wSubParameters);

              if ("" != wSubParameters)
              {
                oRequestForm.mParameters = wSubParameters;
              }
              continue;
            }
          }
        }
      }
      if (true == wMethodNotSet)
      {
        return false;
      }
    }

    return true;
  }

  inline void BClassHttpProcessor::extractParameters(const std::string& iString, std::unordered_map<std::string, std::string>& oMap)
  {
    std::stringstream wStream(iString);

    std::string wParameter;
    std::string wParameterName;
    std::string wParameterValue;
    while (false == wStream.eof())
    {
      std::getline(wStream, wParameter, '&');
      if ((true != wStream.fail()) && ("" != wParameter))
      {
        std::stringstream wParameterStream(wParameter);
        std::getline(wParameterStream, wParameterName, '=');

        if ((true != wParameterStream.fail()) && ("" != wParameterName))
        {
          if (false == wParameterStream.eof())
          {
            oMap[wParameterName] = wParameterStream.str();
          }
        }
      }
    }
  }

  inline const std::string& BClassHttpProcessor::getReponseCode(int iRespose)
  {
    static std::map<int, std::string> cResponseMap;

    if (0 == cResponseMap.size())
    {
      cResponseMap[100] = "100 Continue";
      cResponseMap[101] = "101 Switching Protocols";
      cResponseMap[200] = "200 OK";
      cResponseMap[201] = "201 Created";
      cResponseMap[202] = "202 Accepted";
      cResponseMap[203] = "203 Non-Authoritative Information";
      cResponseMap[204] = "204 No Content";
      cResponseMap[205] = "205 Reset Content";
      cResponseMap[206] = "206 Partial Content";
      cResponseMap[300] = "300 Multiple Choices";
      cResponseMap[301] = "301 Moved Permanently";
      cResponseMap[302] = "302 Found";
      cResponseMap[303] = "303 See Other";
      cResponseMap[304] = "304 Not Modified";
      cResponseMap[305] = "305 Use Proxy";
      cResponseMap[307] = "307 Temporary Redirect";
      cResponseMap[400] = "400 Bad Request";
      cResponseMap[401] = "401 Unauthorized";
      cResponseMap[402] = "402 Payment Required";
      cResponseMap[403] = "403 Forbidden";
      cResponseMap[404] = "404 Not Found";
      cResponseMap[405] = "405 Method Not Allowed";
      cResponseMap[406] = "406 Not Acceptable";
      cResponseMap[407] = "407 Proxy Authentication Required";
      cResponseMap[408] = "408 Request Time-out";
      cResponseMap[409] = "409: Conflict";
      cResponseMap[410] = "410: Gone";
      cResponseMap[411] = "411: Length Required";
      cResponseMap[412] = "412: Precondition Failed";
      cResponseMap[413] = "413: Request Entity Too Large";
      cResponseMap[414] = "414: Request-URI Too Large";
      cResponseMap[415] = "415: Unsupported Media Type";
      cResponseMap[416] = "416: Requested range not satisfiable";
      cResponseMap[417] = "417: Expectation Failed";
      cResponseMap[500] = "500 Internal Server Error";
      cResponseMap[501] = "501 Not Implemented";
      cResponseMap[502] = "502 Bad Gateway";
      cResponseMap[503] = "503 Service Unavailable";
      cResponseMap[504] = "504 Gateway Time-out";
      cResponseMap[505] = "505 HTTP Version not supported";
    }

    std::map<int, std::string>::iterator wIt;
    wIt = cResponseMap.find(iRespose);

    if (wIt == cResponseMap.end())
    {
      return cResponseMap[503];
    }

    return wIt->second;
  }

  inline const std::string& BClassHttpProcessor::getContentType(std::string iExtension)
  {

    static std::map<std::string, std::string> cContenTypeMap;

    if (0 == cContenTypeMap.size())
    {
      cContenTypeMap[".bin"] = "application/macbinary";
      cContenTypeMap[".bmp"] = "image/bmp";
      cContenTypeMap[".c"] = "text/plain";
      cContenTypeMap[".com"] = "text/plain";
      cContenTypeMap[".cpp"] = "text/x-c";
      cContenTypeMap[".css"] = "text/css";
      cContenTypeMap[".cxx"] = "text/plain";
      cContenTypeMap[".doc"] = "application/msword";
      cContenTypeMap[".dot"] = "application/msword";
      cContenTypeMap[".h"] = "text/plain";
      cContenTypeMap[".htm"] = "text/html";
      cContenTypeMap[".html"] = "text/html";
      cContenTypeMap[".htmls"] = "text/html";
      cContenTypeMap[".ico"] = "image/x-icon";
      cContenTypeMap[".jpeg"] = "image/jpeg";;
      cContenTypeMap[".jpg"] = "image/jpeg";
      cContenTypeMap[".png"] = "image/png";
      cContenTypeMap[".txt"] = "text/plain";
      cContenTypeMap[".xml"] = "text/xml";
      cContenTypeMap[".zip"] = "application/zip";
    }

    std::map<std::string, std::string>::iterator wIt;
    wIt = cContenTypeMap.find(iExtension);

    if (wIt == cContenTypeMap.end())
    {
      return cContenTypeMap[".txt"];
    }

    return wIt->second;
  }

}
#endif