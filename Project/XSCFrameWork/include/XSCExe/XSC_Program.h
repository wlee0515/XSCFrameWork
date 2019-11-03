#ifndef XSC_PROGRAM_H
#define XSC_PROGRAM_H

#include "XSCBase/XSC_Common.hxx"
#include "XSCUtility/XSC_Logger.h"
#include "XSCExe/XSC_NodeViewer.h"

#include <vector>
#include <string>

namespace XSC
{
  class XSC_Program : public XSC_Object
  {
  public:

    XSC_Program();

    virtual void SClassSetup();
    virtual bool SClassStart();
    virtual bool SClassStop();

    virtual int ProgramMain(int argc, char *argv[]);

    static XSC_Program*& getGlobalProgramPtrRef();
    static XSC_Program& getGlobalProgram();
    static void logCallback(XSC::Log::eLogType iType, const std::string iMessage);

  private:

    bool processVariables(std::vector<std::string> iCmdIDArgList);
    int runNodeViewer();

    XSC_SingletonProvider mSingletonProvider;
    XSC_NodeViewer mNodeViewer;

    XSC::XSC_Bool mExitProgram;
    XSC::XSC_Bool mRunNodeViewer;
    XSC::XSC_String mLogFileName;
    XSC::XSC_String mLogFilePath;
    XSC::XSC_String mDisplayName;
    unsigned int mDisplayLogLevel;
    std::vector<std::string> mConfigFileList;
  };

}
#endif // !XSC_PROGRAM_H

