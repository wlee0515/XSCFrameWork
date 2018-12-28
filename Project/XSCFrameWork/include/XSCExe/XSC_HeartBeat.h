#ifndef XSC_HEART_BEAT_H
#define XSC_HEART_BEAT_H

#include "XSCBase\XSC_Base.hxx"
#include "XSCBase\XSC_TData.hxx"
#include "XSCBase\XSC_Scheduler.h"

namespace XSC
{
  class XSC_HeartBeat : public XSC::XSC_Object
    , public virtual XSC::IEntryPoint
  {
  public:

    virtual void SClassSetup();
    virtual bool SClassStart();
    virtual bool SClassStop();

    virtual void entrypoint(double iDt);

  private:
    XSC_UInt32 mTier;
    XSC_UInt64  mTimeSinceStart;
    double mCarryOver;
  };

}
#endif
