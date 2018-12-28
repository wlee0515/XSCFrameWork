#include "XSCBase/XSC_Scheduler.h"

#include <thread>
#include <chrono>
#include <math.h>

namespace XSC
{

  XSC_Scheduler::XSC_Scheduler() : mCycleCount(0), mBasePeriod(1.0)
  {

  }

  void XSC_Scheduler::SClassSetup()
  {
    addChild("Base Period", mBasePeriod);
    addChild("Cycle Count", mCycleCount);

    XSC_Object::SClassSetup();
  }

  bool XSC_Scheduler::SClassStart()
  {
    return XSC_Object::SClassStart();
  }

  bool XSC_Scheduler::SClassStop()
  {
    mExecuteScheduler = false;

    if (true == mSchedulerThread.joinable())
    {
      mSchedulerThread.join();
    }

    mExecutionTable.clear();
    return XSC_Object::SClassStop();
  }

  void XSC_Scheduler::addEntryPoint(unsigned int iTier, IEntryPoint& iEntryPoint)
  {
    removeEntryPoint(iEntryPoint);
    EntryPointList& wList = mExecutionTable[iTier];

    for (unsigned int wIndex = 0; wIndex < wList.size(); ++wIndex)
    {
      if (nullptr == wList[wIndex])
      {
        wList[wIndex] = &iEntryPoint;
      }
    }
  }

  void XSC_Scheduler::removeEntryPoint(IEntryPoint& iEntryPoint)
  {
    for (typename EntryPointTable::iterator wListIt = mExecutionTable.begin(); wListIt != mExecutionTable.end(); ++wListIt)
    {
      for (unsigned int wIndex = 0; wIndex < wListIt->second.size(); ++wIndex)
      {
        if (&iEntryPoint == wListIt->second[wIndex])
        {
          wListIt->second[wIndex] = nullptr;
        }
      }
    }
  }

  void XSC_Scheduler::runScheduler()
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

  void XSC_Scheduler::schedulerWorkerThread() const
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

      for (typename EntryPointTable::const_iterator wListIt = mExecutionTable.begin(); wListIt != mExecutionTable.end(); ++wListIt)
      {
        unsigned int wMultiplier = static_cast<unsigned int>(std::pow(2, wListIt->first));
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

}
