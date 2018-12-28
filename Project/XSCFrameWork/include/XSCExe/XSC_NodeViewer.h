#ifndef XSC_NODE_VIEWER_H
#define XSC_NODE_VIEWER_H

#include "XSCBase/XSC_Base.hxx"
#include "XSCBase/XSC_TData.hxx"
#include "XSCBase/XSC_SafePointer.h"

#include <map>
#include <vector>
#include <string>


namespace XSC
{
  class XSC_NodeTree : public XSC_Object
  {
  public:
    XSC_NodeTree();
    XSC_NodeTree(std::string iName, XSC_Object& iObj);

    virtual void distroyedCallback(XSC_Void& iElement);

    void addBranch(std::string iName, XSC_Object& iObj);

    void deleteBranch(unsigned int iBranchIndex);
    void deleteBranch(const std::string& iBranchName);
    void deleteAllBranches();

    XSC_NodeTree* getBranchAtLocation(unsigned int& ioLocation);
    bool getBranchLocation(const std::string& iBranchName, unsigned int& oLocation);

    const std::string& getBranchName() const;

    XSC_Object* getBranchObj();
    const XSC_Object* getBranchObj() const;

    const std::vector<XSC_NodeTree*>& getbBranchList() const;
    void getBranchNameList(std::vector<std::string>& oBranchList) const;

    XSC_NodeTree* getBranch(const std::string& iBranchName);

    void getAllDecendentBranchNameList(std::vector<std::pair<std::string, int>>& oBranchList, unsigned int iParentIndex = 0) const;
    XSC_NodeTree* getDecendentBranch(const std::string& iBranchName);

  private:
    std::string mNodeName;
    XSC_SafeObjectPointer<XSC_Object, true> mObjRef;
    std::vector<XSC_NodeTree*> mBranchList;
  };

  class XSC_NodeViewer : public XSC_Object
  {
  public:

    XSC_NodeViewer();

    virtual void SClassSetup();
    
    void addScope(std::string& iScope, XSC_Object& iObject);

    bool getScopeList(std::vector<std::string>& oTable);

    bool getScopeContent(const std::string& iScope, std::vector<std::pair<std::string, int>> & oTable);

    bool getDataTable(const std::string& iScope, unsigned int iContentIndex, std::vector <std::pair<std::string, std::string> > & oTable);
  
    bool getObjectTable(const std::string& iScope, unsigned int iContentIndex, std::vector <std::string> & oTable);

    bool getArrayTable(const std::string& iScope, unsigned int iContentIndex, std::vector <std::pair<std::string, std::string> > & oTable);

    bool addObjectToTableOfContent(const std::string& iScope, unsigned int iContentIndex);

    bool removedObjectFromTableOfContent(const std::string& iScope, unsigned int iContentIndex);

    bool addObjectToTableOfContent(const std::string& iScope, unsigned int iContentIndex, std::string& iLabelName);

    bool removedObjectFromTableOfContent(const std::string& iScope, unsigned int iContentIndex, std::string& iLabelName);

    bool setContentDataValue(const std::string& iScope, unsigned int iContentIndex, std::string& iLabelName, std::string& iLabelValue);

    XSC_NodeTree* getScope(const std::string& iScope);

    XSC_NodeTree* getContentBranch(const std::string& iScope, unsigned int iContentIndex);

    void printNodeViewer(std::stringstream& oStream);

    void processCmdLine(const std::string& iCmdLine, std::stringstream& oStream);

    void setSelectedScope(const std::string& iScopeName);

    void setContentName(const std::string& iContentName);

    void setContentID(unsigned int iContentIndex);

  private:

    XSC_TData<std::string> mSelectedScope;
    XSC_TData<unsigned int> mSelectedContentIndex;

    XSC_NodeTree  mNodeRoot;
  };
}

#endif