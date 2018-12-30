#include "XSCService/XSC_MemoryBlock_HttpAdapter.h"
#include "XSCServiceProvider/XSC_HttpProvider.h"
#include "XSCMemoryBlock/XSC_DefinedMemoryBlock.h"
#include "BClass/BClass_JSON.h"

#include <vector>


namespace
{
  void generatePacket(std::string& oPacket, const XSC::XSC_MemoryBlockHttpAdapter::PacketHeader& ioPacketHead, const std::vector<char>& iDataString)
  {
    unsigned int wPacketHeadSize = sizeof(XSC::XSC_MemoryBlockHttpAdapter::PacketHeader);
    unsigned int wDataCount = 0;
    wDataCount = wPacketHeadSize + iDataString.size();
    std::vector<char> wDataVector;
    wDataVector.resize(wDataCount, 0);

    memcpy(&wDataVector[0], &ioPacketHead, wPacketHeadSize);
    memcpy(&wDataVector[wPacketHeadSize], &iDataString[0], iDataString.size());

    std::string wOutputDataString(wDataVector.data(), wDataVector.size());
    oPacket.swap(wOutputDataString);
  }

  void generatePacket(std::string& oPacket, const XSC::XSC_MemoryBlockHttpAdapter::PacketHeader& ioPacketHead, const std::string& iDataString)
  {
    unsigned int wPacketHeadSize = sizeof(XSC::XSC_MemoryBlockHttpAdapter::PacketHeader);
    unsigned int wDataCount = 0;
    wDataCount = wPacketHeadSize + iDataString.size();
    std::vector<char> wDataVector;
    wDataVector.resize(wDataCount, 0);

    memcpy(&wDataVector[0], &ioPacketHead, wPacketHeadSize);
    memcpy(&wDataVector[wPacketHeadSize], &iDataString[0], iDataString.size());

    std::string wOutputDataString(wDataVector.data(), wDataVector.size());
    oPacket.swap(wOutputDataString);
  }

  void addMemPtrToJSONMap(XSC::BClassJSON::JSONObjMap& oJSONMap, const std::string& iLabel, const XSC::XSC_VoidMemPtr& iTypeDefine)
  {
    XSC::BClassJSON::JSONObjMap& wTypeDefinition = oJSONMap[iLabel].makeJSONMap();
    wTypeDefinition["Type"].makeJSONNumber().getData() = iTypeDefine.getDataType();
    wTypeDefinition["Count"].makeJSONNumber().getData() = iTypeDefine.getCount();
    wTypeDefinition["Offset"].makeJSONNumber().getData() = iTypeDefine.getOffsetCount();
  }

  template<typename T>
  void addMemPtrMapToJSONMap(XSC::BClassJSON::JSONObjMap& oJSONMap, const std::map < std::string, T*> & iMemPtrMap)
  {
    for (std::map < std::string, T*>::const_iterator wIt = iMemPtrMap.begin(); wIt != iMemPtrMap.end(); ++wIt)
    {
      if (nullptr != wIt->second)
      {
        addMemPtrToJSONMap(oJSONMap, wIt->first, *(wIt->second));
      }
    }
  }
}

namespace XSC
{
  ADD_TYPE_FACTORY_WITH_NAME(XSC_MemoryBlockHttpAdapter, "MemoryBlockHttpAdapter");

  XSC_MemoryBlockHttpAdapter::PacketHeader::PacketHeader() : mPacketHead("SHAREMEMRPLY")
    , mHeaderLength(0)
    , mDataType(eBinaryData)
    , mDataMapVersion(0)
    , mDataLength(0)
  {
  }

  XSC_MemoryBlockHttpAdapter::XSC_MemoryBlockHttpAdapter()
    : UriService("ShareMemory", ".bin")
  {

  }

  void XSC_MemoryBlockHttpAdapter::SClassSetup()
  {
    XSC::XSC_HttpProvider::UriService::SClassSetup();
  }

  bool XSC_MemoryBlockHttpAdapter::SClassStart()
  {
    return XSC::XSC_HttpProvider::UriService::SClassStart();;
  }

