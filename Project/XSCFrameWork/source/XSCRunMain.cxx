#include <iostream>
#include "XSCExe/XSC_Program.h"
#include "XSCExe/XSC_DynamicTypeDefinition.h"

int main(int argc, char *argv[])
{
  bool wTyoeDefined = XSC::TypeDefinition();

  int wState = 1;
  while (1 == wState)
  {
    XSC::XSC_Program wProgramObj;
    XSC::XSC_Program*& wProgramPtrRef = XSC::XSC_Program::getGlobalProgramPtrRef();
    wProgramPtrRef = &wProgramObj;

    wProgramObj.SClassSetup();

    XSC::XSC_Program& wProgram = XSC::XSC_Program::getGlobalProgram();

    bool wSingletonDef = XSC::SingletonInstantiation();

    wState = wProgram.ProgramMain(argc, argv);
  }

  system("pause");
  return 0;
}