#include "XSCExe\XSC_DynamicTypeDefinition.h"

#include "XSCBase\XSC_TypeLibrary.h"
#include "XSCBase\XSC_SingletonProvider.hxx"

#include "XSCBase\XSC_Scheduler.h"

#include "XSCExe\XSC_Program.h"
#include "XSCExe\XSC_HeartBeat.h"

namespace XSC
{
  bool TypeDefinition()
  {
    ADD_TYPE_FACTORY_WITH_NAME(XSC_Program,                      "Program"                );
    ADD_TYPE_FACTORY_WITH_NAME(XSC_Scheduler,                    "Scheduler"              );
    ADD_TYPE_FACTORY_WITH_NAME(XSC_HeartBeat,                    "HeartBeat"              );

    return true;
  }

  bool SingletonInstantiation()
  {

    XSC_Scheduler&           wScheduler        = XSC::Singleton::getSingleton<XSC::XSC_Scheduler>();

    return true;
  }
}
