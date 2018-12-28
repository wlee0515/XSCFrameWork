#ifndef XSC_SAFE_POINTER_H
#define XSC_SAFE_POINTER_H

#include "XSC_Base.hxx"

namespace XSC
{

  template<typename T, bool ReadOnly>
  class XSC_SafeDataPointer : public XSC_Data
  {
  public:
    XSC_SafeDataPointer() : mDataPtr(nullptr)
    {

    }

    void setPtr(T& iXSCData)
    {
      iXSCData.subscribeToDistruction(*this);
      mDataPtr = &iXSCData;
    }

    void setNull()
    {
      if (nullptr != mDataPtr)
      {
        mDataPtr->unsubscribeFromDistruction(*this);
      }
      mDataPtr = nullptr;
    }

    T* getPtr()
    {
      return mDataPtr;
    }

    const T* getPtr() const
    {
      return mDataPtr;
    }

    bool isNull()
    {
      return  nullptr == mDataPtr;
    }

  protected:
    virtual void distroyedCallback(XSC_Void& iElement)
    {
      XSC_Void* wVoidInterface = static_cast<XSC_Void*>(mDataPtr);

      if (&iElement == wVoidInterface)
      {
        mDataPtr = nullptr;
      }

      return XSC_Object::distroyedCallback(iElement);
    }

  public:
    virtual XSC_Data* getDataInterface() {
      if (nullptr != mDataPtr)
      {
        return mDataPtr->getDataInterface();
      }
      return nullptr;
    }

    virtual const XSC_Data* getDataInterface() const {
      if (nullptr != mDataPtr)
      {
        return mDataPtr->getDataInterface();
      }
      return nullptr;
    }

    virtual XSC_Object* getObjectInterface() {
      if (nullptr != mDataPtr)
      {
        return mDataPtr->getObjectInterface();
      }
      return nullptr;
    }

    virtual const XSC_Object* getObjectInterface() const {
      if (nullptr != mDataPtr)
      {
        return mDataPtr->getObjectInterface();
      }
      return nullptr;
    }

    virtual XSC_Array* getArrayInterface() {
      if (nullptr != mDataPtr)
      {
        return mDataPtr->getArrayInterface();
      }
      return nullptr;
    }

    virtual const XSC_Array* getArrayInterface() const {
      if (nullptr != mDataPtr)
      {
        return mDataPtr->getArrayInterface();
      }
      return nullptr;
    }

    virtual void setValue(const std::string& iValue)
    {
      if (false == ReadOnly)
      {
        if (nullptr != mDataPtr)
        {
          mDataPtr->setValue(iValue);
        }
      }
      return;
    }

    virtual std::string getValue() const
    {
      if (nullptr != mDataPtr)
      {
        return mDataPtr->getValue();
      }

      return "";
    }


  private:

    T* mDataPtr;
  };


  template<typename T, bool ReadOnly>
  class XSC_SafeObjectPointer : public XSC_Object
  {
  public:
    XSC_SafeObjectPointer() : mObjPtr(nullptr)
    {

    }

    void setPtr(T& iXSCObj)
    {
      iXSCObj.subscribeToDistruction(*this);
      mObjPtr = &iXSCObj;
    }

    void setNull()
    {
      if (nullptr != mObjPtr)
      {
        mObjPtr->unsubscribeFromDistruction(*this);
      }
      mObjPtr = nullptr;
    }

    T* getPtr()
    {
      return mObjPtr;
    }

    const T* getPtr() const
    {
      return mObjPtr;
    }

    bool isNull()
    {
      return  nullptr == mObjPtr;
    }

  protected:
    virtual void distroyedCallback(XSC_Void& iElement)
    {
      XSC_Void* wVoidInterface = static_cast<XSC_Void*>(mObjPtr);

      if (&iElement == wVoidInterface)
      {
        mObjPtr = nullptr;
      }

      return XSC_Object::distroyedCallback(iElement);
    }

  public:

    virtual XSC_Data* getDataInterface() {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getDataInterface();
      }
      return nullptr;
    }

    virtual const XSC_Data* getDataInterface() const {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getDataInterface();
      }
      return nullptr;
    }

    virtual XSC_Object* getObjectInterface() {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getObjectInterface();
      }
      return nullptr;
    }

    virtual const XSC_Object* getObjectInterface() const {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getObjectInterface();
      }
      return nullptr;
    }

    virtual XSC_Array* getArrayInterface() {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getArrayInterface();
      }
      return nullptr;
    }

    virtual const XSC_Array* getArrayInterface() const {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getArrayInterface();
      }
      return nullptr;
    }


    virtual XSC_Data* getDataRef(const std::string& iDataName) const
    {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getDataRef(iDataName);
      }

      return XSC_Object::getDataRef(iDataName);
    }

    virtual XSC_Object* getObjectRef(const std::string& iObjectName) const
    {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getObjectRef(iObjectName);
      }

      return XSC_Object::getObjectRef(iObjectName);
    }

    virtual const TDataContainer& getDataRefList() const
    {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getDataRefList();
      }

      return XSC_Object::getDataRefList();
    }

    virtual TDataContainer& getDataRefList()
    {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getDataRefList();
      }

      return XSC_Object::getDataRefList();
    }

    virtual const TObjectContainer& getObjectRefList() const
    {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getObjectRefList();
      }

      return XSC_Object::getObjectRefList();
    }

    virtual TObjectContainer& getObjectRefList()
    {
      if (nullptr != mObjPtr)
      {
        return mObjPtr->getObjectRefList();
      }

      return XSC_Object::getObjectRefList();
    }

    virtual bool setDataValue(const std::string& iDataName, const std::string& iValue)
    {
      if (false == ReadOnly)
      {
        if (nullptr != mObjPtr)
        {
          return mObjPtr->setDataValue(iDataName, iValue);
        }
      }
      return XSC_Object::setDataValue(iDataName, iValue);
    }

    virtual void SClassSetup()
    {
      if (nullptr != mObjPtr)
      {
        if (false == ReadOnly)
        {
          mObjPtr->SClassSetup();
        }
      }

      return XSC_Object::SClassSetup();
    }

    virtual bool SClassStart()
    {
      if (nullptr != mObjPtr)
      {
        if (false == ReadOnly)
        {
          mObjPtr->SClassStart();
        }
      }

      return XSC_Object::SClassStart();
    }

    virtual bool SClassStop()
    {
      if (nullptr != mObjPtr)
      {
        if (false == ReadOnly)
        {
          mObjPtr->SClassStop();
        }
      }

      return XSC_Object::SClassStop();
    }

  private:

    T* mObjPtr;
  };

}

#endif // !XSC_SAFE_POINTER_H
