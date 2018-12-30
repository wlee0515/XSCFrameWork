#ifndef XSC_MEMORY_BLOCK_DEFINITION_H
#define XSC_MEMORY_BLOCK_DEFINITION_H

#include "XSCBase/XSC_Common.hxx"
#include "XSCEnum/XSC_DataTypeEnum.h"

namespace XSC
{
  class XSC_MemoryBlockDefinition : public XSC::XSC_Object
  {
  public:
    XSC_MemoryBlockDefinition();

    virtual void SClassSetup();

    void setLabel(const std::string& iValue);
    void setType(eDataType iValue);
    void setCount(unsigned int iValue);
    void setInitialValue(const std::string& iValue);

    const std::string& getLabel() const;
    const eDataType getType() const;
    unsigned int getCount() const;
    const std::string& getInitialValue() const;

  private:
    XSC_String mLabel;
    XSC_UInt32 mCount;
    XSC_TEnum<eDataType>  mDataType;
    XSC_String mInitialValue;
  };

}
#endif // !XSC_DATA_BLOCK_DEFINITION_H
