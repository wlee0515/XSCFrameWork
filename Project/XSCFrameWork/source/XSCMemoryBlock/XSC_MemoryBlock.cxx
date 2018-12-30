#include "XSCMemoryBlock/XSC_MemoryBlock.h"

namespace {

  template< typename T>
  T* createMemoryBlockEntry(const XSC::XSC_MemoryBlockDefinition& iDefinition, std::map<std::string, T*>& iMap, std::vector<char>& iMemoryBlock)
  {
    unsigned int wDataCount = 0;
    wDataCount = iDefinition.getCount();
    if (0 == wDataCount)
    {
      return nullptr;
    }

    unsigned int wDataSize = 0;
    wDataSize = wDataCount*getEnumDataTypeSize(iDefinition.getType());

    if (0 == wDataSize)
    {
      return nullptr;
    }

    if (iMap.end() != iMap.find(iDefinition.getLabel()))
    {
      return nullptr;
    }

    T * wNewTypedPtr = new T();
    iMap[iDefinition.getLabel()] = wNewTypedPtr;

    unsigned int wDataHead = iMemoryBlock.size();
    unsigned int wNewMemorySize = wDataHead + wDataSize;
    iMemoryBlock.resize(wNewMemorySize, 0);

    wNewTypedPtr->setMemoryBlockData(iMemoryBlock, wDataHead, wDataCount);
    wNewTypedPtr->setValue(iDefinition.getInitialValue());

    return wNewTypedPtr;
  }

  template<typename T>
  T* findLabelInMap(std::map<std::string, T*> iMap, const std::string& iLabel)
  {
    typename std::map<std::string, T*>::iterator wIt = iMap.find(iLabel);

    if (iMap.end() != wIt)
    {
      return wIt->second;
    }
    return nullptr;
  }

  template<typename T>
  const T* findLabelInMap_const(const std::map<std::string, T*> iMap, const std::string& iLabel)
  {
    typename std::map<std::string, T*>::const_iterator wIt = iMap.find(iLabel);

    if (iMap.end() != wIt)
    {
      return wIt->second;
    }
    return nullptr;
  }
}

namespace XSC
{
  ADD_TYPE_FACTORY_WITH_NAME(XSC_MemoryBlock, "MemoryBlock");

  XSC_MemoryBlock::XSC_MemoryBlock()
    : mMemoryBlockSize(0)
  {
    mMemoryBlock.clear();
  }

  XSC_MemoryBlock::~XSC_MemoryBlock()
  {
    mMemoryBlock.clear();
    mMemoryBlockSize = 0;
  }

  void XSC_MemoryBlock::SClassSetup()
  {
    addChild("MemoryBlockVersion", mMemoryBlockVersion);
    addChild("MemoryBlockSize", mMemoryBlockSize);
    return XSC_Object::SClassSetup();
  }

  unsigned int XSC_MemoryBlock::getVersion() const
  {
    return mMemoryBlockVersion;
  }

  unsigned int XSC_MemoryBlock::getSize() const
  {
    return mMemoryBlock.size();
  }

  const std::vector<char>& XSC_MemoryBlock::getByteVector() const
  {
    return mMemoryBlock;
  }

