#ifndef  XSC_CONNECTION_ENUM_H
#define  XSC_CONNECTION_ENUM_H

#include "XSCBase/XSC_TEnum.hxx"

namespace XSC
{
  enum eConnectionType
  {
    eNoConnection
    , eLoop
    , eRS232
    , eTCPClient
    , eTCPServer
    , eUDPClient
    , eUDPServer
  };
}

ENUM_DEFINITION_START(XSC::eConnectionType)
  ENUM_DEFINITION_DECLARE(XSC::eNoConnection, "NoConnection");
  ENUM_DEFINITION_DECLARE(XSC::eLoop, "Loop");
  ENUM_DEFINITION_DECLARE(XSC::eRS232, "RS232");
  ENUM_DEFINITION_DECLARE(XSC::eTCPClient, "TCPClient");
  ENUM_DEFINITION_DECLARE(XSC::eTCPServer, "TCPServer");
  ENUM_DEFINITION_DECLARE(XSC::eUDPClient, "UDPClient");
  ENUM_DEFINITION_DECLARE(XSC::eUDPServer, "UDPServer");
ENUM_DEFINITION_END
ENUM_FORWARD_DECLARATION(eConnectionType)



#endif // ! XSC_CONNECTION_ENUM_H
