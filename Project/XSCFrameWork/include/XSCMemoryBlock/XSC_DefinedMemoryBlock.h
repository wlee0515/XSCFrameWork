#ifndef XSC_DEFINED_MEMORY_BLOCK_H
#define XSC_DEFINED_MEMORY_BLOCK_H

#include "XSCBase/XSC_Base.hxx"
#include "XSCEnum/XSC_DataTypeEnum.h"
#include "XSCMemoryBlock/XSC_MemoryBlockDefinition.h"
#include "XSCMemoryBlock/XSC_MemoryBlock.h"


namespace XSC
{
  class XSC_DefinedMemoryBlock : public XSC_Object
  {
  public:

    virtual void SClassSetup();
    virtual bool SClassStart();
    virtual bool SClassStop();

    XSC::XSC_MemoryBlock& getMemoryBlock();

    const XSC::XSC_MemoryBlock& getMemoryBlock() const;

  private:
    XSC::XSC_TArray<XSC::XSC_MemoryBlockDefinition> mDefinition;
    XSC::XSC_MemoryBlock mMemoryBlock;
  };
}
#endif // ! XSC_DATA_BLOCK_H
