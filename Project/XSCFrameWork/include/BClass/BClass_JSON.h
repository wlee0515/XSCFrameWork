#ifndef BCLASS_JSON_H
#define BCLASS_JSON_H
#include <string>
#include <sstream>
#include <iostream>

#include <map>
#include <vector>
#include "BClass_BracketTree.h"
#include "BClass_TypenameMapper.h"
#include "XSCUtility/XSC_StringTools.h"

namespace XSC
{
  enum eJSONValueType
  {
    eJSONNull,
    eJSONString,
    eJSONNumber,
    eJSONBool,
    eJSONMap,
    eJSONArray,
  };

  class BClassJSON
  {
  public:
    //-----------------------------
    // JSON Object Forward Type Declaration
    class JSONObjBase;
    class JSONObjString;
    class JSONObjNumber;
    class JSONObjBool;
    class JSONObjMap;
    class JSONObjArray;

    //-----------------------------
    // JSON Object Types
    class JSONObjBase
    {
    public:

      virtual ~JSONObjBase() {}
      virtual eJSONValueType getValueType() const
      {
        return eJSONNull;
      }

      virtual void generateJSONString(std::string& oString) const
      {
        oString = "null";
      }

      JSONObjBase*   getTypePtr_Base() { if (eJSONNull == getValueType()) return reinterpret_cast<JSONObjBase*>(this); return nullptr; }
      JSONObjString*   getTypePtr_String() { if (eJSONString == getValueType()) return reinterpret_cast<JSONObjString*>(this); return nullptr; }
      JSONObjNumber*   getTypePtr_Number() { if (eJSONNumber == getValueType()) return reinterpret_cast<JSONObjNumber*>(this); return nullptr; }
      JSONObjBool*   getTypePtr_Bool() { if (eJSONBool == getValueType()) return reinterpret_cast<JSONObjBool*>(this); return nullptr; }
      JSONObjMap*   getTypePtr_Map() { if (eJSONMap == getValueType()) return reinterpret_cast<JSONObjMap*>(this); return nullptr; }
      JSONObjArray*   getTypePtr_Array() { if (eJSONArray == getValueType()) return reinterpret_cast<JSONObjArray*>(this); return nullptr; }

      const JSONObjBase*   getTypePtr_Base() const { if (eJSONNull == getValueType()) return reinterpret_cast<const JSONObjBase*>(this); return nullptr; }
      const JSONObjString*   getTypePtr_String() const { if (eJSONString == getValueType()) return reinterpret_cast<const JSONObjString*>(this); return nullptr; }
      const JSONObjNumber*   getTypePtr_Number() const { if (eJSONNumber == getValueType()) return reinterpret_cast<const JSONObjNumber*>(this); return nullptr; }
      const JSONObjBool*   getTypePtr_Bool() const { if (eJSONBool == getValueType()) return reinterpret_cast<const JSONObjBool*>(this); return nullptr; }
      const JSONObjMap*   getTypePtr_Map() const { if (eJSONMap == getValueType()) return reinterpret_cast<const JSONObjMap*>(this); return nullptr; }
      const JSONObjArray*   getTypePtr_Array() const { if (eJSONArray == getValueType()) return reinterpret_cast<const JSONObjArray*>(this); return nullptr; }

    };

    class JSONObjString : public JSONObjBase
                        , public virtual BClassTypenameMapper<std::string>
    {
    public:
      virtual eJSONValueType getValueType() const
      {
        return eJSONString;
      }

      virtual void generateJSONString(std::string& oString) const
      {
        std::string wOutput = XSC::StrTools::replaceAllSubString(getData(), "\\" , "\\\\");

        oString = "\"";
        oString += wOutput;
        oString += "\"";
      }

    };

    class JSONObjNumber : public JSONObjBase
                        , public virtual BClassTypenameMapper<double>
    {
    public:
      virtual eJSONValueType getValueType() const
      {
        return eJSONNumber;
      }

      virtual void generateJSONString(std::string& oString) const
      {
        std::stringstream oStream;
        oStream << getData();
        oString = oStream.str();
      }

    };

