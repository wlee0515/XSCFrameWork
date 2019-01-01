#include "XSCBase/XSC_Base.hxx"
#include "XSCUtility/XSC_Logger.h"

#include <string>
#include <set>
#include <map>
#include <deque>
#include <vector>

namespace XSC
{
  XSC_Void::~XSC_Void()
  {
    onDistruction();
  }

  void XSC_Void::subscribeToDistruction(XSC_Void& iElement)
  {
    mDistructionSubscribers.insert(&iElement);
    iElement.mDistructionSubscribers.insert(this);
  }

  void XSC_Void::unsubscribeFromDistruction(XSC_Void& iElement)
  {
    mDistructionSubscribers.erase(&iElement);
    iElement.mDistructionSubscribers.erase(this);
  }

  XSC_Data* XSC_Void::getDataInterface()
  {
    return nullptr;
  }

  const XSC_Data* XSC_Void::getDataInterface() const
  {
    return nullptr;
  }

  XSC_Object* XSC_Void::getObjectInterface()
  {
    return nullptr;
  }

  const XSC_Object* XSC_Void::getObjectInterface() const
  {
    return nullptr;
  }

  XSC_Array* XSC_Void::getArrayInterface()
  {
    return nullptr;
  }

  const XSC_Array* XSC_Void::getArrayInterface() const
  {
    return nullptr;
  }

  void XSC_Void::onDistruction()
  {
    for (std::set<XSC_Void*>::iterator wIt = mDistructionSubscribers.begin(); wIt != mDistructionSubscribers.end(); ++wIt)
    {
      (*wIt)->distroyedCallback(*this);
    }
    mDistructionSubscribers.clear();
  }

  void XSC_Void::distroyedCallback(XSC_Void& iElement)
  {
    mDistructionSubscribers.erase(&iElement);
  }


  XSC_Data::~XSC_Data()
  {
    onDistruction();
  }

  void XSC_Data::setValue(const std::string& iValue)
  {
    return;
  }

  std::string XSC_Data::getValue() const
  {
    return "";
  }

  XSC_Data* XSC_Data::getDataInterface()
  {
    return this;
  }

  const XSC_Data* XSC_Data::getDataInterface() const
  {
    return this;
  }

  XSC_Object::~XSC_Object()
  {
    TDataContainer wTempDataContainer;
    wTempDataContainer.swap(mDataRefList);
    for (TDataContainer::iterator wIt = wTempDataContainer.begin(); wIt != wTempDataContainer.end(); ++wIt)
    {
      if (1 == std::get<2>(*wIt))
      {
        delete std::get<0>(*wIt);
        std::get<0>(*wIt) = nullptr;
      }
    }

    TObjectContainer wTempObjectContainer;
    wTempObjectContainer.swap(mObjectRefList);
    for (TObjectContainer::iterator wIt = wTempObjectContainer.begin(); wIt != wTempObjectContainer.end(); ++wIt)
    {
      if (1 == std::get<2>(*wIt))
      {
        delete std::get<0>(*wIt);
        std::get<0>(*wIt) = nullptr;
      }
    }

    onDistruction();
  }

  void XSC_Object::addChild(std::string iName, XSC_Data& iDataRef, bool iCleanUp)
  {
    XSC_Data* wRef = getDataRef(iName);

    if (nullptr == wRef)
    {
      if (true == iCleanUp)
      {
        mDataRefList.push_back(std::make_tuple(&iDataRef, iName, 1));
      }
      else
      {
        mDataRefList.push_back(std::make_tuple(&iDataRef, iName, 0));
      }
      iDataRef.subscribeToDistruction(*this);
      return;
    }

    LOG_ERROR("Name [" << iName << "] is already used");
  }

  void XSC_Object::addChild(std::string iName, XSC_Object& iObjectRef, bool iCleanUp)
  {
    XSC_Object* wRef = getObjectRef(iName);

    if (nullptr == wRef)
    {
      if (true == iCleanUp)
      {
        mObjectRefList.push_back(std::make_tuple(&iObjectRef, iName, 1));
      }
      else
      {
        mObjectRefList.push_back(std::make_tuple(&iObjectRef, iName, 0));
      }
      iObjectRef.subscribeToDistruction(*this);
      return;
    }

    LOG_ERROR("Name [" << iName << "] is already used");
  }

  XSC_Object* XSC_Object::getObjectInterface()
  {
    return this;
  }

  const XSC_Object* XSC_Object::getObjectInterface() const
  {
    return this;
  }


