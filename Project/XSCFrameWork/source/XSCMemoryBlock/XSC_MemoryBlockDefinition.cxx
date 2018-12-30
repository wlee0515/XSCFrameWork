#include "XSCMemoryBlock/XSC_MemoryBlockDefinition.h"

namespace XSC
{
  ADD_TYPE_FACTORY_WITH_NAME(XSC_MemoryBlockDefinition, "MemoryBlockDefinition");

  XSC_MemoryBlockDefinition::XSC_MemoryBlockDefinition()
    : mLabel("")
    , mCount(1)
    , mDataType(eSInt8)
    , mInitialValue("")
  {

  }

  void XSC_MemoryBlockDefinition::SClassSetup()
  {
    addChild("Label", mLabel);
    addChild("Count", mCount);
    addChild("DataType", mDataType);
    addChild("Value", mInitialValue);

    return XSC_Object::SClassSetup();
  }

  void XSC_MemoryBlockDefinition::setLabel(const std::string& iValue)
  {
    mLabel = iValue;
  }

  void XSC_MemoryBlockDefinition::setType(eDataType iValue)
  {
    mDataType = iValue;
  }

  void XSC_MemoryBlockDefinition::setCount(unsigned int iValue)
  {
    mCount = iValue;
  }

  void XSC_MemoryBlockDefinition::setInitialValue(const std::string& iValue)
  {
    mInitialValue = iValue;
  }

  const std::string& XSC_MemoryBlockDefinition::getLabel() const
  {
    return mLabel.getData();
  }

  const eDataType XSC_MemoryBlockDefinition::getType() const
  {
    return mDataType;
  }

  unsigned int XSC_MemoryBlockDefinition::getCount() const
  {
    if (0 != mCount)
    {
      return mCount.getData();
    }
    return 1;
  }

  const std::string& XSC_MemoryBlockDefinition::getInitialValue() const
  {
    return mInitialValue.getData();
  }

}