    class JSONObjBool : public JSONObjBase
                      , public virtual BClassTypenameMapper<bool>
    {
    public:
      virtual eJSONValueType getValueType() const
      {
        return eJSONBool;
      }

      virtual void generateJSONString(std::string& oString) const
      {
        if (true == getData())
        {
          oString = "true";
        }
        else
        {
          oString = "false";
        }
      }
    };

    class JSONObjMap : public JSONObjBase
                     , public virtual std::map<std::string, BClassJSON>
    {
    public:
      virtual eJSONValueType getValueType() const
      {
        return eJSONMap;
      }

      virtual void generateJSONString(std::string& oString) const
      {
        oString = "{";
        
        JSONObjMap::const_iterator wIt = this->begin();

        if (wIt != this->end())
        {
          std::string wTemp = "";
          wIt->second.generateJSONString(wTemp);

          oString += "\"";
          oString += wIt->first;
          oString += "\":";
          oString += wTemp;
          ++wIt;
        }

        for (; wIt != this->end(); ++wIt)
        {
          std::string wTemp = "";
          wIt->second.generateJSONString(wTemp);

          oString += ",\"";
          oString += wIt->first;
          oString += "\":";
          oString += wTemp;
        }
        oString += "}";
      }

      const BClassJSON* findChild(const std::string& iName) const
      {
        BClassJSON::JSONObjMap::const_iterator wChild = this->find(iName);

        if (this->end() != wChild)
        {
          return &(wChild->second);
        }
        return nullptr;
      }

      BClassJSON* findChild(const std::string& iName)
      {
        BClassJSON::JSONObjMap::iterator wChild = this->find(iName);

        if (this->end() != wChild)
        {
          return &(wChild->second);
        }
        return nullptr;
      }

      bool getChildValue(const std::string& iName, bool& oValue) const
      {
        const BClassJSON* wChild = findChild(iName);
        if (nullptr != wChild)
        {
          const BClassJSON::JSONObjBool* wType_Ptr = wChild->getJSONObjTypePtr_Bool();
          if (nullptr != wType_Ptr)
          {
            oValue = wType_Ptr->getData();
            return true;
          }
        }
        return false;
      }

      bool getChildValue(const std::string& iName, int& oValue) const
      {
        const BClassJSON* wChild = findChild(iName);
        if (nullptr != wChild)
        {
          const BClassJSON::JSONObjNumber* wType_Ptr = wChild->getJSONObjTypePtr_Number();
          if (nullptr != wType_Ptr)
          {
            double wValue = wType_Ptr->getData();
            if (0 <= wValue)
            {
              oValue = static_cast<int>(wValue + 0.5);
            }
            else
            {
              oValue = static_cast<int>(wValue - 0.5);
            }
            return true;
          }
        }
        return false;
      }

      bool getChildValue(const std::string& iName, double& oValue) const
      {
        const BClassJSON* wChild = findChild(iName);
        if (nullptr != wChild)
        {
          const BClassJSON::JSONObjNumber* wType_Ptr = wChild->getJSONObjTypePtr_Number();
          if (nullptr != wType_Ptr)
          {
            oValue = wType_Ptr->getData();
            return true;
          }
        }
        return false;
      }

      bool getChildValue(const std::string& iName, std::string& oValue) const
      {
        const BClassJSON* wChild = findChild(iName);
        if (nullptr != wChild)
        {
          const BClassJSON::JSONObjString* wType_Ptr = wChild->getJSONObjTypePtr_String();
          if (nullptr != wType_Ptr)
          {
            oValue = wType_Ptr->getData();
            return true;
          }
        }
        return false;
      }

    };

    class JSONObjArray : public JSONObjBase
                       , public virtual std::vector<BClassJSON>
    {
    public:
      virtual eJSONValueType getValueType() const
      {
        return eJSONArray;
      }

      virtual void generateJSONString(std::string& oString) const
      {
        oString = "[";

        JSONObjArray::const_iterator wIt = this->begin();

        if (wIt != this->end())
        {
          std::string wTemp = "";
          wIt->generateJSONString(wTemp);
          oString += wTemp;
          ++wIt;
        }

        for (; wIt != this->end(); ++wIt)
        {
          std::string wTemp = "";
          wIt->generateJSONString(wTemp);
          oString += ",";
          oString += wTemp;
        }
        oString += "]";
      }
    };

