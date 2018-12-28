#include "XSCExe\XSC_NodeViewer.h"

#include <map>
#include <vector>
#include <string>


namespace XSC
{
  XSC_NodeTree::XSC_NodeTree()
    : mNodeName("NodeTree")
  {

  }

  XSC_NodeTree::XSC_NodeTree(std::string iName, XSC_Object& iObj)
    : mNodeName(iName)
  {
    mObjRef.setPtr(iObj);
  }

  void XSC_NodeTree::distroyedCallback(XSC_Void& iElement)
  {
    for (int wi = mBranchList.size() - 1; wi >= 0; --wi)
    {
      if (nullptr != mBranchList[wi])
      {
        XSC_Object* wBranch = mBranchList[wi]->mObjRef.getPtr();

        bool wDelete = false;
        if (nullptr == mBranchList[wi]->mObjRef.getPtr())
        {
          wDelete = true;
        }
        else if (&iElement == static_cast<XSC_Void*>(wBranch))
        {
          wDelete = true;
        }

        if (true == wDelete)
        {
          delete mBranchList[wi];
          mBranchList[wi] = nullptr;

          if (nullptr == mBranchList[wi])
          {
            for (unsigned int wj = wi; wj < mBranchList.size() - 1; ++wj)
            {
              mBranchList[wj] = mBranchList[wj + 1];
            }
          }

          mBranchList.pop_back();
        }
      }
    }

    XSC_Object::distroyedCallback(iElement);
  }

  void XSC_NodeTree::addBranch(std::string iName, XSC_Object& iObj)
  {
    for (unsigned int wi = 0; wi < mBranchList.size(); ++wi)
    {
      if (nullptr != mBranchList[wi])
      {
        if (iName == mBranchList[wi]->mNodeName)
        {
          return;
        }

        if (&iObj == mBranchList[wi]->mObjRef.getPtr())
        {
          return;
        }
      }
    }

    iObj.subscribeToDistruction(*this);
    XSC_NodeTree* wNewChild = new XSC_NodeTree(iName, iObj);
    wNewChild->SClassSetup();
    addChild("Branch", *wNewChild);
    mBranchList.push_back(wNewChild);
  }

  void XSC_NodeTree::deleteBranch(unsigned int iBranchIndex)
  {
    if (iBranchIndex < mBranchList.size())
    {
      XSC_NodeTree* wNode = mBranchList[iBranchIndex];
      for (unsigned int wi = iBranchIndex; wi < mBranchList.size() - 1; ++wi)
      {
        mBranchList[wi] = mBranchList[wi + 1];
      }
      mBranchList.pop_back();

      wNode->SClassStop();
      delete wNode;
      wNode = nullptr;
    }
  }

  void XSC_NodeTree::deleteBranch(const std::string& iBranchName)
  {
    if (0 != mBranchList.size())
    {
      for (unsigned int wi = 0; wi < mBranchList.size() - 1; ++wi)
      {
        if (nullptr != mBranchList[wi])
        {
          if (iBranchName == mBranchList[wi]->mNodeName)
          {
            return deleteBranch(wi);
          }
        }
      }

    }
  }

  void XSC_NodeTree::deleteAllBranches()
  {

    std::vector<XSC_NodeTree*> wEmptyBranchList;
    mBranchList.swap(wEmptyBranchList);

    for (unsigned int wi = 0; wi < wEmptyBranchList.size(); ++wi)
    {
      delete wEmptyBranchList[wi];
      wEmptyBranchList[wi] = nullptr;
    }
  }

  XSC_NodeTree* XSC_NodeTree::getBranchAtLocation(unsigned int& ioLocation)
  {
    if (ioLocation == 0)
    {
      return this;
    }
    else
    {
      --ioLocation;

      for (unsigned int wi = 0; wi < mBranchList.size(); ++wi)
      {
        if (nullptr != mBranchList[wi])
        {
          XSC_NodeTree* wBranch = mBranchList[wi]->getBranchAtLocation(ioLocation);
          if (nullptr != wBranch)
          {
            return wBranch;
          }
        }
      }
    }
    return nullptr;
  }

  bool XSC_NodeTree::getBranchLocation(const std::string& iBranchName, unsigned int& oLocation)
  {
    if (iBranchName == mNodeName)
    {
      return true;
    }

    ++oLocation;

    for (unsigned int wi = 0; wi < mBranchList.size(); ++wi)
    {
      if (nullptr != mBranchList[wi])
      {
        if (true == mBranchList[wi]->getBranchLocation(iBranchName, oLocation))
        {
          return true;
        }
      }
    }

    return false;
  }


