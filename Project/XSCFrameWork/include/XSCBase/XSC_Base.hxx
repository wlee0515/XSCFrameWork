#ifndef  XSC_BASE_HXX
#define XSC_BASE_HXX

#include <string>
#include <set>
#include <map>
#include <deque>
#include <vector>
#include "XSCUtility/XSC_Logger.h"
#include "XSCBase/XSC_EnumTableProvider.h"

namespace XSC
{
  enum XSC_Action {
    eXSC_Setup,
    eXSC_Start,
    eXSC_Stop,
  };
}


ENUM_DEFINITION_START(XSC::XSC_Action)
ENUM_DEFINITION_DECLARE(XSC::eXSC_Setup, "Setup");
ENUM_DEFINITION_DECLARE(XSC::eXSC_Start, "Start");
ENUM_DEFINITION_DECLARE(XSC::eXSC_Stop, "Stop");
ENUM_DEFINITION_END

namespace XSC
{
  class XSC_Data;
  class XSC_Object;
  class XSC_Array;

  typedef void(*XSC_Cmd_Function_Ptr)(XSC_Object& iObject, const std::vector<std::string>& iArgVec);

  class XSC_Void
  {
  public:

    virtual ~XSC_Void();

    void subscribeToDistruction(XSC_Void& iElement);

    void unsubscribeFromDistruction(XSC_Void& iElement);

    virtual XSC_Data* getDataInterface();
    virtual const XSC_Data* getDataInterface() const;

    virtual XSC_Object* getObjectInterface();
    virtual const XSC_Object* getObjectInterface() const;

    virtual XSC_Array* getArrayInterface();
    virtual const XSC_Array* getArrayInterface() const;

  protected:

    void onDistruction();

    virtual void distroyedCallback(XSC_Void& iElement);

  private:
    std::set<XSC_Void*> mDistructionSubscribers;
  };

  class XSC_Data : public XSC_Void
  {
  public:

    virtual ~XSC_Data();

    virtual void setValue(const std::string& iValue);

    virtual std::string getValue() const;

    virtual XSC_Data* getDataInterface();
    virtual const XSC_Data* getDataInterface() const;
  };

  class XSC_Object : public XSC_Void
  {
  public:
    typedef std::vector<std::tuple<XSC_Data*, std::string, unsigned int>> TDataContainer;
    typedef std::vector<std::tuple<XSC_Object*, std::string, unsigned int>> TObjectContainer;

    virtual ~XSC_Object();

    virtual void addChild(std::string iName, XSC_Data& iDataRef, bool iCleanUp = false);
    virtual void addChild(std::string iName, XSC_Object& iObjectRef, bool iCleanUp = false);

    virtual XSC_Object* getObjectInterface();
    virtual const XSC_Object* getObjectInterface() const;

    virtual XSC_Data* getDataRef(const std::string& iDataName) const;

    virtual XSC_Object* getObjectRef(const std::string& iObjectName) const;

    virtual const TDataContainer& getDataRefList() const;
    TDataContainer& getDataRefList();

    virtual const TObjectContainer& getObjectRefList() const;
    virtual TObjectContainer& getObjectRefList();

    virtual bool setDataValue(const std::string& iDataName, const std::string& iValue);

    virtual void SClassSetup();
    virtual bool SClassStart();
    virtual bool SClassStop();

    bool processAction(const XSC_Action& iAction);
    
  protected:
    virtual void distroyedCallback(XSC_Void& iElement);

  private:

    TDataContainer mDataRefList;
    TObjectContainer mObjectRefList;
  };

  class XSC_Array : public XSC_Object
  {
  public:
    virtual XSC_Array* getArrayInterface();

    virtual const XSC_Array* getArrayInterface() const;

    virtual unsigned int getArraySize() = 0;

    virtual XSC_Void* getElementAt(unsigned int iIndex) = 0;

    virtual const XSC_Void* getElementAt(unsigned int iIndex) const = 0;

    virtual XSC_Void* pushBackNewElement() = 0;
    virtual XSC_Void* pushFrontNewElement() = 0;

    virtual void popLastElement() = 0;
    virtual void popFirstElement() = 0;

    virtual XSC_Void* insertElementAt(unsigned int iIndex) = 0;
    virtual void deleteElementAt(unsigned int iIndex) = 0;

    virtual void getVoidPtrArray(std::vector<XSC_Void*>& oArrayPtr) = 0;
    virtual void getVoidPtrArray(std::vector<const XSC_Void*>& oArrayPtr) const = 0;
  };

