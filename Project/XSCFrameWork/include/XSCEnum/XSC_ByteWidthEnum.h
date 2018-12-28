#ifndef  XSC_BYTE_WIDTH_ENUM_H
#define  XSC_BYTE_WIDTH_ENUM_H

#include "XSCBase\XSC_TEnum.hxx"
#include "XSCEnum\XSC_DataTypeEnum.h"

namespace XSC
{
  enum eByteWidth
  {
      eByteWidth_1
    , eByteWidth_2
    , eByteWidth_4
  };

  ENUM_DEFINITION_START(XSC::eByteWidth)
    ENUM_DEFINITION_DECLARE(XSC::eByteWidth_1,      "1ByteWidth");
    ENUM_DEFINITION_DECLARE(XSC::eByteWidth_2,      "2ByteWidth");
    ENUM_DEFINITION_DECLARE(XSC::eByteWidth_4,      "4ByteWidth");
  ENUM_DEFINITION_END


  inline eDataType getByteWidthDataType_SInt(const eByteWidth& iDataType)
  {
    switch (iDataType)
    {
    case eByteWidth_1:
      return eSInt8;
    case eByteWidth_2:
      return eSInt16;
    case eByteWidth_4:
      return eSInt32;
    default:
        break;
    }

    return eSInt8;
  }

  inline eDataType getByteWidthDataType_UInt(const eByteWidth& iDataType)
  {
    switch (iDataType)
    {
    case eByteWidth_1:
      return eUInt8;
    case eByteWidth_2:
      return eUInt16;
    case eByteWidth_4:
      return eUInt32;
    default:
      break;
    }

    return eUInt8;
  }
}


#endif // ! XSC_CONNECTION_ENUM_H
