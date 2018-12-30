#ifndef XSC_JSON_REPLY_PROVIDER_HXX
#define XSC_JSON_REPLY_PROVIDER_HXX

#include "XSCServiceProvider/XSC_HttpProvider.h"
#include "BClass/BClass_JSON.h"
#include "XSCBase/XSC_SafePointer.hxx"

namespace XSC
{
  template< typename T_ReplyObject>
  class XSC_JsonReplyProvider : public XSC_HttpProvider::UriService
  {
  public:
    typedef void(*cProcessCmdFunction)(T_ReplyObject& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReply);

    virtual void SClassSetup()
    {
      XSC_HttpProvider::UriService::SClassSetup();
      addChild("ReplyObject", mReplyObject);
    }

    // XSC_HttpProvder::UriService Start
    virtual bool processHttpPOSTMethod(const std::string& iURI, const std::string& iRequest, std::string& oResponse)
    {
      BClassJSON wJSONObject;
      if (true == wJSONObject.parseJSONString(iRequest))
      {
        BClassJSON::JSONObjMap* wJSONMap_Ptr = wJSONObject.getJSONObjTypePtr_Map();
        if (nullptr != wJSONMap_Ptr)
        {
          BClassJSON::JSONObjMap::iterator wCmdListIt = wJSONMap_Ptr->find("CommandList");

          if (wCmdListIt != wJSONMap_Ptr->end())
          {
            BClassJSON::JSONObjArray* wJSONCommandListMap_Ptr = wCmdListIt->second.getJSONObjTypePtr_Array();
            if (nullptr != wJSONCommandListMap_Ptr)
            {
              for (BClassJSON::JSONObjArray::iterator wIt = wJSONCommandListMap_Ptr->begin(); wIt != wJSONCommandListMap_Ptr->end(); ++wIt)
              {
                BClassJSON::JSONObjMap* wCmdMap_Ptr = wIt->getJSONObjTypePtr_Map();
                if (nullptr != wCmdMap_Ptr)
                {
                  processJSONCmdReply(*wCmdMap_Ptr);
                }
              }
            }
          }
          else
          {
            processJSONCmdReply(*wJSONMap_Ptr);

          }

          oResponse = "";
          wJSONObject.generateJSONString(oResponse);
        }
      }
      return true;
    }
    // XSC_HttpProvder::UriService stop

    void setReplyObject(T_ReplyObject& iReplyClass)
    {
      mReplyObject.setPtr(iReplyClass);
    }

    void removeReplyObject()
    {
      mReplyObject.setNull();
    }

    bool addProcessCmdFunction(const std::string iCmdName, cProcessCmdFunction iFunction)
    {
      typename std::map<std::string, cProcessCmdFunction>::const_iterator wIt = mProcessCmdList.find(iCmdName);

      if (wIt != mProcessCmdList.end())
      {
        return false;
      }

      mProcessCmdList[iCmdName] = iFunction;
      return true;
    }

    void removeProcessCmdFunction(std::string iCmdName)
    {
      mProcessCmdList.erase(iCmdName);
    }

  private:

    void processJSONCmdReply(BClassJSON::JSONObjMap& ioCommand)
    {
      T_ReplyObject* wObject = mReplyObject.getPtr();

      if (nullptr != wObject)
      {
        if (0 != mProcessCmdList.size())
        {
          BClassJSON::JSONObjMap::iterator wCmdNameIt = ioCommand.find("cmd");

          if (wCmdNameIt != ioCommand.end())
          {
            BClassJSON::JSONObjString* wCmdNameString = wCmdNameIt->second.getJSONObjTypePtr_String();

            if (nullptr != wCmdNameString)
            {
              typename std::map<std::string, cProcessCmdFunction>::const_iterator wIt = mProcessCmdList.find(wCmdNameString->getData());

              if (wIt != mProcessCmdList.end())
              {
                BClassJSON::JSONObjMap& wArgumentMap = ioCommand["argument"].makeJSONMap();
                BClassJSON::JSONObjMap& wReturnMap = ioCommand["reply"].makeJSONMap();
                wIt->second(*wObject, wArgumentMap, wReturnMap);
              }
            }
          }
        }
      }
    }

    XSC_SafeObjectPointer<T_ReplyObject, true>  mReplyObject;
    std::map<std::string, cProcessCmdFunction> mProcessCmdList;
  };
}

#endif