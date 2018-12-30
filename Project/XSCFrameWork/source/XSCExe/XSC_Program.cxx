#include "XSCExe/XSC_Program.h"
#include "XSCExe/XSC_ParseXml.h"

#include <iostream>
#include <sstream>
#include <string>
#include <list>

namespace XSC
{

  ADD_TYPE_FACTORY_WITH_NAME(XSC_Program, "Program");

  XSC_Program::XSC_Program()
    : mLogFileName("LogFile.txt")
    , mLogFilePath("./")
    , mDisplayName("Program")
  {

  }

  void XSC_Program::SClassSetup()
  {
    addChild("LogFileName", mLogFileName);
    addChild("LogFilePath", mLogFilePath);
    addChild("DisplayName", mDisplayName);

    addChild("SingletonProvider", mSingletonProvider);
    addChild("NodeViewer", mNodeViewer);

    Singleton::setSingletonProvider(mSingletonProvider);

    return XSC_Object::SClassSetup();
  }

  bool XSC_Program::SClassStart()
  {
    XSC::Log::Logger::getGlobalLogger().subscribeToLogs(&XSC_Program::logCallback, XSC::Log::eDisplay | mDisplayLogLevel);

    Log::Logger::getGlobalLogger().setFileName(mLogFileName);
    Log::Logger::getGlobalLogger().setFilePath(mLogFilePath);

    mNodeViewer.addScope(mDisplayName, *this);

    Log::Logger::getGlobalLogger().saveDequeToTrunk();
    return XSC_Object::SClassStart();
  }

  bool XSC_Program::SClassStop()
  {
    return XSC_Object::SClassStop();
  }

  int XSC_Program::ProgramMain(int argc, char *argv[])
  {
    std::cout << "argc = " << argc << std::endl;
    std::vector<std::string> wArgv;
    for (int i = 0; i < argc; i++)
    {
      std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
      wArgv.push_back(argv[i]);
    }

    processVariables(wArgv);

    for (unsigned int wi = 0; wi < mConfigFileList.size(); ++wi)
    {
      std::list<std::string> wFileStack;
      Parse::parseXMLFile(*this, mConfigFileList[wi], wFileStack);
    }

    this->SClassStart();
    XSC::Singleton::getSingleton<XSC::XSC_Scheduler>().runScheduler();

    int wState = runNodeViewer();
    XSC::Log::Logger::getGlobalLogger().saveDequeToTrunk();

    this->SClassStop();
    return wState;
  }

  XSC_Program*& XSC_Program::getGlobalProgramPtrRef()
  {
    static XSC_Program* wGlobal = nullptr;
    return wGlobal;
  }

  XSC_Program& XSC_Program::getGlobalProgram()
  {
    XSC_Program*& wGlobal = getGlobalProgramPtrRef();

    if (nullptr == wGlobal)
    {
      wGlobal = new XSC_Program();
      wGlobal->SClassSetup();
    }

    return *wGlobal;
  }

  void XSC_Program::logCallback(XSC::Log::eLogType iType, const std::string iMessage)
  {
    std::cout << iMessage << std::endl;
  }


  int XSC_Program::runNodeViewer()
  {
    std::string wInput("");
    std::string wCurrentScope = mDisplayName;
    unsigned int wCurrentContentIndex = 0;

    while ((wInput != "exit") && (wInput != "reboot"))
    {
      std::stringstream wStream;
      mNodeViewer.processCmdLine(wInput, wStream);

      std::cout << wStream.str();
      std::cout << "cmd>";

      std::getline(std::cin, wInput);

      std::cout << "\n\n\n";
    }

    if (wInput == "reboot")
    {
      return 1;
    }
    
    return 0;
  }

  bool XSC_Program::processVariables(std::vector<std::string> iCmdIDArgList)
  {
    if (iCmdIDArgList.size() < 2)
    {
      std::cout << "No input arguments detected. Entering default Mode" << std::endl;
    }
    else
    {
      for (unsigned int wi = 0; wi < iCmdIDArgList.size(); ++wi)
      {
        if (iCmdIDArgList[wi] == "-file")
        {
          if (++wi < iCmdIDArgList.size())
          {
            mConfigFileList.push_back(iCmdIDArgList[wi]);
          }
        }
      }
    }
    return true;
  }


}