  const std::string& XSC_NodeTree::getBranchName() const
  {
    return mNodeName;
  }

  XSC_Object* XSC_NodeTree::getBranchObj()
  {
    return mObjRef.getPtr();
  }

  const XSC_Object* XSC_NodeTree::getBranchObj() const
  {
    return mObjRef.getPtr();
  }

  const std::vector<XSC_NodeTree*>& XSC_NodeTree::getbBranchList() const
  {
    return mBranchList;
  }

  void XSC_NodeTree::getBranchNameList(std::vector<std::string>& oBranchList) const
  {
    for (unsigned int wi = 0; wi < mBranchList.size(); ++wi)
    {
      if (nullptr != mBranchList[wi])
      {
        oBranchList.push_back(mBranchList[wi]->mNodeName);
      }
    }
  }

  XSC_NodeTree* XSC_NodeTree::getBranch(const std::string& iBranchName)
  {
    for (unsigned int wi = 0; wi < mBranchList.size(); ++wi)
    {
      if (nullptr != mBranchList[wi])
      {
        if (iBranchName == mBranchList[wi]->mNodeName)
        {
          return mBranchList[wi];
        }
      }
    }

    return nullptr;
  }

  void XSC_NodeTree::getAllDecendentBranchNameList(std::vector<std::pair<std::string, int>>& oBranchList, unsigned int iParentIndex ) const
  {
    unsigned int wSelfIndex = oBranchList.size();
    oBranchList.push_back(std::make_pair(this->mNodeName, iParentIndex));

    for (unsigned int wi = 0; wi < mBranchList.size(); ++wi)
    {
      if (nullptr != mBranchList[wi])
      {
        mBranchList[wi]->getAllDecendentBranchNameList(oBranchList, wSelfIndex);
      }
    }
  }

  XSC_NodeTree* XSC_NodeTree::getDecendentBranch(const std::string& iBranchName)
  {
    if (mNodeName == iBranchName)
    {
      return this;
    }

    for (unsigned int wi = 0; wi < mBranchList.size(); ++wi)
    {
      if (nullptr != mBranchList[wi])
      {
        XSC_NodeTree* wReturn = mBranchList[wi]->getDecendentBranch(iBranchName);

        if (nullptr != wReturn)
        {
          return wReturn;
        }
      }
    }

    return nullptr;
  }


  XSC_NodeViewer::XSC_NodeViewer()
    : mSelectedScope("")
    , mSelectedContentIndex(0)
  {

  }

  void XSC_NodeViewer::SClassSetup()
  {
    addChild("SelectedScope", mSelectedScope);
    addChild("SelectedContentIndex", mSelectedContentIndex);
    addChild("NodeRoot", mNodeRoot);
    XSC_Object::SClassSetup();
  }

  void XSC_NodeViewer::addScope(std::string& iScope, XSC_Object& iObject)
  {
    mNodeRoot.addBranch(iScope, iObject);
  }

  bool XSC_NodeViewer::getScopeList(std::vector<std::string>& oTable)
  {
    oTable.clear();
    mNodeRoot.getBranchNameList(oTable);
    return true;
  }


  bool XSC_NodeViewer::getScopeContent(const std::string& iScope, std::vector<std::pair<std::string, int>> & oTable)
  {
    XSC_NodeTree* wActiveScope = getScope(iScope);

    if (nullptr != wActiveScope)
    {
      oTable.clear();
      wActiveScope->getAllDecendentBranchNameList(oTable, 0);
      return true;
    }

    return false;
  }

  bool XSC_NodeViewer::getDataTable(const std::string& iScope, unsigned int iContentIndex, std::vector <std::pair<std::string, std::string> > & oTable)
  {
    XSC_NodeTree* wBranch = getContentBranch(iScope, iContentIndex);
    if (nullptr != wBranch)
    {
      const XSC_Object* wObject = wBranch->getBranchObj();

      if (nullptr != wObject)
      {
        const TDataContainer& wLabelMap = wObject->getDataRefList();

        for (TDataContainer::const_iterator wIt = wLabelMap.begin(); wIt != wLabelMap.end(); ++wIt)
        {
          XSC_Data* wObjectPtr = std::get<0>(*wIt);

          if (nullptr != wObjectPtr)
          {
            oTable.push_back(std::make_pair(std::get<1>(*wIt), std::get<0>(*wIt)->getValue()));
          }
        }
        return true;
      }
    }
    return false;
  }

