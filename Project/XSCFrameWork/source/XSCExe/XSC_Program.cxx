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
    std::cout << "Starting Program Object Main Function" << std::endl;
    std::cout << "argc = " << argc << std::endl;
    std::vector<std::string> wArgv;
    for (int i = 0; i < argc; i++)
    {
      std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
      wArgv.push_back(argv[i]);
    }

    std::cout << "Processing Arguments" << std::endl;
    processVariables(wArgv);

    for (unsigned int wi = 0; wi < mConfigFileList.size(); ++wi)
    {
      std::cout << "Processing Configuration File : " << mConfigFileList[wi] << std::endl;
      std::list<std::string> wFileStack;
      Parse::parseXMLFile(*this, mConfigFileList[wi], wFileStack);
    }

    std::cout << "Starting Program Object" << std::endl;
    this->SClassStart();

    std::cout << "Starting Program Object Scheduler" << std::endl;
    XSC::Singleton::getSingleton<XSC::XSC_Scheduler>().runScheduler();

    std::cout << "Starting Node Viewer" << std::endl;
    int wState = runNodeViewer();
    std::cout << "Node Viewer End" << std::endl;

    std::cout << "Saving Log" << std::endl;
    XSC::Log::Logger::getGlobalLogger().saveDequeToTrunk();
    std::cout << "Saving Log End" << std::endl;

    std::cout << "Stopping Program" << std::endl;
    this->SClassStop();
    std::cout << "Stopping Program - end" << std::endl;
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

