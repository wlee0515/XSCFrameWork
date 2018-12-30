#ifndef XSC_NODEVIEWER_HTTP_ADAPTER_H
#define XSC_NODEVIEWER_HTTP_ADAPTER_H

#include "XSCBase/XSC_Common.hxx"
#include "XSCServiceProvider/XSC_JsonReplyProvider.hxx"
#include "XSCExe/XSC_NodeViewer.h"
#include "XSCExe/XSC_Program.h"

namespace XSC
{
  class XSC_NodeViewerHttpAdapter : public XSC::XSC_JsonReplyProvider<XSC_NodeViewerHttpAdapter>
  {
  public:
    XSC_NodeViewerHttpAdapter();
    virtual void SClassSetup();
    virtual bool SClassStart();

    static void processSendRequest(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn);
    static void processGetDisplay(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn);
    static void processScopeList(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn);
    static void processContentList(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn);
    static void processInfoList(XSC_NodeViewerHttpAdapter& iReplyClass, const BClassJSON::JSONObjMap& iCmdArguments, BClassJSON::JSONObjMap& oReturn);

  private:
    XSC_NodeViewer mNodeViewer;    
  };

}

#endif