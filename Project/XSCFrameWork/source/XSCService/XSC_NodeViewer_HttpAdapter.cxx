#include "XSCService/XSC_NodeViewer_HttpAdapter.h"

#include <vector>

namespace XSC
{
  ADD_TYPE_FACTORY_WITH_NAME(XSC_NodeViewerHttpAdapter, "NodeViewerHttpAdapter");

  XSC_NodeViewerHttpAdapter::XSC_NodeViewerHttpAdapter()
  {
  }

  void XSC_NodeViewerHttpAdapter::SClassSetup()
  {
    setReplyObject(*this);

    addChild("NodeViewer", mNodeViewer);

    addProcessCmdFunction("SendRequest", processSendRequest);
    addProcessCmdFunction("GetDisplay", processGetDisplay);

    addProcessCmdFunction("ScopeList", processScopeList);
    addProcessCmdFunction("ContentList", processContentList);
    addProcessCmdFunction("ContentInfo", processInfoList);

    XSC::XSC_JsonReplyProvider<XSC_NodeViewerHttpAdapter>::SClassSetup();
  }

  bool XSC_NodeViewerHttpAdapter::SClassStart()
  {
    XSC::XSC_Program& wProgram = XSC::XSC_Program::getGlobalProgram();
    mNodeViewer.addScope(std::string("System"), wProgram);

    return XSC::XSC_JsonReplyProvider<XSC_NodeViewerHttpAdapter>::SClassStart();;
  }

  void XSC_NodeViewerHttpAdapter::processSendRequest(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn)
  {
    BClassJSON::JSONObjMap::const_iterator wRequestLine = iCmdArguments.find("RequestLine");

    if (wRequestLine != iCmdArguments.end())
    {
      const BClassJSON::JSONObjString* wRequestLineString = wRequestLine->second.getJSONObjTypePtr_String();

      if (nullptr != wRequestLineString)
      {

        BClassJSON::JSONObjMap::const_iterator wScopeIt = iCmdArguments.find("Scope");
        BClassJSON::JSONObjMap::const_iterator wContentIt = iCmdArguments.find("ContentID");

        if ((wScopeIt != iCmdArguments.end()) && (wContentIt != iCmdArguments.end()))
        {
          const BClassJSON::JSONObjString* wScopeString = wScopeIt->second.getJSONObjTypePtr_String();
          const BClassJSON::JSONObjNumber* wContentID = wContentIt->second.getJSONObjTypePtr_Number();

          if ((nullptr != wScopeString) && (nullptr != wContentID))
          {
            iReplyClass.mNodeViewer.setSelectedScope(wScopeString->getData());
            iReplyClass.mNodeViewer.setContentID(static_cast<unsigned int>(wContentID->getData()));
          }
        }

        std::stringstream wStream;
        iReplyClass.mNodeViewer.processCmdLine(wRequestLineString->getData(), wStream);

        BClassJSON::JSONObjString& wDisplayLine = oReturn["CmdNodeDisplay"].makeJSONString();
        wDisplayLine.getData() = wStream.str();
      }
    }
  }

  void XSC_NodeViewerHttpAdapter::processGetDisplay(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn)
  {
    std::stringstream wStream;
    iReplyClass.mNodeViewer.printNodeViewer(wStream);

    BClassJSON::JSONObjString& wDisplayLine = oReturn["CmdNodeDisplay"].makeJSONString();
    wDisplayLine.getData() = wStream.str();
  }

  void XSC_NodeViewerHttpAdapter::processScopeList(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn)
  {
    std::vector<std::string> wScopeList;
    iReplyClass.mNodeViewer.getScopeList(wScopeList);

    BClassJSON::JSONObjArray& wScopeListReturn = oReturn["ScopeList"].makeJSONArray();

    if (0 != wScopeList.size())
    {
      wScopeListReturn.resize(wScopeList.size());

      for (unsigned int wi = 0; (wi < wScopeList.size()) && (wi < wScopeListReturn.size()); ++wi)
      {
        wScopeListReturn[wi].makeJSONString().getData() = wScopeList[wi];
      }
    }
  }