  template<typename T>
  class XSC_TArray : public XSC_Array
                  , public std::deque<T>
  {
  public:

    virtual unsigned int getArraySize()
    {
      return this->size();
    }

    virtual XSC_Void* getElementAt(unsigned int iIndex)
    {
      if (this->size() > iIndex)
      {
        return &this->operator[](iIndex);
      }
      return nullptr;
    }

    virtual const XSC_Void* getElementAt(unsigned int iIndex) const
    {
      if (this->size() > iIndex)
      {
        return &this->operator[](iIndex);
      }
      return nullptr;
    }

    virtual XSC_Void* pushBackNewElement()
    {
      this->push_back(T());
      XSC_Object* wObj = this->back().getObjectInterface();
      if (nullptr != wObj)
      {
        wObj->SClassSetup();
      }
      return &this->back();
    }

    virtual XSC_Void* pushFrontNewElement()
    {
      this->push_front(T());
      XSC_Object* wObj = this->front().getObjectInterface();
      if (nullptr != wObj)
      {
        wObj->SClassSetup();
      }
      return &this->front();
    }

    virtual void popLastElement()
    {
      this->pop_back();
    }

    virtual void popFirstElement()
    {
      this->pop_front();
    }

    virtual XSC_Void* insertElementAt(unsigned int iIndex)
    {
      if (iIndex < this->size())
      {
        this->insert(this->begin() + iIndex, T());
        XSC_Object* wObj = this->operator[](iIndex).getObjectInterface();
        if (nullptr != wObj)
        {
          wObj->SClassSetup();
        }
        return &this->operator[](iIndex);
      }
      return pushBackNewElement();
    }

    virtual void deleteElementAt(unsigned int iIndex)
    {
      if (iIndex < this->size())
      {
        this->erase(this->begin() + iIndex);
      }
    }

    virtual void getVoidPtrArray(std::vector<XSC_Void*>& oArrayPtr)
    {
      oArrayPtr.clear();
      for (typename std::deque<T>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        oArrayPtr.push_back(&(*wIt));
      }
    }

    virtual void getVoidPtrArray(std::vector<const XSC_Void*>& oArrayPtr) const
    {
      oArrayPtr.clear();
      for (typename std::deque<T>::const_iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        oArrayPtr.push_back(&(*wIt));
      }
    }

    virtual void SClassSetup()
    {
      for (typename std::deque<T>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        XSC_Object* wElement = wIt->getObjectInterface();
        if (nullptr != wElement)
        {
          wElement->SClassSetup();
        }
      }
      return XSC_Array::SClassSetup();
    }

    virtual bool SClassStart()
    {
      bool wState = true;
      for (typename std::deque<T>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        XSC_Object* wElement = wIt->getObjectInterface();
        if (nullptr != wElement)
        {
          wState = wState && wElement->SClassStart();
        }
      }
      return wState && XSC_Array::SClassStart();
    }

    virtual bool SClassStop()
    {
      bool wState = true;
      for (typename std::deque<T>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        XSC_Object* wElement = wIt->getObjectInterface();
        if (nullptr != wElement)
        {
          wState = wState && wElement->SClassStop();
        }
      }
      return wState && XSC_Array::SClassStop();
    }
  };


  template<typename T>
  class XSC_TArrayPtr : public XSC_Array
    , public std::deque<T*>
  {
  public:

    virtual ~XSC_TArrayPtr()
    {
      for (typename std::deque<T*>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        delete *wIt;
        *wIt = nullptr;
      }

      this->clear();
    }

    virtual unsigned int getArraySize()
    {
      return this->size();
    }

    virtual XSC_Void* getElementAt(unsigned int iIndex)
    {
      if (this->size() > iIndex)
      {
        return this->operator[](iIndex);
      }
      return nullptr;
    }

    virtual const XSC_Void* getElementAt(unsigned int iIndex) const
    {
      if (this->size() > iIndex)
      {
        return this->operator[](iIndex);
      }
      return nullptr;
    }

    virtual XSC_Void* pushBackNewElement()
    {
      this->push_back(new T());
      XSC_Object* wObj = this->back()->getObjectInterface();
      if (nullptr != wObj)
      {
        wObj->SClassSetup();
      }
      return this->back();
    }

    virtual XSC_Void* pushFrontNewElement()
    {
      this->push_front(new T());
      XSC_Object* wObj = this->front()->getObjectInterface();
      if (nullptr != wObj)
      {
        wObj->SClassSetup();
      }
      return this->front();
    }

    virtual void popLastElement()
    {
      delete this->back();
      this->pop_back();
    }

    virtual void popFirstElement()
    {
      delete this->front();
      this->pop_front();
    }

    virtual XSC_Void* insertElementAt(unsigned int iIndex)
    {
      if (iIndex < this->size())
      {
        this->insert(this->begin() + iIndex, new T());
        XSC_Object* wObj = this->operator[](iIndex)->getObjectInterface();
        if (nullptr != wObj)
        {
          wObj->SClassSetup();
        }
        return this->operator[](iIndex);
      }
      return pushBackNewElement();
    }

    virtual void deleteElementAt(unsigned int iIndex)
    {
      if (iIndex < this->size())
      {
        delete this->operator[](iIndex);
        this->operator[](iIndex) = nullptr;
        this->erase(this->begin() + iIndex);
      }
    }

    virtual void getVoidPtrArray(std::vector<XSC_Void*>& oArrayPtr)
    {
      oArrayPtr.clear();
      for (typename std::deque<T*>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        oArrayPtr.push_back(*wIt);
      }
    }

    virtual void getVoidPtrArray(std::vector<const XSC_Void*>& oArrayPtr) const
    {
      oArrayPtr.clear();
      for (typename std::deque<T*>::const_iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        oArrayPtr.push_back(*wIt);
      }
    }

    virtual void SClassSetup()
    {
      for (typename std::deque<T*>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        if (nullptr != *wIt)
        {
          XSC_Object* wElement = (*wIt)->getObjectInterface();
          if (nullptr != wElement)
          {
            wElement->SClassSetup();
          }
        }
      }
      return XSC_Array::SClassSetup();
    }

    virtual bool SClassStart()
    {
      bool wState = true;
      for (typename std::deque<T*>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        if (nullptr != *wIt)
        {
          XSC_Object* wElement = (*wIt)->getObjectInterface();
          if (nullptr != wElement)
          {
            wState = wState && wElement->SClassStart();
          }
        }
      }
      return wState && XSC_Array::SClassStart();
    }

    virtual bool SClassStop()
    {
      bool wState = true;
      for (typename std::deque<T*>::iterator wIt = this->begin(); wIt != this->end(); ++wIt)
      {
        if (nullptr != *wIt)
        {
          XSC_Object* wElement = (*wIt)->getObjectInterface();
          if (nullptr != wElement)
          {
            wState = wState && wElement->SClassStop();
          }
        }
      }
      return wState && XSC_Array::SClassStop();
    }
  };
}

#endif // ! XSC_BASE_H
