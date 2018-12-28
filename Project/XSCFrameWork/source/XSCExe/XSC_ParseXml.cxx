#include "XSCExe/XSC_ParseXml.h"
#include "XSCBase/XSC_Base.hxx"
#include "XSCUtility/XSC_Logger.h"
#include "XSCBase/XSC_TypeLibrary.h"

#include <string>
#include <list>

namespace XSC
{
  namespace Parse
  {
    bool parseXMLFile(XSC::XSC_Object& iObjectRef, const std::string& iFileName, std::list<std::string>& iFileStack, const std::string& iSubNodeName)
    {
      tinyxml2::XMLDocument doc;
      doc.LoadFile(iFileName.c_str());

      if (doc.ErrorID() != 0)
      {
        LOG_ERROR("Error Processing File : " << iFileName.c_str());
        return false;
      }
      else
      {
        // do not continue if file is in file stack to prevent infinit looping
        std::list<std::string>::iterator witem = std::find(iFileStack.begin(), iFileStack.end(), iFileName);
        if (witem != iFileStack.end())
        {

          LOG_ERROR("Circular file referencing detected : " << iFileName.c_str());

          return false;
        }
      }

      LOG_TRACE("Processing File : " << iFileName.c_str());

      iFileStack.push_back(iFileName);


      tinyxml2::XMLNode* wNode = doc.FirstChild();

      if (nullptr != wNode)
      {
        if ("" != iSubNodeName)
        {
          tinyxml2::XMLNode* wNode2 = findXMLNodeWithName(*wNode, iSubNodeName);

          if (nullptr != wNode)
          {
            wNode = wNode2->FirstChild();
          }
          else
          {
            LOG_ERROR("Unable to find object [" << iSubNodeName << "] in file namae : " << iFileName.c_str());

          }
        }

        if (nullptr != wNode)
        {
          if (false == parseXMLNode(iObjectRef, *wNode, iFileStack))
          {
            return false;
          }
        }
      }

      std::list<std::string>::iterator witem = std::find(iFileStack.begin(), iFileStack.end(), iFileName);
      if (witem != iFileStack.end())
      {
        iFileStack.erase(witem);
      }

      return true;
    }