  void XSC_NodeViewerHttpAdapter::processContentList(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn)
  {
    BClassJSON::JSONObjMap::const_iterator wScopeIt = iCmdArguments.find("Scope");

    if (wScopeIt != iCmdArguments.end())
    {
      const BClassJSON::JSONObjString* wScopeString = wScopeIt->second.getJSONObjTypePtr_String();

      if (nullptr != wScopeString)
      {
        std::vector<std::pair<std::string, int>> wTableOfContent;
        iReplyClass.mNodeViewer.getScopeContent(wScopeString->getData(), wTableOfContent);

        if (0 != wTableOfContent.size())
        {
          BClassJSON::JSONObjArray& wCtxListReturn = oReturn["ContentList"].makeJSONArray();

          wCtxListReturn.resize(wTableOfContent.size());

          std::vector<int> wParentMap;

          for (unsigned int wi = 0; (wi < wTableOfContent.size()) && (wi < wCtxListReturn.size()); ++wi)
          {
            wParentMap.push_back(wTableOfContent[wi].second);

            int wParentIndex = wi;

            std::vector<int> wStack;
            for (unsigned int wj = wi; wj != 0; --wj)
            {
              wParentIndex = wParentMap[wParentIndex];
              wStack.push_back(wParentIndex);
              if (0 >= wParentIndex)
              {
                break;
              }

            }

            std::stringstream wIndexStr;
            while (0 != wStack.size())
            {
              wIndexStr << "[" << wStack.back() << "]";
              wStack.pop_back();
            }

            wCtxListReturn[wi].makeJSONString().getData() = wIndexStr.str() + wTableOfContent[wi].first;
          }
        }
      }
    }
  }

  void XSC_NodeViewerHttpAdapter::processInfoList(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn)
  {
    BClassJSON::JSONObjMap::const_iterator wScopeIt = iCmdArguments.find("Scope");
    BClassJSON::JSONObjMap::const_iterator wContentIt = iCmdArguments.find("ContentID");

    if ((wScopeIt != iCmdArguments.end()) && (wContentIt != iCmdArguments.end()))
    {
      const BClassJSON::JSONObjString* wScopeString = wScopeIt->second.getJSONObjTypePtr_String();
      const BClassJSON::JSONObjNumber* wContentID = wContentIt->second.getJSONObjTypePtr_Number();

      if ((nullptr != wScopeString) && (nullptr != wContentID))
      {
        const std::string wScope = wScopeString->getData();
        int wConetent = static_cast<int>(wContentID->getData());

        std::vector <std::pair<std::string, std::string> > wDataTable;
        iReplyClass.mNodeViewer.getDataTable(wScope, wConetent, wDataTable);

        std::vector <std::string> wObjectRefList;
        iReplyClass.mNodeViewer.getObjectTable(wScope, wConetent, wObjectRefList);

        std::vector <std::pair<std::string, std::string> > wArrayElementList;
        iReplyClass.mNodeViewer.getArrayTable(wScope, wConetent, wArrayElementList);

        if (0 != wDataTable.size())
        {
          BClassJSON::JSONObjArray& wDataReturn = oReturn["DataTable"].makeJSONArray();

          wDataReturn.resize(wDataTable.size());

          for (unsigned int wi = 0; (wi < wDataReturn.size()) && (wi < wDataTable.size()); ++wi)
          {
            wDataReturn[wi].makeJSONMap()["Label"].makeJSONString().getData() = wDataTable[wi].first;
            wDataReturn[wi].makeJSONMap()["Value"].makeJSONString().getData() = wDataTable[wi].second;
          }
        }

        if (0 != wObjectRefList.size())
        {
          BClassJSON::JSONObjArray& wObjListReturn = oReturn["ObjectTable"].makeJSONArray();

          wObjListReturn.resize(wObjectRefList.size());

          for (unsigned int wi = 0; (wi < wObjectRefList.size()) && (wi < wObjListReturn.size()); ++wi)
          {
            wObjListReturn[wi].makeJSONString().getData() = wObjectRefList[wi];
          }
        }

        if (0 != wArrayElementList.size())
        {
          BClassJSON::JSONObjArray& wArrayReturn = oReturn["ArrayTable"].makeJSONArray();

          wArrayReturn.resize(wArrayElementList.size());

          for (unsigned int wi = 0; (wi < wArrayReturn.size()) && (wi < wArrayElementList.size()); ++wi)
          {
            wArrayReturn[wi].makeJSONMap()["Label"].makeJSONString().getData() = wArrayElementList[wi].first;
            wArrayReturn[wi].makeJSONMap()["Value"].makeJSONString().getData() = wArrayElementList[wi].second;
          }
        }
      }
    }
  }
}
