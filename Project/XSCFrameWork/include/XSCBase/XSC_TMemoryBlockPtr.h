#ifndef XSC_TMEMORY_BLOCK_PTR_H
#define XSC_TMEMORY_BLOCK_PTR_H

#include "XSCBase\XSC_Base.hxx"
#include "XSCEnum\XSC_DataTypeEnum.h"
#include "XSCUtility\XSC_StringTools.h"
#include <string>

namespace XSC
{
  class XSC_VoidMemPtr : public XSC_Data
  {
  public:
    XSC_VoidMemPtr( eDataType iType )
      : mMemoryBlockPtr(nullptr)
      , mOffsetCount(0)
      , mCount(1)
      , mDataType(iType)
    {
    }

    XSC_VoidMemPtr(std::vector<char>& iMemoryBlock, unsigned int iOffset, unsigned int iCount, eDataType iType)
      : mMemoryBlockPtr(&iMemoryBlock)
      , mOffsetCount(iOffset)
      , mCount(iCount)
      , mDataType(iType)
    {
      if (0 == mCount)
      {
        mCount = 1;
      }
    }

    virtual void setValue(const std::string& iValue)
    {
      void* wDataPtr = getVoidDataPtr();
      if ((nullptr != wDataPtr) && (nullptr != mMemoryBlockPtr))
      {
        unsigned int wEnd = mOffsetCount + mCount*getEnumDataTypeSize(mDataType);

        if (wEnd <= mMemoryBlockPtr->size())
        {
          switch (mDataType)
          {
          case eSInt8:
            XSC::StrTools::setStringToValue(iValue, reinterpret_cast<char*>(wDataPtr), mCount);
            return;
          case eSInt16:
            XSC::StrTools::setStringToValue(iValue, reinterpret_cast<short int*>(wDataPtr), mCount);
            return;
          case eSInt32:
            XSC::StrTools::setStringToValue(iValue, reinterpret_cast<int*>(wDataPtr), mCount);
            return;
          case eUInt8:
            XSC::StrTools::setStringToValue(iValue, reinterpret_cast<unsigned char*>(wDataPtr), mCount);
            return;
          case eUInt16:
            XSC::StrTools::setStringToValue(iValue, reinterpret_cast<unsigned short int*>(wDataPtr), mCount);
            return;
          case eUInt32:
            XSC::StrTools::setStringToValue(iValue, reinterpret_cast<unsigned int*>(wDataPtr), mCount);
            return;
          case eFloat32:
            XSC::StrTools::setStringToValue(iValue, reinterpret_cast<float*>(wDataPtr), mCount);
            return;
          case eFloat64:
            XSC::StrTools::setStringToValue(iValue, reinterpret_cast<double*>(wDataPtr), mCount);
            return;
          case eString:
            XSC::StrTools::setStringToCString(iValue, reinterpret_cast< char*>(wDataPtr), mCount);
            return;
          default:
            break;
          }
        }
      }

      return;
    }

    virtual std::string getValue() const
    {
      const void* wDataPtr = getVoidDataPtr();
      if ((nullptr != wDataPtr) && (nullptr != mMemoryBlockPtr))
      {
        unsigned int wEnd = mOffsetCount + mCount*getEnumDataTypeSize(mDataType);

        if (wEnd <= mMemoryBlockPtr->size())
        {

          switch (mDataType)
          {
          case eSInt8:
            return XSC::StrTools::printValueToString(reinterpret_cast<const char*>(wDataPtr), mCount);
          case eSInt16:
            return XSC::StrTools::printValueToString(reinterpret_cast<const short int*>(wDataPtr), mCount);
          case eSInt32:
            return XSC::StrTools::printValueToString(reinterpret_cast<const int*>(wDataPtr), mCount);
          case eUInt8:
            return XSC::StrTools::printValueToString(reinterpret_cast<const unsigned char*>(wDataPtr), mCount);
          case eUInt16:
            return XSC::StrTools::printValueToString(reinterpret_cast<const unsigned short int*>(wDataPtr), mCount);
          case eUInt32:
            return XSC::StrTools::printValueToString(reinterpret_cast<const unsigned int*>(wDataPtr), mCount);
          case eFloat32:
            return XSC::StrTools::printValueToString(reinterpret_cast<const float*>(wDataPtr), mCount);
          case eFloat64:
            return XSC::StrTools::printValueToString(reinterpret_cast<const double*>(wDataPtr), mCount);
          case eString:
            return XSC::StrTools::printCStringToString(reinterpret_cast<const char*>(wDataPtr), mCount);
          default:
            break;
          }
        }
      }

      return "";
    }