    bool parseXMLNode(XSC::XSC_Object& iObjectRef, tinyxml2::XMLNode& iXMLNode, std::list<std::string>& iFileStack)
    {
      tinyxml2::XMLNode* wNode = &iXMLNode;
      bool wState = true;

      while (nullptr != wNode)
      {
        tinyxml2::XMLElement* wElement = wNode->ToElement();

        if (wElement != nullptr)
        {
          std::string wCmd = "";
          std::string wText = "";

          if (nullptr != wElement->Name())
          {
            wCmd = wElement->Name();
          }

          if (nullptr != wElement->GetText())
          {
            wText = wElement->GetText();
          }

          std::map<std::string, std::string> wAttributeMap;

          const tinyxml2::XMLAttribute* wAttribute = wElement->FirstAttribute();
          std::string wChildName = "";
          std::string wTypename = "";

          while (wAttribute != nullptr)
          {
            if (wAttribute->Name() == XMLKeys::cChildName)
            {
              wChildName = wAttribute->Value();
            }
            else if (wAttribute->Name() == XMLKeys::cTypeName)
            {
              wTypename = wAttribute->Value();
            }
            else
            {
              wAttributeMap[wAttribute->Name()] = wAttribute->Value();
            }

            wAttribute = wAttribute->Next();
          }

          if (XMLKeys::cXMLFileTag == wCmd)
          {
            if ("" != wText)
            {
              std::string wAbsFileName;
              XSC::tools::resolveRelativePath(wAbsFileName, iFileStack.back(), wText);
              wState = wState && parseXMLFile(iObjectRef, wAbsFileName, iFileStack, wChildName);
            }
            else
            {
              std::map<std::string, std::string>::iterator wIt = wAttributeMap.find(XMLKeys::cFileReferenceTag);

              if (wIt != wAttributeMap.end())
              {
                std::string wAbsFileName;
                XSC::tools::resolveRelativePath(wAbsFileName, iFileStack.back(), wIt->second);
                wState = wState && parseXMLFile(iObjectRef, wAbsFileName, iFileStack, wChildName);
              }
              else
              {
                wState = false;
              }
            }
          }
          else if ((XMLKeys::cCreateTag == wCmd) || (XMLKeys::cClassDefTag == wCmd))
          {
            if ("" == wChildName)
            {
              LOG_ERROR("Object name not defined : " << wCmd);
              return false;
            }

            if (XMLKeys::cCreateTag == wCmd)
            {
              if ("" == wTypename)
              {
                LOG_ERROR("Type name not defined : " << wCmd << " name : " << wChildName);
                return false;
              }

              //create new Obj
              XSC_Void* wNewChild = XSC_TypeLibrary::CreateObject(wTypename);
              if (nullptr != wNewChild)
              {
                XSC_Data* wNewChildDataPtr = wNewChild->getDataInterface();
                if (nullptr != wNewChildDataPtr)
                {
                  iObjectRef.addChild(wChildName, *wNewChildDataPtr, true);
                }
                else
                {
                  XSC_Object* wNewChildObjPtr = wNewChild->getObjectInterface();
                  if (nullptr != wNewChildObjPtr)
                  {
                    wNewChildObjPtr->SClassSetup();
                    iObjectRef.addChild(wChildName, *wNewChildObjPtr, true);
                  }
                }
              }
              else
              {
                LOG_ERROR("Type name not valid : " << wCmd << " name : " << wChildName << " typename : " << wTypename);
                return false;
              }
            }

            XSC_Data* wChildDataPtr = iObjectRef.getDataRef(wChildName);
            if (nullptr != wChildDataPtr)
            {
              if ("" != wText)
              {
                wChildDataPtr->setValue(wText);
              }
              else
              {
                std::map<std::string, std::string>::iterator wValueIt = wAttributeMap.find(XMLKeys::cValueTag);

                if (wAttributeMap.end() != wValueIt)
                {
                  wChildDataPtr->setValue(wValueIt->second);
                }
              }
            }
            else
            {
              XSC_Object* wChildObjPtr = iObjectRef.getObjectRef(wChildName);
              if (nullptr != wChildObjPtr)
              {
                for (std::map<std::string, std::string>::iterator wIt = wAttributeMap.begin(); wIt != wAttributeMap.end(); ++wIt)
                {
                  wChildObjPtr->setDataValue(wIt->first, wIt->second);
                }

                tinyxml2::XMLNode* wChildNode = wNode->FirstChild();
                if (nullptr != wChildNode)
                {
                  if (false == parseXMLNode(*wChildObjPtr, *wChildNode, iFileStack))
                  {
                    return false;
                  }
                }
              }
            }
          }
          else if (XMLKeys::cArrayDefTag == wCmd)
          {
            XSC_Array* wArray = iObjectRef.getArrayInterface();

            if (nullptr != wArray)
            {
              XSC_Void* wElement = wArray->pushBackNewElement();

              if (nullptr != wElement)
              {
                XSC_Data* wChildDataPtr = wElement->getDataInterface();
                if (nullptr != wChildDataPtr)
                {
                  if ("" != wText)
                  {
                    wChildDataPtr->setValue(wText);
                  }
                  else
                  {
                    std::map<std::string, std::string>::iterator wValueIt = wAttributeMap.find(XMLKeys::cValueTag);

                    if (wAttributeMap.end() != wValueIt)
                    {
                      wChildDataPtr->setValue(wValueIt->second);
                    }
                  }
                }
                else
                {
                  XSC_Object* wChildObjPtr = wElement->getObjectInterface();
                  if (nullptr != wChildObjPtr)
                  {
                    for (std::map<std::string, std::string>::iterator wIt = wAttributeMap.begin(); wIt != wAttributeMap.end(); ++wIt)
                    {
                      wChildObjPtr->setDataValue(wIt->first, wIt->second);
                    }

                    tinyxml2::XMLNode* wChildNode = wNode->FirstChild();
                    if (nullptr != wChildNode)
                    {
                      if (false == parseXMLNode(*wChildObjPtr, *wChildNode, iFileStack))
                      {
                        return false;
                      }
                    }
                  }
                }
              }
            }
          }
          else {

            tinyxml2::XMLNode* wChildNode = wNode->FirstChild();
            if (nullptr != wChildNode)
            {
              XSC_Object* wChildObjPtr = iObjectRef.getObjectRef(wChildName);
              if (nullptr != wChildObjPtr)
              {
                if (false == parseXMLNode(*wChildObjPtr, *wChildNode, iFileStack))
                {
                  return false;
                }
              }
              else
              {
                if (false == parseXMLNode(iObjectRef, *wChildNode, iFileStack))
                {
                  return false;
                }
              }
            }
          }
        }

        wNode = wNode->NextSibling();
      }

      return true;
    }

    tinyxml2::XMLNode* findXMLNodeWithName(tinyxml2::XMLNode& iXMLNode, const std::string& iSubNodeName)
    {

      tinyxml2::XMLNode* wNode = &iXMLNode;
      bool wState = true;

      while (nullptr != wNode)
      {

        tinyxml2::XMLElement* wElement = wNode->ToElement();

        if (wElement != nullptr)
        {
          std::string wCmd = "";
          std::string wText = "";

          if (nullptr != wElement->Name())
          {
            wCmd = wElement->Name();
          }

          if (nullptr != wElement->GetText())
          {
            wText = wElement->GetText();
          }

          std::map<std::string, std::string> wAttributeMap;

          const tinyxml2::XMLAttribute* wAttribute = wElement->FirstAttribute();
          std::string wChildName = "";

          while (wAttribute != nullptr)
          {
            if (wAttribute->Name() == XMLKeys::cChildName)
            {
              wChildName = wAttribute->Value();
            }

            wAttribute = wAttribute->Next();
          }

          if (iSubNodeName == wChildName)
          {
            return wNode;
          }
          else
          {
            tinyxml2::XMLNode* wChildNode = wNode->FirstChild();
            if (nullptr != wChildNode)
            {
              tinyxml2::XMLNode* wSearchNode = findXMLNodeWithName(*wChildNode, iSubNodeName);

              if (nullptr != wSearchNode)
              {
                return wSearchNode;
              }
            }
          }
        }

        wNode = wNode->NextSibling();
      }
      return nullptr;
    }
  }
}