    //-----------------------------
    // JSON Global Function

    BClassJSON() : mJSONObjPtr(nullptr)
    {

    }

    virtual ~BClassJSON()
    {
      if (nullptr != mJSONObjPtr)
      {
        delete mJSONObjPtr;
      }
      mJSONObjPtr = nullptr;
    }

    eJSONValueType getValueType() const
    {
      getJSONObjRef().getValueType();
    }

    JSONObjBase& getJSONObjRef()
    {
      if (nullptr == mJSONObjPtr)
      {
        mJSONObjPtr = new JSONObjBase;
      }
      return *mJSONObjPtr;
    }

    const JSONObjBase& getJSONObjRef() const
    {
      if (nullptr == mJSONObjPtr)
      {
        const_cast<JSONObjBase*>(mJSONObjPtr) = new JSONObjBase;
      }
      return *mJSONObjPtr;
    }

    //-------------------------------
    // JSON Type Conversion

    JSONObjString*   getJSONObjTypePtr_String() { return getJSONObjRef().getTypePtr_String(); }
    JSONObjNumber*   getJSONObjTypePtr_Number() { return getJSONObjRef().getTypePtr_Number(); }
    JSONObjBool*   getJSONObjTypePtr_Bool() { return getJSONObjRef().getTypePtr_Bool(); }
    JSONObjMap*   getJSONObjTypePtr_Map() { return getJSONObjRef().getTypePtr_Map(); }
    JSONObjArray*   getJSONObjTypePtr_Array() { return getJSONObjRef().getTypePtr_Array(); }


    const JSONObjString*   getJSONObjTypePtr_String() const { return getJSONObjRef().getTypePtr_String(); }
    const JSONObjNumber*   getJSONObjTypePtr_Number() const { return getJSONObjRef().getTypePtr_Number(); }
    const JSONObjBool*   getJSONObjTypePtr_Bool() const { return getJSONObjRef().getTypePtr_Bool(); }
    const JSONObjMap*   getJSONObjTypePtr_Map() const { return getJSONObjRef().getTypePtr_Map(); }
    const JSONObjArray*   getJSONObjTypePtr_Array() const { return getJSONObjRef().getTypePtr_Array(); }

    //-------------------------------
    // JSON Type Creation

    JSONObjBase& makeJSONNull()
    {
      if (nullptr != mJSONObjPtr)
      {
        if (eJSONNull != mJSONObjPtr->getValueType())
        {
          delete mJSONObjPtr;
          mJSONObjPtr = nullptr;
        }
      }

      if (nullptr == mJSONObjPtr)
      {
        mJSONObjPtr = new JSONObjBase();
      }

      return *(mJSONObjPtr->getTypePtr_Base());
    }

    JSONObjString&  makeJSONString()
    {
      if (nullptr != mJSONObjPtr)
      {
        if (eJSONString != mJSONObjPtr->getValueType())
        {
          delete mJSONObjPtr;
          mJSONObjPtr = nullptr;
        }
      }

      if (nullptr == mJSONObjPtr)
      {
        mJSONObjPtr = new JSONObjString();
      }

      return *(mJSONObjPtr->getTypePtr_String());
    }

    JSONObjNumber&  makeJSONNumber()
    {
      if (nullptr != mJSONObjPtr)
      {
        if (eJSONNumber != mJSONObjPtr->getValueType())
        {
          delete mJSONObjPtr;
          mJSONObjPtr = nullptr;
        }
      }

      if (nullptr == mJSONObjPtr)
      {
        mJSONObjPtr = new JSONObjNumber();
      }

      return *(mJSONObjPtr->getTypePtr_Number());
    }

    JSONObjBool&  makeJSONBool()
    {
      if (nullptr != mJSONObjPtr)
      {
        if (eJSONBool != mJSONObjPtr->getValueType())
        {
          delete mJSONObjPtr;
          mJSONObjPtr = nullptr;
        }
      }

      if (nullptr == mJSONObjPtr)
      {
        mJSONObjPtr = new JSONObjBool();
      }

      return *(mJSONObjPtr->getTypePtr_Bool());
    }


