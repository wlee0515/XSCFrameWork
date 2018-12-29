#include "XSCExe/XSC_HeartBeat.h"

namespace XSC
{
  ADD_TYPE_FACTORY_WITH_NAME(XSC_HeartBeat, "HeartBeat");

  void XSC_HeartBeat::SClassSetup()
  {
    addChild("Tier", mTier);
    addChild("Time Since Start", mTimeSinceStart);

    XSC_Object::SClassSetup();
  }

  bool XSC_HeartBeat::SClassStart()
  {
    XSC::Singleton::getSingleton<XSC::XSC_Scheduler>().addEntryPoint(mTier, *this);
    mTimeSinceStart = 0;
    mCarryOver = 0.0;
    return XSC_Object::SClassStart();
  }

  bool XSC_HeartBeat::SClassStop()
  {
    return XSC_Object::SClassStop();
  }

  void XSC_HeartBeat::entrypoint(double iDt)
  {
    mCarryOver += iDt;
    while (1.0 < mCarryOver)
    {
      mCarryOver -= 1.0;
      ++mTimeSinceStart;
    }
  }

}