  bool XSC_NodeViewer::getObjectTable(const std::string& iScope, unsigned int iContentIndex, std::vector <std::string> & oTable)
  {
    XSC_NodeTree* wBranch = getContentBranch(iScope, iContentIndex);
    if (nullptr != wBranch)
    {
      const XSC_Object* wObject = wBranch->getBranchObj();

      if (nullptr != wObject)
      {

        const TObjectContainer& wLabelMap = wObject->getObjectRefList();

        for (TObjectContainer::const_iterator wIt = wLabelMap.begin(); wIt != wLabelMap.end(); ++wIt)
        {
          XSC_Object* wObjectPtr = std::get<0>(*wIt);

          if (nullptr != wObjectPtr)
          {
            oTable.push_back(std::get<1>(*wIt));
          }
        }
        return true;
      }
    }
    return false;
  }

  bool XSC_NodeViewer::getArrayTable(const std::string& iScope, unsigned int iContentIndex, std::vector <std::pair<std::string, std::string> > & oTable)
  {
    XSC_NodeTree* wBranch = getContentBranch(iScope, iContentIndex);
    if (nullptr != wBranch)
    {
      const XSC_Object* wObject = wBranch->getBranchObj();

      if (nullptr != wObject)
      {
        const XSC_Array* wArrayObj = wObject->getArrayInterface();

        if (nullptr != wArrayObj)
        {
          oTable.clear();

          std::vector<const XSC_Void*> wVoidPtrArray;
          wArrayObj->getVoidPtrArray(wVoidPtrArray);

          for (unsigned int wi = 0; wi < wVoidPtrArray.size(); ++wi)
          {
            if (nullptr != wVoidPtrArray[wi])
            {
              std::string wValue = "";

              const XSC_Data* wData = wVoidPtrArray[wi]->getDataInterface();

              if (nullptr != wData)
              {
                wValue = wData->getValue();
              }

              std::stringstream wName;
              wName << "Array_" << wi;

              oTable.push_back(std::make_pair(wName.str(), wValue));
            }
          }

          return true;

        }
      }
    }
    return false;
  }

  bool XSC_NodeViewer::addObjectToTableOfContent(const std::string& iScope, unsigned int iContentIndex)
  {
    XSC_NodeTree* wBranch = getContentBranch(iScope, iContentIndex);
    if (nullptr != wBranch)
    {
      XSC_Object* wObject = wBranch->getBranchObj();

      if (nullptr != wObject)
      {
        const TObjectContainer& wLabelMap = wObject->getObjectRefList();

        for (TObjectContainer::const_iterator wIt = wLabelMap.begin(); wIt != wLabelMap.end(); ++wIt)
        {
          XSC_Object* wObjectPtr = std::get<0>(*wIt);

          if (nullptr != wObjectPtr)
          {
            wBranch->addBranch(std::get<1>(*wIt), *wObjectPtr);
          }
        }

        XSC_Array* wArrayObj = wObject->getArrayInterface();

        if (nullptr != wArrayObj)
        {
          std::vector<XSC_Void*> wVoidPtrArray;
          wArrayObj->getVoidPtrArray(wVoidPtrArray);

          for (unsigned int wi = 0; wi < wVoidPtrArray.size(); ++wi)
          {
            if (nullptr != wVoidPtrArray[wi])
            {

              XSC_Object* wObjElement = wVoidPtrArray[wi]->getObjectInterface();

              if (nullptr != wObjElement)
              {

                std::stringstream wName;
                wName << "Array_" << wi;

                wBranch->addBranch(wName.str(), *wObjElement);
              }
              else
              {
                break;
              }
            }
          }

          return true;

        }
        return true;
      }
    }
    return false;

  }

  bool XSC_NodeViewer::removedObjectFromTableOfContent(const std::string& iScope, unsigned int iContentIndex)
  {
    XSC_NodeTree* wBranch = getContentBranch(iScope, iContentIndex);
    if (nullptr != wBranch)
    {
      const XSC_Object* wObject = wBranch->getBranchObj();

      if (nullptr != wObject)
      {
        wBranch->deleteAllBranches();
      }

      const XSC_Array* wArrayObj = wObject->getArrayInterface();

      if (nullptr != wArrayObj)
      {
        std::vector<const XSC_Void*> wVoidPtrArray;
        wArrayObj->getVoidPtrArray(wVoidPtrArray);

        for (unsigned int wi = 0; wi < wVoidPtrArray.size(); ++wi)
        {
          if (nullptr != wVoidPtrArray[wi])
          {

            const XSC_Object* wObjElement = wVoidPtrArray[wi]->getObjectInterface();

            if (nullptr != wObjElement)
            {
              std::stringstream wName;
              wName << "Array_" << wi;
              wBranch->deleteBranch(wName.str());
            }
            else
            {
              break;
            }
          }
        }
      }
      return true;
    }
    return false;
  }

