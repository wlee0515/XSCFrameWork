#ifndef  XSC_ACTION_ENUM_H
#define  XSC_ACTION_ENUM_H

#include "XSCBase/XSC_EnumTableProvider.h"

namespace XSC
{
  enum XSC_Action {
    eXSC_Setup,
    eXSC_Start,
    eXSC_Stop,
  };

  ENUM_DEFINITION_DECLARE(XSC, eXSC_Setup, "Setup");
  ENUM_DEFINITION_DECLARE(XSC, eXSC_Start, "Start");
  ENUM_DEFINITION_DECLARE(XSC, eXSC_Stop, "Stop");
}

#endif // ! XSC_ACTION_ENUM_H