    void setMemoryBlockData(std::vector<char>& iMemoryBlock, unsigned int iOffset, unsigned int iCount)
    {
      mMemoryBlockPtr = &iMemoryBlock;
      mOffsetCount = iOffset;
      mCount = iCount;
      if (0 == mCount)
      {
        mCount = 1;
      }
    }

    virtual unsigned int getOffsetCount() const
    {
      return mOffsetCount;
    }

    virtual unsigned int getCount() const
    {
      return mCount;
    }

    virtual eDataType getDataType() const
    {
      return mDataType;
    }

    void* getVoidDataPtr()
    {
      if (nullptr != mMemoryBlockPtr)
      {
        if (mOffsetCount < mMemoryBlockPtr->size())
        {
          return reinterpret_cast<void*>(&mMemoryBlockPtr->operator[](mOffsetCount));
        }
      }
      return nullptr;
    }

    const void* getVoidDataPtr() const
    {
      if (nullptr != mMemoryBlockPtr)
      {
        if (mOffsetCount < mMemoryBlockPtr->size())
        {
          return reinterpret_cast<const void*>(&mMemoryBlockPtr->operator[](mOffsetCount));
        }
      }
      return nullptr;
    }

    void* getVoidPtrAt(unsigned int iIndex)
    {
      if (iIndex < mCount)
      {
        char* wHead = reinterpret_cast<char*>( getVoidDataPtr());

        if (nullptr != wHead)
        {
          void* wNewAddr = wHead + iIndex*getEnumDataTypeSize(mDataType);
          return wNewAddr;
        }
      }

      return nullptr;
    }

    const void* getVoidPtrAt(unsigned int iIndex) const
    {
      if (iIndex < mCount)
      {
        const char* wHead = reinterpret_cast<const char*>(getVoidDataPtr());

        if (nullptr != wHead)
        {
          const void* wNewAddr = wHead + iIndex*getEnumDataTypeSize(mDataType);
          return wNewAddr;
        }
      }

      return nullptr;
    }

    bool setVoidPtrAt(unsigned int iIndex, void* iValue)
    {
      if (nullptr != iValue)
      {
        void* wData = getVoidPtrAt(iIndex);

        if (nullptr != wData)
        {
          memcpy(wData, iValue,getEnumDataTypeSize(mDataType));
          return true;
        }
      }
      return false;
    }
  protected:

    std::vector<char>* mMemoryBlockPtr;
    unsigned int mOffsetCount;
    unsigned int mCount;
    const eDataType mDataType;
  };

  template<typename T, eDataType DType>
  class XSC_TMemoryBlockPtr : public XSC_VoidMemPtr
  {
  public:

    XSC_TMemoryBlockPtr()
      : XSC_VoidMemPtr(DType)
    {
    }

    XSC_TMemoryBlockPtr(std::vector<char>& iMemoryBlock, unsigned int iOffset, unsigned int iCount)
     : XSC_VoidMemPtr(iMemoryBlock, iOffset, iCount, DType)
    {
    }

    virtual void setValue(const std::string& iValue)
    {
      T* wDataPtr = getDataPtr();

      if ((nullptr != wDataPtr) && (nullptr != mMemoryBlockPtr))
      {
        unsigned int wEnd = mOffsetCount + mCount*sizeof(T);

        if (wEnd <= mMemoryBlockPtr->size())
        {
          XSC::StrTools::setStringToValue(iValue, wDataPtr, mCount);
          return;
        }
      }

      return;
    }

    virtual std::string getValue() const
    {
      const T* wDataPtr = getDataPtr();

      if ((nullptr != wDataPtr) && (nullptr != mMemoryBlockPtr))
      {
        unsigned int wEnd = mOffsetCount + mCount*sizeof(T);

        if (wEnd <= mMemoryBlockPtr->size())
        {
          return XSC::StrTools::printValueToString(wDataPtr,mCount);
        }
      }

      return "";
    }

    T* getDataPtr()
    {
      return reinterpret_cast<T*>(getVoidDataPtr());
    }

