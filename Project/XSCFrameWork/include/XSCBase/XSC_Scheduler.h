#ifndef XSC_SCHEDULER_H
#define XSC_SCHEDULER_H

#include "XSCBase/XSC_Base.hxx"
#include "XSCBase/XSC_TData.hxx"

#include <thread>

namespace XSC
{
  class IEntryPoint
  {
  public:
    virtual void entrypoint(double iDt) = 0;
  };
  
  class XSC_Scheduler : public XSC_Object
  {
  public:

    typedef std::vector<IEntryPoint*> EntryPointList;
    typedef std::map<unsigned int, EntryPointList> EntryPointTable;

    XSC_Scheduler();;

    virtual void SClassSetup();
    virtual bool SClassStart();

    virtual bool SClassStop();
    void addEntryPoint(unsigned int iTier, IEntryPoint& iEntryPoint);
    void removeEntryPoint(IEntryPoint& iEntryPoint);
    void runScheduler();

    void schedulerWorkerThread() const;

  private:
    XSC_Float64 mBasePeriod;
    mutable XSC_UInt64  mCycleCount;

    bool mExecuteScheduler;
    std::thread mSchedulerThread;
    EntryPointTable mExecutionTable;
  };

}

#endif