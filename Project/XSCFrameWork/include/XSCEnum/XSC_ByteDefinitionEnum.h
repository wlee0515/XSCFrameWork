#ifndef  XSC_BYTE_DEFINITION_ENUM_H
#define  XSC_BYTE_DEFINITION_ENUM_H

#include "XSCBase/XSC_TEnum.hxx"

namespace XSC
{
  enum eByteDefinition
  {
    eNullByte
    , eFullByte
    , eTextByte
    , eDataLength
    , eCommandID
    , eSourceID
    , eDestinationID
    , eCheckSum
    , eCheckSumInv
  };

  ENUM_DEFINITION_DECLARE(XSC, eNullByte, "NullByte");
  ENUM_DEFINITION_DECLARE(XSC, eFullByte, "FullByte");
  ENUM_DEFINITION_DECLARE(XSC, eTextByte, "TextByte");
  ENUM_DEFINITION_DECLARE(XSC, eDataLength, "DataLength");
  ENUM_DEFINITION_DECLARE(XSC, eCommandID, "CommandID");
  ENUM_DEFINITION_DECLARE(XSC, eSourceID, "SourceID");
  ENUM_DEFINITION_DECLARE(XSC, eDestinationID, "DestinationID");
  ENUM_DEFINITION_DECLARE(XSC, eCheckSum, "CheckSum");
  ENUM_DEFINITION_DECLARE(XSC, eCheckSumInv, "CheckSumInv");
}


#endif // ! XSC_CONNECTION_ENUM_H