    const T* getDataPtr() const
    {
      return reinterpret_cast<const T*>(getVoidDataPtr());
    }

    operator T*()
    {
      return reinterpret_cast<T*>(getVoidDataPtr());
    }

    operator const T*() const
    {
      return reinterpret_cast<T*>(getVoidDataPtr());
    }

    T* getPtrAt(unsigned int iIndex)
    {
      if (iIndex < mCount)
      {
        return getDataPtr() + iIndex;
      }

      return nullptr;
    }

    const T* getPtrAt(unsigned int iIndex) const
    {
      if (iIndex < mCount)
      {
        return getDataPtr() + iIndex;
      }

      return nullptr;
    }

    bool setVoidPtrAt(unsigned int iIndex, T iValue)
    {
      T* wValuePtr = getPtrAt(iIndex);
      if (nullptr != wValuePtr)
      {
        *wValuePtr = iValue;
      }
      return false;
    }

  private:
  };
}

namespace XSC
{
  typedef XSC_TMemoryBlockPtr<char, eSInt8> XSC_SInt8MemPtr;
  typedef XSC_TMemoryBlockPtr<short, eSInt16> XSC_SInt16MemPtr;
  typedef XSC_TMemoryBlockPtr<int, eSInt32> XSC_SInt32MemPtr;

  typedef XSC_TMemoryBlockPtr<unsigned char, eUInt8> XSC_UInt8MemPtr;
  typedef XSC_TMemoryBlockPtr<unsigned short, eUInt16> XSC_UInt16MemPtr;
  typedef XSC_TMemoryBlockPtr<unsigned int, eUInt32> XSC_UInt32MemPtr;

  typedef XSC_TMemoryBlockPtr<float, eFloat32> XSC_Float32MemPtr;
  typedef XSC_TMemoryBlockPtr<double, eFloat64> XSC_Float64MemPtr;

}

namespace XSC
{
  class XSC_CStringMemPtr : public XSC_VoidMemPtr
  {
  public:

    XSC_CStringMemPtr()
      : XSC_VoidMemPtr(eString)
    {
    }

    XSC_CStringMemPtr(std::vector<char>& iMemoryBloc, unsigned int iOffset, unsigned int iCount)
      : XSC_VoidMemPtr(iMemoryBloc, iOffset, iCount, eString)
    {
    }

    virtual void setValue(const std::string& iValue)
    {
      char* wDataPtr = getDataPtr();

      if ((nullptr != wDataPtr) && (nullptr != mMemoryBlockPtr))
      {
        unsigned int wEnd = mOffsetCount + mCount*sizeof(char);

        if (wEnd <= mMemoryBlockPtr->size())
        {
          XSC::StrTools::setStringToCString(iValue, wDataPtr, mCount);
        }
      }

      return;
    }

    virtual std::string getValue() const
    {
      const char* wDataPtr = getDataPtr();

      if ((nullptr != wDataPtr) && (nullptr != mMemoryBlockPtr))
      {
        unsigned int wEnd = mOffsetCount + mCount*sizeof(char);

        if (wEnd <= mMemoryBlockPtr->size())
        {
          return XSC::StrTools::printCStringToString(wDataPtr, mCount);
        }
      }

      return "";
    }

    char * getDataPtr()
    {
      return reinterpret_cast<char *>(getVoidDataPtr());
    }

    const char * getDataPtr() const
    {
      return reinterpret_cast<const char *>(getVoidDataPtr());
    }

    operator char *()
    {
      return reinterpret_cast<char *>(getVoidDataPtr());
    }

    operator const char*() const
    {
      return reinterpret_cast<const char *>(getVoidDataPtr());
    }


    char* getPtrAt(unsigned int iIndex)
    {
      if (iIndex < mCount)
      {
        return getDataPtr() + iIndex;
      }

      return nullptr;
    }

    const char* getPtrAt(unsigned int iIndex) const
    {
      if (iIndex < mCount)
      {
        return getDataPtr() + iIndex;
      }

      return nullptr;
    }

    bool setVoidPtrAt(unsigned int iIndex, char iValue)
    {
      char* wValuePtr = getPtrAt(iIndex);
      if (nullptr != wValuePtr)
      {
        *wValuePtr = iValue;
      }
      return false;
    }

  private:

  };

  
}

#endif // !XSC_DATA_BLOCK_PTR_H
