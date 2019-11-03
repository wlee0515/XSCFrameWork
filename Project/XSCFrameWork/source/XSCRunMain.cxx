#include <iostream>
#include "XSCExe/XSC_Program.h"
#include "XSCBase/XSC_Scheduler.h"
#include "XSCConnect/XSC_ConnectionServer.h"
#include "XSCMemoryBlock/XSC_DefinedMemoryBlock.h"
#include "XSCServiceProvider/XSC_HttpProvider.h"

int main(int argc, char *argv[])
{
  std::cout << "Program Starting" << std::endl;
  
  int wState = 1;
  while (1 == wState)
  {
    // Create an instance of the main program
    std::cout << "Creating Program Object" << std::endl;
    XSC::XSC_Program wProgramObj;
    std::cout << "Program Object Creation Completed" << std::endl;

    // Set this program as the main referenced program
    std::cout << "Registering Program Object" << std::endl;
    XSC::XSC_Program*& wProgramPtrRef = XSC::XSC_Program::getGlobalProgramPtrRef();
    wProgramPtrRef = &wProgramObj;

    // Setup the all components of the program
    std::cout << "Setting up Program Object" << std::endl;
    wProgramObj.SClassSetup();
    std::cout << "Program Object Setup Complete" << std::endl;

    // Create all Singletons that needs to be defineable in the XML
    std::cout << "Creating Program Definitable Singletons" << std::endl;
    XSC::Singleton::getSingleton<XSC::XSC_Scheduler>();
    XSC::Singleton::getSingleton<XSC::XSC_DefinedMemoryBlock>();
    XSC::Singleton::getSingleton<XSC::XSC_ConnectionServer>();    
    XSC::Singleton::getSingleton<XSC::XSC_HttpProvider>();
    std::cout << "Singletons Creation Complete" << std::endl;

    // Ge a reference to the current program
    XSC::XSC_Program& wProgram = XSC::XSC_Program::getGlobalProgram();

    // Execute the program Main
    std::cout << "Running Program Object ..." << std::endl;
    wState = wProgram.ProgramMain(argc, argv);
    std::cout << "Exited Program Object" << std::endl;
  }
  
  std::cout << std::endl;
  std::cout << "Main Program Exited" << std::endl;
  std::cout << "Good Bye" << std::endl;

  return 0;
}