    JSONObjMap&  makeJSONMap()
    {
      if (nullptr != mJSONObjPtr)
      {
        if (eJSONMap != mJSONObjPtr->getValueType())
        {
          delete mJSONObjPtr;
          mJSONObjPtr = nullptr;
        }
      }

      if (nullptr == mJSONObjPtr)
      {
        mJSONObjPtr = new JSONObjMap();
      }
      return *(mJSONObjPtr->getTypePtr_Map());
    }

    JSONObjArray&   makeJSONArray()
    {
      if (nullptr != mJSONObjPtr)
      {
        if (eJSONArray != mJSONObjPtr->getValueType())
        {
          delete mJSONObjPtr;
          mJSONObjPtr = nullptr;
        }
      }

      if (nullptr == mJSONObjPtr)
      {
        mJSONObjPtr = new JSONObjArray();
      }

      return *(mJSONObjPtr->getTypePtr_Array());
    }

    JSONObjBase& setValueType(const eJSONValueType& iType)
    {
      switch (iType)
      {
      case eJSONString :
        return makeJSONString();
      case eJSONNumber:
        return makeJSONNumber();
      case eJSONBool:
        return makeJSONBool();
      case eJSONMap:
        return makeJSONMap();
      case eJSONArray:
        return makeJSONArray();
      case eJSONNull:
      default:
        return makeJSONNull();
      }

      return getJSONObjRef();
    }

    //---------------------------------------------------------------
    //Parsing Functions

    virtual void generateJSONString(std::string& oString) const
    {
      getJSONObjRef().generateJSONString(oString);
    }

    const std::vector<BracketChar>& getBracketTypes() const
    {
      static std::vector<BracketChar> gBrackets;
      if (gBrackets.size() == 0)
      {
        gBrackets.push_back(BracketChar('{', ',', '}'));
        gBrackets.push_back(BracketChar('[', ',', ']'));
        gBrackets.push_back(BracketChar('\"', '\"'));
        gBrackets.push_back(BracketChar('\'', '\''));
        gBrackets.push_back(BracketChar(':'));

      }
      return gBrackets;
    }

    bool parseJSONString(const std::string& iString)
    {
      BracketTree wTree;

      if (true == getBracketTree(wTree, getBracketTypes(), iString))
      {
        if (wTree.mIndex == -1)
        {
          if (wTree.mElements.size() != 0)
          {
            return this->parseBracketTree(wTree.mElements[0]);
          }
        }
        return this->parseBracketTree(wTree);
      }
      return false;
    }