  bool XSC_NodeViewer::addObjectToTableOfContent(const std::string& iScope, unsigned int iContentIndex, std::string& iLabelName)
  {
    XSC_NodeTree* wBranch = getContentBranch(iScope, iContentIndex);
    if (nullptr != wBranch)
    {
      XSC_Object* wObject = wBranch->getBranchObj();

      if (nullptr != wObject)
      {
        const TObjectContainer& wLabelMap = wObject->getObjectRefList();

        for (TObjectContainer::const_iterator wIt = wLabelMap.begin(); wIt != wLabelMap.end(); ++wIt)
        {
          XSC_Object* wObjectPtr = std::get<0>(*wIt);

          if (nullptr != wObjectPtr)
          {
            if (iLabelName == std::get<1>(*wIt))
            {
              wBranch->addBranch(std::get<1>(*wIt), *wObjectPtr);
            }
          }
        }

        XSC_Array* wArrayObj = wObject->getArrayInterface();

        if (nullptr != wArrayObj)
        {
          std::vector<XSC_Void*> wVoidPtrArray;
          wArrayObj->getVoidPtrArray(wVoidPtrArray);

          for (unsigned int wi = 0; wi < wVoidPtrArray.size(); ++wi)
          {
            if (nullptr != wVoidPtrArray[wi])
            {

              XSC_Object* wObjElement = wVoidPtrArray[wi]->getObjectInterface();

              if (nullptr != wObjElement)
              {

                std::stringstream wName;
                wName << "Array_" << wi;

                if (iLabelName == wName.str())
                {
                  wBranch->addBranch(wName.str(), *wObjElement);
                }
              }
              else
              {
                break;
              }
            }
          }
        }
        return true;
      }
    }

    return false;
  }

  bool XSC_NodeViewer::removedObjectFromTableOfContent(const std::string& iScope, unsigned int iContentIndex, std::string& iLabelName)
  {
    XSC_NodeTree* wBranch = getContentBranch(iScope, iContentIndex);
    if (nullptr != wBranch)
    {
      const XSC_Object* wObject = wBranch->getBranchObj();

      if (nullptr != wObject)
      {

        const TObjectContainer& wLabelMap = wObject->getObjectRefList();

        for (TObjectContainer::const_iterator wIt = wLabelMap.begin(); wIt != wLabelMap.end(); ++wIt)
        {
          XSC_Object* wObjectPtr = std::get<0>(*wIt);

          if (nullptr != wObjectPtr)
          {
            if (iLabelName == std::get<1>(*wIt))
            {
              wBranch->deleteBranch(iLabelName);
            }
          }
        }

        const XSC_Array* wArrayObj = wObject->getArrayInterface();

        if (nullptr != wArrayObj)
        {
          std::vector<const XSC_Void*> wVoidPtrArray;
          wArrayObj->getVoidPtrArray(wVoidPtrArray);

          for (unsigned int wi = 0; wi < wVoidPtrArray.size(); ++wi)
          {
            if (nullptr != wVoidPtrArray[wi])
            {

              const XSC_Object* wObjElement = wVoidPtrArray[wi]->getObjectInterface();

              if (nullptr != wObjElement)
              {

                std::stringstream wName;
                wName << "Array_" << wi;

                if (iLabelName == wName.str())
                {
                  wBranch->deleteBranch(iLabelName);
                }
              }
              else
              {
                break;
              }
            }
          }
        }
        return true;
      }
    }
    return false;
  }


