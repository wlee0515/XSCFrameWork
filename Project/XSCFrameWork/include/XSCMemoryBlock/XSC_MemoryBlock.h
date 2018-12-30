#ifndef  XSC_MEMEORY_BLOCK_H
#define XSC_MEMEORY_BLOCK_H

#include "XSCBase/XSC_Common.hxx"
#include "XSCEnum/XSC_DataTypeEnum.h"
#include "XSCMemoryBlock/XSC_MemoryBlockDefinition.h"
#include <vector>

namespace XSC
{
  class XSC_MemoryBlock : public XSC_Object
  {
  public:

    XSC_MemoryBlock();
    virtual ~XSC_MemoryBlock();

    virtual void SClassSetup();

    unsigned int getVersion() const;

    unsigned int getSize() const;

    const std::vector<char>& getByteVector() const;

    XSC_VoidMemPtr* appendDataBlockDefinition(const XSC_MemoryBlockDefinition& iDefinition);

    const std::map<std::string, XSC_VoidMemPtr*>&   getLabelMap_Void() const;
    const std::map<std::string, XSC_SInt8MemPtr*>&   getLabelMap_SInt8() const;
    const std::map<std::string, XSC_SInt16MemPtr*>&  getLabelMap_SInt16() const;
    const std::map<std::string, XSC_SInt32MemPtr*>&  getLabelMap_SInt32() const;
    const std::map<std::string, XSC_UInt8MemPtr*>&   getLabelMap_UInt8() const;
    const std::map<std::string, XSC_UInt16MemPtr*>&  getLabelMap_UInt16() const;
    const std::map<std::string, XSC_UInt32MemPtr*>&  getLabelMap_UInt32() const;
    const std::map<std::string, XSC_Float32MemPtr*>& getLabelMap_Float32() const;
    const std::map<std::string, XSC_Float64MemPtr*>& getLabelMap_Float64() const;
    const std::map<std::string, XSC_CStringMemPtr*>& getLabelMap_CString() const;

    XSC_VoidMemPtr*   getLabel_Void(const std::string& iLabel);
    XSC_SInt8MemPtr*   getLabel_SInt8(const std::string& iLabel);
    XSC_SInt16MemPtr*  getLabel_SInt16(const std::string& iLabel);
    XSC_SInt32MemPtr*  getLabel_SInt32(const std::string& iLabel);
    XSC_UInt8MemPtr*   getLabel_UInt8(const std::string& iLabel);
    XSC_UInt16MemPtr*  getLabel_UInt16(const std::string& iLabel);
    XSC_UInt32MemPtr*  getLabel_UInt32(const std::string& iLabel);
    XSC_Float32MemPtr* getLabel_Float32(const std::string& iLabel);
    XSC_Float64MemPtr* getLabel_Float64(const std::string& iLabel);
    XSC_CStringMemPtr* getLabel_CString(const std::string& iLabel);

    const XSC_VoidMemPtr*   getLabel_Void(const std::string& iLabel) const;
    const XSC_SInt8MemPtr*   getLabel_SInt8(const std::string& iLabel) const;
    const XSC_SInt16MemPtr*  getLabel_SInt16(const std::string& iLabel) const;
    const XSC_SInt32MemPtr*  getLabel_SInt32(const std::string& iLabel) const;
    const XSC_UInt8MemPtr*   getLabel_UInt8(const std::string& iLabel) const;
    const XSC_UInt16MemPtr*  getLabel_UInt16(const std::string& iLabel) const;
    const XSC_UInt32MemPtr*  getLabel_UInt32(const std::string& iLabel) const;
    const XSC_Float32MemPtr* getLabel_Float32(const std::string& iLabel) const;
    const XSC_Float64MemPtr* getLabel_Float64(const std::string& iLabel) const;
    const XSC_CStringMemPtr* getLabel_CString(const std::string& iLabel) const;

  private:

    std::vector<char>   mMemoryBlock;
    XSC::XSC_UInt32              mMemoryBlockSize;
    XSC::XSC_UInt32              mMemoryBlockVersion;

    std::map<std::string, XSC_VoidMemPtr*> mLabelMap_Void;
    std::map<std::string, XSC_SInt8MemPtr*> mLabelMap_SInt8;
    std::map<std::string, XSC_SInt16MemPtr*> mLabelMap_SInt16;
    std::map<std::string, XSC_SInt32MemPtr*> mLabelMap_SInt32;
    std::map<std::string, XSC_UInt8MemPtr*> mLabelMap_UInt8;
    std::map<std::string, XSC_UInt16MemPtr*> mLabelMap_UInt16;
    std::map<std::string, XSC_UInt32MemPtr*> mLabelMap_UInt32;
    std::map<std::string, XSC_Float32MemPtr*> mLabelMap_Float32;
    std::map<std::string, XSC_Float64MemPtr*> mLabelMap_Float64;
    std::map<std::string, XSC_CStringMemPtr*> mLabelMap_CString;
  };
}
#endif // ! XSC_DATA_BLOCK_H