    virtual bool parseBracketTree(const BracketTree& iBracketTree)
    {
      std::string wTreeContent = StrTools::removeBracingWhiteSpace(iBracketTree.mContent);
      if ((iBracketTree.mIndex == -1) && (iBracketTree.mElements.size() != 0))
      {
        for (const BracketTree& wElement : iBracketTree.mElements)
        {
          if (wElement.mType != BracketChar())
          {
            return parseBracketTree(wElement);
          }
        }
      }

      eJSONValueType wType = eJSONNull;
      if (iBracketTree.mType == BracketChar())
      {
        if ((wTreeContent == "NULL") || (wTreeContent == "null"))
        {
          wType = eJSONNull;
          return true;
        }
        else if ((wTreeContent == "TRUE") || (wTreeContent == "true"))
        {
          wType = eJSONBool;
          setValueType(wType);
          if (nullptr != mJSONObjPtr)
          {
            JSONObjBool* wJSONBool = mJSONObjPtr->getTypePtr_Bool();
            if (nullptr != wJSONBool)
            {
              wJSONBool->getData() = true;
              return true;
            }
          }
        }
        else if ((wTreeContent == "FALSE") || (wTreeContent == "false"))
        {
          wType = eJSONBool;
          setValueType(wType);
          if (nullptr != mJSONObjPtr)
          {
            JSONObjBool* wJSONBool = mJSONObjPtr->getTypePtr_Bool();
            if (nullptr != wJSONBool)
            {
              wJSONBool->getData() = false;
              return true;
            }
          }
        }

        std::stringstream wIsNumber(wTreeContent);
        double wNumberValue;
        wIsNumber >> wNumberValue;

        if ((false == wIsNumber.fail()))
        {
          wType = eJSONNumber;
          setValueType(wType);
          if (nullptr != mJSONObjPtr)
          {
            JSONObjNumber* wJSONNumber = mJSONObjPtr->getTypePtr_Number();
            if (nullptr != wJSONNumber)
            {
              wJSONNumber->getData() = wNumberValue;
              return true;
            }
          }
        }
      }
      else if (iBracketTree.mType == BracketChar('{', ',', '}'))
      {
        wType = eJSONMap;
        setValueType(wType);
        if (nullptr != mJSONObjPtr)
        {
          JSONObjMap* wJSONMap = mJSONObjPtr->getTypePtr_Map();
          if (nullptr != wJSONMap)
          {
            JSONObjMap& wJSONMap_Ref = *wJSONMap;
            wJSONMap_Ref.clear();

            for (const BracketTree& wElement : iBracketTree.mElements)
            {
              if (wElement.mElements.size() == 0)
              {
                return false;
              }

              unsigned int iColonPos = static_cast<unsigned int>(wElement.mContent.find(':'));
              if (iColonPos == wElement.mContent.npos)
              {
                return false;
              }

              unsigned int wAssignmentCut = wElement.mLocation.mStart + iColonPos;

              const BracketTree* wSubTreeName = nullptr;
              std::vector<const BracketTree*> wSubTreeValue;

              for (const BracketTree& iSubTree : wElement.mElements)
              {
                if (iSubTree.mLocation.mStart < wAssignmentCut)
                {
                  if ((iSubTree.mType == BracketChar('\"', '\"')) || (iSubTree.mType == BracketChar('\'', '\'')))
                  {
                    wSubTreeName = &iSubTree;
                  }
                }
                else
                {
                  wSubTreeValue.push_back(&iSubTree);
                }
              }

              if ((wSubTreeName == NULL))
              {
                return false;
              }

              if (wSubTreeValue.size() == 1)
              {
                wJSONMap_Ref[wSubTreeName->mContent].parseBracketTree(*wSubTreeValue[0]);
              }
              else
              {
                for (const BracketTree* wSub : wSubTreeValue)
                {
                  if (wSub->mType != BracketChar())
                  {
                    wJSONMap_Ref[wSubTreeName->mContent].parseBracketTree(*wSub);
                    break;
                  }
                }
              }
            }
            return true;
          }
        }
      }
      else if (iBracketTree.mType == BracketChar('[', ',', ']'))
      {
        wType = eJSONArray;
        setValueType(wType);
        if (nullptr != mJSONObjPtr)
        {
          JSONObjArray* wJSONArray = mJSONObjPtr->getTypePtr_Array();
          if (nullptr != wJSONArray)
          {
            JSONObjArray& wJSONArray_Ref = *wJSONArray;
            wJSONArray_Ref.clear();
            wJSONArray_Ref.reserve(iBracketTree.mElements.size());

            for (const BracketTree& wElement : iBracketTree.mElements)
            {
              if (wElement.mType != BracketChar())
              {
                if (0 != wElement.mElements.size())
                {
                  wJSONArray_Ref.push_back(BClassJSON());
                  wJSONArray_Ref.back().parseBracketTree(wElement.mElements[0]);
                }
              }
            }
            return true;
          }
        }

      }
      else if ((iBracketTree.mType == BracketChar('\'', '\'')) || (iBracketTree.mType == BracketChar('\"', '\"')))
      {
        wType = eJSONString;
        setValueType(wType);
        if (nullptr != mJSONObjPtr)
        {
          JSONObjString* wJSONString = mJSONObjPtr->getTypePtr_String();
          if (nullptr != wJSONString)
          {
            wJSONString->getData() = wTreeContent;
            return true;
          }
        }
      }

      setValueType(wType);
      return false;
    }

  private:
    JSONObjBase* mJSONObjPtr;
  };
}

#endif