  bool XSC_NodeViewer::setContentDataValue(const std::string& iScope, unsigned int iContentIndex, std::string& iLabelName, std::string& iLabelValue)
  {

    XSC_NodeTree* wBranch = getContentBranch(iScope, iContentIndex);
    if (nullptr != wBranch)
    {
      XSC_Object* wObject = wBranch->getBranchObj();

      if (nullptr != wObject)
      {
        TDataContainer& wLabelMap = wObject->getDataRefList();

        bool wValueSet = false;

        for (TDataContainer::const_iterator wIt = wLabelMap.begin(); wIt != wLabelMap.end(); ++wIt)
        {
          if (iLabelName == std::get<1>(*wIt))
          {
            if (nullptr != std::get<0>(*wIt))
            {
              std::get<0>(*wIt)->setValue(iLabelValue);
            }
            wValueSet = true;
          }
        }

        if (true == wValueSet)
        {
          return true;
        }

        XSC_Array* wArrayObj = wObject->getArrayInterface();

        if (nullptr != wArrayObj)
        {
          std::vector<XSC_Void*> wVoidPtrArray;
          wArrayObj->getVoidPtrArray(wVoidPtrArray);

          for (unsigned int wi = 0; wi < wVoidPtrArray.size(); ++wi)
          {
            if (nullptr != wVoidPtrArray[wi])
            {

              XSC_Data* wObjElement = wVoidPtrArray[wi]->getDataInterface();

              if (nullptr != wObjElement)
              {

                std::stringstream wName;
                wName << "Array_" << wi;

                if (iLabelName == wName.str())
                {
                  wObjElement->setValue(iLabelValue);
                  return true;
                }
              }
              else
              {
                break;
              }
            }
          }
        }
      }
    }

    return false;
  }

  XSC_NodeTree* XSC_NodeViewer::getScope(const std::string& iScope)
  {
    return mNodeRoot.getBranch(iScope);
  }

  XSC_NodeTree* XSC_NodeViewer::getContentBranch(const std::string& iScope, unsigned int iContentIndex)
  {
    XSC_NodeTree* wActiveScope = getScope(iScope);

    if (nullptr != wActiveScope)
    {
      return wActiveScope->getBranchAtLocation(iContentIndex);

    }
    return nullptr;
  }

  void XSC_NodeViewer::printNodeViewer(std::stringstream& oStream)
  {
    std::vector<std::string> wScopeList;
    getScopeList(wScopeList);

    if ("" == mSelectedScope)
    {
      if (0 != wScopeList.size())
      {
        mSelectedScope = wScopeList[0];
        mSelectedContentIndex = 0;
      }
    }

    std::vector<std::pair<std::string, int>> wTableOfContent;
    getScopeContent(mSelectedScope, wTableOfContent);

    std::vector <std::pair<std::string, std::string> > wDataTable;
    getDataTable(mSelectedScope, mSelectedContentIndex, wDataTable);

    std::vector <std::string> wObjectRefList;
    getObjectTable(mSelectedScope, mSelectedContentIndex, wObjectRefList);

    std::vector <std::pair<std::string, std::string> > wArrayElementList;
    getArrayTable(mSelectedScope, mSelectedContentIndex, wArrayElementList);

    if (0 != wScopeList.size())
    {
      oStream << "Scope List :" << std::endl;

      for (unsigned int wi = 0; wi < wScopeList.size(); ++wi)
      {
        if (mSelectedScope == wScopeList[wi])
        {
          oStream << "  sel->";
        }
        else
        {
          oStream << "       ";
        }

        oStream << wScopeList[wi] << std::endl;
      }

      oStream << std::endl;
    }

    if (0 != wTableOfContent.size())
    {
      oStream << "Content List :" << std::endl;
      for (unsigned int wi = 0; wi < wTableOfContent.size(); ++wi)
      {
        if (wi == mSelectedContentIndex)
        {
          oStream << "  sel->";
        }
        else
        {
          oStream << "       ";
        }

        oStream << wTableOfContent[wi].first << std::endl;
      }
      oStream << std::endl;
    }

    if (0 != wDataTable.size())
    {
      oStream << "Data Table :" << std::endl;
      for (unsigned int wi = 0; wi < wDataTable.size(); ++wi)
      {
        oStream << "       " << std::setw(25) << std::left << wDataTable[wi].first << wDataTable[wi].second << std::endl;
      }
      oStream << std::endl;
    }

    if (0 != wObjectRefList.size())
    {
      oStream << "Composed Object List :" << std::endl;

      for (unsigned int wi = 0; wi < wObjectRefList.size(); ++wi)
      {
        oStream << "       " << std::setw(20) << std::left << wObjectRefList[wi] << std::endl;
      }

      oStream << std::endl;
    }

    if (0 != wArrayElementList.size())
    {
      oStream << "Array Element Table :" << std::endl;
      for (unsigned int wi = 0; wi < wArrayElementList.size(); ++wi)
      {
        oStream << "       " << std::setw(20) << std::left << wArrayElementList[wi].first << wArrayElementList[wi].second << std::endl;
      }

      oStream << std::endl;
    }
  }