  XSC_Data* XSC_Object::getDataRef(const std::string& iDataName) const
  {
    for (XSC_Object::TDataContainer::const_iterator wIt = mDataRefList.begin(); wIt != mDataRefList.end(); ++wIt)
    {
      if (iDataName == std::get<1>(*wIt))
      {
        return std::get<0>(*wIt);
      }
    }
    return nullptr;
  }

  XSC_Object* XSC_Object::getObjectRef(const std::string& iObjectName) const
  {
    for (XSC_Object::TObjectContainer::const_iterator wIt = mObjectRefList.begin(); wIt != mObjectRefList.end(); ++wIt)
    {
      if (iObjectName == std::get<1>(*wIt))
      {
        return std::get<0>(*wIt);
      }
    }
    return nullptr;
  }

  const XSC_Object::TDataContainer& XSC_Object::getDataRefList() const
  {
    return mDataRefList;
  }

  XSC_Object::TDataContainer& XSC_Object::getDataRefList()
  {
    return mDataRefList;
  }

  const XSC_Object::TObjectContainer& XSC_Object::getObjectRefList() const
  {
    return mObjectRefList;
  }

  XSC_Object::TObjectContainer& XSC_Object::getObjectRefList()
  {
    return mObjectRefList;
  }

  bool XSC_Object::setDataValue(const std::string& iDataName, const std::string& iValue)
  {
    XSC_Data* wDataRef = getDataRef(iDataName);

    if (nullptr != wDataRef)
    {
      wDataRef->setValue(iValue);
      return true;
    }

    return false;
  }

  void XSC_Object::SClassSetup()
  {
    for (XSC_Object::TObjectContainer::iterator wIt = mObjectRefList.begin(); wIt != mObjectRefList.end(); ++wIt)
    {
      XSC_Object* wObjectPtr = std::get<0>(*wIt);

      if (nullptr != wObjectPtr)
      {
        wObjectPtr->SClassSetup();
      }
    }
  }

  bool XSC_Object::SClassStart()
  {
    bool wState = true;
    for (XSC_Object::TObjectContainer::iterator wIt = mObjectRefList.begin(); wIt != mObjectRefList.end(); ++wIt)
    {
      XSC_Object* wObjectPtr = std::get<0>(*wIt);

      if (nullptr != wObjectPtr)
      {
        wState = wState && wObjectPtr->SClassStart();
      }
    }

    return wState;
  }

  bool XSC_Object::SClassStop()
  {
    bool wState = true;
    for (XSC_Object::TObjectContainer::iterator wIt = mObjectRefList.begin(); wIt != mObjectRefList.end(); ++wIt)
    {
      XSC_Object* wObjectPtr = std::get<0>(*wIt);

      if (nullptr != wObjectPtr)
      {
        std::cout << "Stopping Object [" << std::get<1>(*wIt) << "] - start" << std::endl;
        wState = wState && wObjectPtr->SClassStop();
        std::cout << "Stopping Object [" << std::get<1>(*wIt) << "] - end" << std::endl;
      }
    }

    return wState;
  }

  bool XSC_Object::processAction(const XSC_Action& iAction) {

    switch (iAction)
    {
    case eXSC_Setup:
      SClassSetup();
      return true;
    case eXSC_Start:
      return SClassStart();
    case eXSC_Stop:
      return SClassStop();
    default:
      break;
    }
    return false;
  }

  void XSC_Object::distroyedCallback(XSC_Void& iElement)
  {
    XSC_Data* wDataPtr = (&iElement)->getDataInterface();
    if (nullptr != wDataPtr)
    {
      for (XSC_Object::TDataContainer::iterator wIt = mDataRefList.begin(); wIt != mDataRefList.end(); )
      {
        if (wDataPtr == std::get<0>(*wIt))
        {
          wIt = mDataRefList.erase(wIt);
        }
        else
        {
          ++wIt;
        }
      }
    }

    XSC_Object* wObjectPtr = (&iElement)->getObjectInterface();
    if (nullptr != wObjectPtr)
    {
      for (XSC_Object::TObjectContainer::iterator wIt = mObjectRefList.begin(); wIt != mObjectRefList.end(); )
      {
        if (wObjectPtr == std::get<0>(*wIt))
        {
          wIt = mObjectRefList.erase(wIt);
        }
        else
        {
          ++wIt;
        }
      }
    }

    return XSC_Void::distroyedCallback(iElement);
  }


  XSC_Array* XSC_Array::getArrayInterface()
  {
    return this;
  }

  const XSC_Array* XSC_Array::getArrayInterface() const
  {
    return this;
  }

}

