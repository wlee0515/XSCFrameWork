#ifndef XSC_SCHEDULER_H
#define XSC_SCHEDULER_H

#include "XSCBase/XSC_Base.hxx"
#include "XSCBase/XSC_TData.hxx"

#include <thread>
#include <chrono>

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

    XSC_Scheduler() : mCycleCount(0), mBasePeriod(1.0)
    {

    }

    virtual void SClassSetup()
    {
      addChild("Base Period", mBasePeriod);
      addChild("Cycle Count", mCycleCount);

      XSC_Object::SClassSetup();
    }

    virtual bool SClassStart()
    {
      return XSC_Object::SClassStart();
    }

    virtual bool SClassStop()
    {
      mExecuteScheduler = false;

      if (true == mSchedulerThread.joinable())
      {
        mSchedulerThread.join();
      }

      mExecutionTable.clear();
      return XSC_Object::SClassStop();
    }

    void addEntryPoint(unsigned int iTier, IEntryPoint& iEntryPoint)
    {
      mExecutionTable[iTier].push_back(&iEntryPoint);
    }

    void removeEntryPoint(IEntryPoint& iEntryPoint)
    {
//      mExecutionTable[iTier].push_back(&iEntryPoint);
    }

    void runScheduler()
    {
      if (0 != mExecutionTable.size())
      {
        if (false == mExecuteScheduler)
        {
          mExecuteScheduler = true;
          mSchedulerThread = std::thread(&XSC_Scheduler::schedulerWorkerThread, this);
          mSchedulerThread.detach();
        }
      }
    }

    void schedulerWorkerThread() const
    {
      double wBasePeriod = mBasePeriod.getData();
      unsigned long int wUInt_Period_Nano = static_cast<unsigned long int>(wBasePeriod * 1000000000.0);
      std::chrono::nanoseconds wNano_Period(wUInt_Period_Nano);

      std::chrono::nanoseconds wDutyCycle = std::chrono::nanoseconds(0);
      std::chrono::nanoseconds wRestCycle = std::chrono::nanoseconds(0);

      std::chrono::high_resolution_clock::time_point wStartTime = std::chrono::high_resolution_clock::now();
      std::chrono::high_resolution_clock::time_point wEndTime = std::chrono::high_resolution_clock::now();
      std::chrono::high_resolution_clock::time_point wNextCycleTime = std::chrono::high_resolution_clock::now();
      
      mCycleCount = 0;

      while (true == mExecuteScheduler)
      {
        wStartTime = std::chrono::high_resolution_clock::now();

        for (EntryPointTable::const_iterator wListIt = mExecutionTable.begin(); wListIt != mExecutionTable.end(); ++wListIt)
        {
          unsigned int wMultiplier = static_cast<unsigned int>(pow(2, wListIt->first));
          unsigned int wCycle = mCycleCount % wMultiplier;
          double wPeriod = wMultiplier * wBasePeriod;

          for (unsigned int wIndex = 0; wIndex < wListIt->second.size(); ++wIndex)
          {
            if (wCycle == wIndex % wMultiplier)
            {
              wListIt->second[wIndex]->entrypoint(wPeriod);
            }
          }
        }

        wNextCycleTime = wStartTime;
        wNextCycleTime += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(wNano_Period);
        wEndTime = std::chrono::high_resolution_clock::now();

        wDutyCycle = std::chrono::duration_cast<std::chrono::nanoseconds>(wEndTime - wStartTime);
        wRestCycle = std::chrono::nanoseconds(0);

        if (wNano_Period > wDutyCycle)
        {
          wRestCycle = wNano_Period - wDutyCycle;
          std::this_thread::sleep_for(wRestCycle);
        }

        ++mCycleCount;
      }
    }

  private:
    XSC_Float64 mBasePeriod;
    mutable XSC_UInt64  mCycleCount;

    bool mExecuteScheduler;
    std::thread mSchedulerThread;
    EntryPointTable mExecutionTable;
  };

}

#endif