  bool XSC_MemoryBlockHttpAdapter::processHttpPOSTMethod(const std::string& iURI, const std::string& iRequest, std::string& oResponse)
  {
    BClassJSON wJSONObject;
    if (true == wJSONObject.parseJSONString(iRequest))
    {
      BClassJSON::JSONObjMap* wJSONMap_Ptr = wJSONObject.getJSONObjTypePtr_Map();
      if (nullptr != wJSONMap_Ptr)
      {
        BClassJSON::JSONObjMap::iterator wCmdNameIt = wJSONMap_Ptr->find("cmd");

        if (wCmdNameIt != wJSONMap_Ptr->end())
        {
          BClassJSON::JSONObjString* wCmdNameString = wCmdNameIt->second.getJSONObjTypePtr_String();

          if (nullptr != wCmdNameString)
          {
            std::string wCommand = wCmdNameString->getData();

            if ("GetDataMap" == wCommand)
            {
              createDataMapResponse(oResponse);
            }
            else if ("GetDataBlock" == wCommand)
            {
              createDataBlockResponse(oResponse);
            }
            else if ("SetDataBlock" == wCommand)
            {
              createJSONSetDataListResponse(*wJSONMap_Ptr, oResponse);
            }
            else if ("SetGetDataBlock" == wCommand)
            {
              std::string wDummyString("");
              createJSONSetDataListResponse(*wJSONMap_Ptr, oResponse);
              createDataBlockResponse(oResponse);
            }
          }
        }
      }
    }
    return true;
  }


