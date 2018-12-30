#include <iostream>
#include "XSCExe/XSC_Program.h"
#include "XSCBase/XSC_Scheduler.h"
#include "XSCConnect/XSC_ConnectionServer.h"
#include "XSCMemoryBlock/XSC_DefinedMemoryBlock.h"
#include "XSCServiceProvider/XSC_HttpProvider.h"

int main(int argc, char *argv[])
{
  int wState = 1;
  while (1 == wState)
  {
    // Create an instance of the main program
    XSC::XSC_Program wProgramObj;

    // Set this program as the main referenced program
    XSC::XSC_Program*& wProgramPtrRef = XSC::XSC_Program::getGlobalProgramPtrRef();
    wProgramPtrRef = &wProgramObj;

    // Setup the all components of the program
    wProgramObj.SClassSetup();

    // Create all Singletons that needs to be defineable in the XML
    XSC::XSC_Scheduler&           wScheduler          = XSC::Singleton::getSingleton<XSC::XSC_Scheduler>();
    XSC::XSC_ConnectionServer&    wConnectionServer   = XSC::Singleton::getSingleton<XSC::XSC_ConnectionServer>();
    XSC::XSC_DefinedMemoryBlock&  wDefinedMemoryBlock = XSC::Singleton::getSingleton<XSC::XSC_DefinedMemoryBlock>();
    XSC::XSC_HttpProvider&        wHttpProvider       = XSC::Singleton::getSingleton<XSC::XSC_HttpProvider>();

    // Ge a reference to the current program
    XSC::XSC_Program& wProgram = XSC::XSC_Program::getGlobalProgram();

    // Execute the program Main
    wState = wProgram.ProgramMain(argc, argv);
  }

  system("pause");
  return 0;
}