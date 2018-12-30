#include "XSCMemoryBlock/XSC_DefinedMemoryBlock.h"

namespace XSC
{
  ADD_TYPE_FACTORY_WITH_NAME(XSC_MemoryBlockDefinition, "DefinedMemoryBlock");

  void XSC_DefinedMemoryBlock::SClassSetup()
  {
    addChild("Definition", mDefinition);
    addChild("MemoryBlock", mMemoryBlock);

    return XSC_Object::SClassSetup();
  }


  bool XSC_DefinedMemoryBlock::SClassStart()
  {
    for (unsigned int wi = 0; wi < mDefinition.size(); ++wi)
    {
      XSC_MemoryBlockDefinition& wDefinedData = mDefinition[wi];
      mMemoryBlock.appendDataBlockDefinition(wDefinedData);
    }

    return XSC_Object::SClassStart();
  }

  bool XSC_DefinedMemoryBlock::SClassStop()
  {
    return XSC_Object::SClassStop();
  }

  XSC::XSC_MemoryBlock& XSC_DefinedMemoryBlock::getMemoryBlock()
  {
    return mMemoryBlock;
  }

  const XSC::XSC_MemoryBlock& XSC_DefinedMemoryBlock::getMemoryBlock() const
  {
    return mMemoryBlock;
  }

}