  bool XSC_MemoryBlockHttpAdapter::createDataMapResponse(std::string& oResponse)
  {
    XSC_DefinedMemoryBlock&  wCommonDataBase = XSC::Singleton::getSingleton<XSC::XSC_DefinedMemoryBlock>();
    XSC_MemoryBlock& wMemoryBlock = wCommonDataBase.getMemoryBlock();

    BClassJSON wJSONObject;
    BClassJSON::JSONObjMap& wLabelMap = wJSONObject.makeJSONMap();

    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_SInt8());
    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_SInt16());
    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_SInt32());
    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_UInt8());
    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_UInt16());
    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_UInt32());
    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_Float32());
    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_Float64());
    addMemPtrMapToJSONMap(wLabelMap, wMemoryBlock.getLabelMap_CString());

    std::string wJSONString("");
    wLabelMap.generateJSONString(wJSONString);

    XSC_MemoryBlockHttpAdapter::PacketHeader wHeader;
    wHeader.mHeaderLength = sizeof(PacketHeader);
    wHeader.mDataMapVersion = wMemoryBlock.getVersion();
    wHeader.mDataType = eJSONData;
    wHeader.mDataLength = wJSONString.size();

    generatePacket(oResponse, wHeader, wJSONString);
    return true;
  }

  bool XSC_MemoryBlockHttpAdapter::createDataBlockResponse(std::string& oResponse)
  {
    XSC_DefinedMemoryBlock&  wCommonDataBase = XSC::Singleton::getSingleton<XSC::XSC_DefinedMemoryBlock>();
    XSC_MemoryBlock& wMemoryBlock = wCommonDataBase.getMemoryBlock();
    const std::vector<char>& wDataVector = wMemoryBlock.getByteVector();

    XSC_MemoryBlockHttpAdapter::PacketHeader wHeader;
    wHeader.mHeaderLength = sizeof(PacketHeader);
    wHeader.mDataMapVersion = wMemoryBlock.getVersion();
    wHeader.mDataType = eBinaryData;
    wHeader.mDataLength = wDataVector.size();

    generatePacket(oResponse, wHeader, wDataVector);
    return true;
  }

  bool XSC_MemoryBlockHttpAdapter::createJSONSetDataListResponse(BClassJSON::JSONObjMap& iRequest, std::string& oResponse)
  {
    XSC_DefinedMemoryBlock&  wCommonDataBase = XSC::Singleton::getSingleton<XSC::XSC_DefinedMemoryBlock>();
    XSC_MemoryBlock& wMemoryBlock = wCommonDataBase.getMemoryBlock();

    BClassJSON::JSONObjMap::iterator wSetListIt = iRequest.find("SetList");

    if (wSetListIt == iRequest.end())
    {
      return false;
    }
    BClassJSON::JSONObjArray* wSetListArray_Ptr = wSetListIt->second.getJSONObjTypePtr_Array();
    if (nullptr == wSetListArray_Ptr)
    {
      return false;
    }

    for (BClassJSON::JSONObjArray::iterator wElementIt = wSetListArray_Ptr->begin(); wElementIt != wSetListArray_Ptr->end(); ++wElementIt)
    {
      BClassJSON::JSONObjMap* wElementMap = wElementIt->getJSONObjTypePtr_Map();

      if (nullptr == wElementMap)
      {
        continue;
      }

      BClassJSON::JSONObjMap::iterator wLabelIt = wElementMap->find("Label");

      if (wElementMap->end() == wLabelIt)
      {
        continue;
      }

      BClassJSON::JSONObjString* wLabelString = wLabelIt->second.getJSONObjTypePtr_String();

      if (nullptr == wLabelString)
      {
        continue;
      }

      BClassJSON::JSONObjMap::iterator wIndextIt = wElementMap->find("Index");

      if (wElementMap->end() == wIndextIt)
      {
        continue;
      }
      BClassJSON::JSONObjNumber* wIndexNumber = wIndextIt->second.getJSONObjTypePtr_Number();

      if (nullptr == wIndexNumber)
      {
        continue;
      }

      unsigned int wIndex = static_cast<unsigned int>(wIndexNumber->getData() + 0.5);

      BClassJSON::JSONObjMap::iterator wValueIt = wElementMap->find("Value");

      BClassJSON::JSONObjString* wValueString = wValueIt->second.getJSONObjTypePtr_String();

      if (nullptr == wValueString)
      {
        continue;
      }

      if (wElementMap->end() == wValueIt)
      {
        continue;
      }

      XSC_VoidMemPtr* wMemBlockPtr = wMemoryBlock.getLabel_Void(wLabelString->getData());

      if (nullptr == wMemBlockPtr)
      {
        continue;
      }

      void* wDataPtr = wMemBlockPtr->getVoidPtrAt(wIndex);

      if (nullptr == wDataPtr)
      {
        continue;
      }

      switch (wMemBlockPtr->getDataType())
      {
      case eSInt8:
      {
        XSC::StrTools::setStringToValue(wValueString->getData(), *static_cast<char*>(wDataPtr));
      }
      break;
      case eSInt16:
      {
        XSC::StrTools::setStringToValue(wValueString->getData(), *static_cast<short*>(wDataPtr));
      }
      break;
      case eSInt32:
      {
        XSC::StrTools::setStringToValue(wValueString->getData(), *static_cast<int*>(wDataPtr));
      }
      break;
      case eUInt8:
      {
        XSC::StrTools::setStringToValue(wValueString->getData(), *static_cast<unsigned char*>(wDataPtr));
      }
      break;
      case eUInt16:
      {
        XSC::StrTools::setStringToValue(wValueString->getData(), *static_cast<unsigned short*>(wDataPtr));
      }
      break;
      case eUInt32:
      {
        XSC::StrTools::setStringToValue(wValueString->getData(), *static_cast<unsigned int*>(wDataPtr));
      }
      break;
      case eFloat32:
      {
        XSC::StrTools::setStringToValue(wValueString->getData(), *static_cast<float*>(wDataPtr));
      }
      break;
      case eFloat64:
      {
        XSC::StrTools::setStringToValue(wValueString->getData(), *static_cast<double*>(wDataPtr));
      }
      break;
      case eString:
      {
        std::string& wStringToCopy = wValueString->getData();
        unsigned int wMaxLengthToCopy = wMemBlockPtr->getCount() - wIndex;

        if (wMaxLengthToCopy > wStringToCopy.length())
        {
          wMaxLengthToCopy = wStringToCopy.length();
        }

        memcpy(wDataPtr, wStringToCopy.c_str(), wMaxLengthToCopy);
      }
      break;
      default:
        break;
      }
    }

    std::string wText = "SetData Complete";

    XSC_MemoryBlockHttpAdapter::PacketHeader wHeader;
    wHeader.mHeaderLength = sizeof(PacketHeader);
    wHeader.mDataMapVersion = wMemoryBlock.getVersion();
    wHeader.mDataType = eTextData;
    wHeader.mDataLength = wText.size();

    generatePacket(oResponse, wHeader, wText);
    return true;
  }
}