  XSC_VoidMemPtr* XSC_MemoryBlock::appendDataBlockDefinition(const XSC_MemoryBlockDefinition& iDefinition)
  {
    if (nullptr != this->getDataRef(iDefinition.getLabel()))
    {
      return nullptr;
    }

    XSC_VoidMemPtr* wNewDataPtr(nullptr);
    switch (iDefinition.getType())
    {
    case eSInt8:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_SInt8, mMemoryBlock);
      break;
    }
    case eSInt16:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_SInt16, mMemoryBlock);
      break;
    }
    case eSInt32:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_SInt32, mMemoryBlock);
      break;
    }
    case eUInt8:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_UInt8, mMemoryBlock);
      break;
    }
    case eUInt16:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_UInt16, mMemoryBlock);
      break;
    }
    case eUInt32:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_UInt32, mMemoryBlock);
      break;
    }
    case eFloat32:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_Float32, mMemoryBlock);
      break;
    }
    case eFloat64:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_Float64, mMemoryBlock);
      break;
    }
    case eString:
    {
      wNewDataPtr = createMemoryBlockEntry(iDefinition, mLabelMap_CString, mMemoryBlock);
      break;
    }
    default:
      break;
    }

    if (nullptr != wNewDataPtr)
    {
      this->addChild(iDefinition.getLabel(), *wNewDataPtr, true);
      mMemoryBlockVersion = 1 + mMemoryBlockVersion;
      mMemoryBlockSize = mMemoryBlock.size();
      mLabelMap_Void[iDefinition.getLabel()] = wNewDataPtr;
      return wNewDataPtr;
    }

    return nullptr;
  }

  const std::map<std::string, XSC_VoidMemPtr*>&   XSC_MemoryBlock::getLabelMap_Void() const
  {
    return mLabelMap_Void;
  }

  const std::map<std::string, XSC_SInt8MemPtr*>&   XSC_MemoryBlock::getLabelMap_SInt8() const
  {
    return mLabelMap_SInt8;
  }

  const std::map<std::string, XSC_SInt16MemPtr*>&  XSC_MemoryBlock::getLabelMap_SInt16() const
  {
    return mLabelMap_SInt16;
  }

  const std::map<std::string, XSC_SInt32MemPtr*>&  XSC_MemoryBlock::getLabelMap_SInt32() const
  {
    return mLabelMap_SInt32;
  }

  const std::map<std::string, XSC_UInt8MemPtr*>&   XSC_MemoryBlock::getLabelMap_UInt8() const
  {
    return mLabelMap_UInt8;
  }

  const std::map<std::string, XSC_UInt16MemPtr*>&  XSC_MemoryBlock::getLabelMap_UInt16() const
  {
    return mLabelMap_UInt16;
  }

  const std::map<std::string, XSC_UInt32MemPtr*>&  XSC_MemoryBlock::getLabelMap_UInt32() const
  {
    return mLabelMap_UInt32;
  }

  const std::map<std::string, XSC_Float32MemPtr*>& XSC_MemoryBlock::getLabelMap_Float32() const
  {
    return mLabelMap_Float32;
  }

  const std::map<std::string, XSC_Float64MemPtr*>& XSC_MemoryBlock::getLabelMap_Float64() const
  {
    return mLabelMap_Float64;
  }

  const std::map<std::string, XSC_CStringMemPtr*>& XSC_MemoryBlock::getLabelMap_CString() const
  {
    return mLabelMap_CString;
  }

  XSC_VoidMemPtr*   XSC_MemoryBlock::getLabel_Void(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_Void, iLabel);
  }

  XSC_SInt8MemPtr*   XSC_MemoryBlock::getLabel_SInt8(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_SInt8, iLabel);
  }

  XSC_SInt16MemPtr*  XSC_MemoryBlock::getLabel_SInt16(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_SInt16, iLabel);
  }

  XSC_SInt32MemPtr*  XSC_MemoryBlock::getLabel_SInt32(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_SInt32, iLabel);
  }

  XSC_UInt8MemPtr*   XSC_MemoryBlock::getLabel_UInt8(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_UInt8, iLabel);
  }

  XSC_UInt16MemPtr*  XSC_MemoryBlock::getLabel_UInt16(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_UInt16, iLabel);
  }

  XSC_UInt32MemPtr*  XSC_MemoryBlock::getLabel_UInt32(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_UInt32, iLabel);
  }

  XSC_Float32MemPtr* XSC_MemoryBlock::getLabel_Float32(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_Float32, iLabel);
  }

  XSC_Float64MemPtr* XSC_MemoryBlock::getLabel_Float64(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_Float64, iLabel);
  }

  XSC_CStringMemPtr* XSC_MemoryBlock::getLabel_CString(const std::string& iLabel)
  {
    return findLabelInMap(mLabelMap_CString, iLabel);
  }

  const XSC_VoidMemPtr*   XSC_MemoryBlock::getLabel_Void(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_Void, iLabel);
  }

  const XSC_SInt8MemPtr*   XSC_MemoryBlock::getLabel_SInt8(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_SInt8, iLabel);
  }

  const XSC_SInt16MemPtr*  XSC_MemoryBlock::getLabel_SInt16(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_SInt16, iLabel);
  }

  const XSC_SInt32MemPtr*  XSC_MemoryBlock::getLabel_SInt32(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_SInt32, iLabel);
  }

  const XSC_UInt8MemPtr*   XSC_MemoryBlock::getLabel_UInt8(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_UInt8, iLabel);
  }

  const XSC_UInt16MemPtr*  XSC_MemoryBlock::getLabel_UInt16(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_UInt16, iLabel);
  }

  const XSC_UInt32MemPtr*  XSC_MemoryBlock::getLabel_UInt32(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_UInt32, iLabel);
  }

  const XSC_Float32MemPtr* XSC_MemoryBlock::getLabel_Float32(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_Float32, iLabel);
  }

  const XSC_Float64MemPtr* XSC_MemoryBlock::getLabel_Float64(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_Float64, iLabel);
  }

  const XSC_CStringMemPtr* XSC_MemoryBlock::getLabel_CString(const std::string& iLabel) const
  {
    return findLabelInMap(mLabelMap_CString, iLabel);
  }

}