  void XSC_NodeViewer::processCmdLine(const std::string& iCmdLine, std::stringstream& oStream)
  {
    std::string wCommand = "";
    std::vector<std::string> wArguments;
    XSC::tools::splitCmdLine(iCmdLine, wCommand, wArguments);
    
    if ("" == wCommand)
    {
    }
    else if ("help" == wCommand)
    {
      oStream << "Command List:" << std::endl;
      oStream << "\"add\" : Add Object to Table of Content" << std::endl;
      oStream << "\"rmv\" : Remove Object to Table of Content" << std::endl;
      oStream << "\"scp\" : Select the current Scope" << std::endl;
      oStream << "\"ctx\" : Select the current Content" << std::endl;
      oStream << "\"see\" : Add and Select the Object to Table of Content" << std::endl;
      oStream << "\"set\" : Set the value of the Content Data" << std::endl;

    }
    else if ("add" == wCommand)
    {
      if (0 == wArguments.size())
      {
        addObjectToTableOfContent(mSelectedScope, mSelectedContentIndex);
      }
      else
      {
        for (unsigned int wi = 0; wi < wArguments.size(); ++wi)
        {
          addObjectToTableOfContent(mSelectedScope, mSelectedContentIndex, wArguments[wi]);
        }
      }
    }

    else if ("rmv" == wCommand)
    {
      if (0 == wArguments.size())
      {
        removedObjectFromTableOfContent(mSelectedScope, mSelectedContentIndex);
      }
      else
      {
        for (unsigned int wi = 0; wi < wArguments.size(); ++wi)
        {
          removedObjectFromTableOfContent(mSelectedScope, mSelectedContentIndex, wArguments[wi]);
        }
      }
    }

    else if ("scp" == wCommand)
    {
      if (1 == wArguments.size())
      {
        setSelectedScope(wArguments[0]);
      }
      else
      {
        oStream << "Error: Command \"ctx\" requires 0 arguments" << std::endl;
      }
    }

    else if ("ctx" == wCommand)
    {
      if (1 == wArguments.size())
      {
        setContentName(wArguments[0]);
      }
      else
      {
        oStream << "Error: Command \"ctx\" requires 1 arguments" << std::endl;
      }
    }
    else if ("see" == wCommand)
    {
      if (1 == wArguments.size())
      {
        addObjectToTableOfContent(mSelectedScope, mSelectedContentIndex, wArguments[0]);

        setContentName(wArguments[0]);
      }
      else
      {
        oStream << "Error: Command \"ctx\" requires 1 arguments" << std::endl;
      }

    }
    else if ("set" == wCommand)
    {
      if (2 <= wArguments.size())
      {
        setContentDataValue(mSelectedScope, mSelectedContentIndex, wArguments[0], wArguments[1]);
      }
      else
      {
        oStream << "Error: Command \"set\" requires 2 arguments" << std::endl;
      }
    }
    else
    {
      oStream << "Error: Command Not Recognized" << std::endl;
    }

    printNodeViewer(oStream);
  }

  void XSC_NodeViewer::setSelectedScope(const std::string& iScopeName)
  {
    if (iScopeName == mSelectedScope)
    {
      return;
    }

    XSC_NodeTree* wScope = mNodeRoot.getBranch(iScopeName);

    if (nullptr != wScope)
    {
      mSelectedScope = iScopeName;
    }
  }

  void XSC_NodeViewer::setContentName(const std::string& iContentName)
  {
    XSC_NodeTree* wScope = mNodeRoot.getBranch(mSelectedScope);

    if (nullptr != wScope)
    {
      XSC_NodeTree* wScope = mNodeRoot.getBranch(mSelectedScope);
      unsigned int wLocation = 0;

      if (true == wScope->getBranchLocation(iContentName, wLocation))
      {
        mSelectedContentIndex = wLocation;
      }
    }
  }

  void XSC_NodeViewer::setContentID(unsigned int iContentIndex)
  {
    if (iContentIndex == mSelectedContentIndex)
    {
      return;
    }

    XSC_NodeTree* wBranch = getContentBranch(mSelectedScope, iContentIndex);
    if (nullptr != wBranch)
    {
      mSelectedContentIndex = iContentIndex;
    }
  }

}