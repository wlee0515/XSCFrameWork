#ifndef XSC_SMDATATYPE_ENUM_H
#define XSC_SMDATATYPE_ENUM_H

#include "XSCBase/XSC_TEnum.hxx"

namespace XSC
{
  enum eDataType
  {
    eSInt8 = 1
    , eSInt16 = 2
    , eSInt32 = 3
    , eUInt8 = 4
    , eUInt16 = 5
    , eUInt32 = 6
    , eFloat32 = 7
    , eFloat64 = 8
    , eString = 9
  };

  ENUM_DEFINITION_DECLARE(XSC, eSInt8, "eSInt8");
  ENUM_DEFINITION_DECLARE(XSC, eSInt16, "eSInt16");
  ENUM_DEFINITION_DECLARE(XSC, eSInt32, "eSInt32");
  ENUM_DEFINITION_DECLARE(XSC, eUInt8, "eUInt8");
  ENUM_DEFINITION_DECLARE(XSC, eUInt16, "eUInt16");
  ENUM_DEFINITION_DECLARE(XSC, eUInt32, "eUInt32");
  ENUM_DEFINITION_DECLARE(XSC, eFloat32, "eFloat32");
  ENUM_DEFINITION_DECLARE(XSC, eFloat64, "eFloat64");
  ENUM_DEFINITION_DECLARE(XSC, eString, "eString");
}


namespace XSC
{
  inline unsigned int getEnumDataTypeSize(const eDataType& iDataType)
  {
    switch (iDataType)
    {
    case eSInt8:
      return sizeof(char);
    case eSInt16:
      return sizeof(short int);
    case eSInt32:
      return sizeof(int);
    case eUInt8:
      return sizeof(unsigned char);
    case eUInt16:
      return sizeof(unsigned short int);
    case eUInt32:
      return sizeof(unsigned int);
    case eFloat32:
      return sizeof(float);
    case eFloat64:
      return sizeof(double);
    case eString:
      return sizeof(char);
    default:
      break;
    }
    
    return 0;
  }

}

#endif
