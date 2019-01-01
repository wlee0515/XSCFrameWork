#ifndef XSC_LOGGER_H
#define XSC_LOGGER_H

#include "XSC_UtilityFunctions.h"

#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <map>

namespace XSC
{
  namespace Log {

    enum eLogType
    {
      eDisplay    = 0x001
      , eError    = 0x002
      , eWarnning = 0x004
      , eTrace    = 0x008
      , eDebug1   = 0x010
      , eDebug2   = 0x020
      , eDebug3   = 0x040
    };


    class Logger
    {
    public:

      typedef void(*callback_FPtr)(Log::eLogType, const std::string);

      Logger()
        : mFileName("LogFile.txt")
        , mFilePath("./")
      {
      }


      inline void logMessage(const eLogType iLogType, const std::string& iLogMessage)
      {
        std::stringstream wLogString;

        switch (iLogType) {
        case eDisplay:
          wLogString << "DISP";
          break;
        case eWarnning:
          wLogString << "WARN";
          break;
        case eTrace:
          wLogString << "TRACE";
          break;
        case eDebug1:
          wLogString << "DEBUG1";
          break;
        case eDebug2:
          wLogString << "DEBUG2";
          break;
        case eDebug3:
          wLogString << "DEBUG3";
          break;
        default:
          break;
        }

        wLogString << ":" << tools::printTime() << " " << iLogMessage;
        mLogQueue.push_back(wLogString.str());

        for (std::map<callback_FPtr, unsigned int>::iterator wIt = mLogSucscribers.begin(); wIt != mLogSucscribers.end(); ++wIt)
        {
          if (0 != (iLogType & wIt->second))
          {
            wIt->first(iLogType, iLogMessage);
          }
        }
      }

      inline void saveDequeToTrunk()
      {
        if (mLogQueue.size() != 0)
        {
          std::list<std::string> wLogs;
          wLogs.swap(mLogQueue);

          std::ofstream wFileStream(mFilePath + "./" + mFileName, std::fstream::app);

          for (std::list<std::string>::iterator wIt = wLogs.begin(); wIt != wLogs.end(); ++wIt)
          {
            wFileStream << *wIt << std::endl;
          }

          wFileStream.close();
        }
      }

      inline void setFileName(std::string& iFileName)
      {
        mFileName = iFileName;
      }

      inline void setFilePath(std::string& iFilePath)
      {
        mFilePath = iFilePath;
      }

      inline std::string getFileName() const
      {
        return mFileName;
      }

      inline std::string getFilePath() const
      {
        return mFilePath;
      }

      inline void subscribeToLogs(callback_FPtr iLogSubscriber, unsigned int iDisplayFlag)
      {
        mLogSucscribers[iLogSubscriber] = iDisplayFlag;
      }

      inline void unsubscribeToLogs(callback_FPtr iLogSubscriber)
      {
        mLogSucscribers.erase(iLogSubscriber);
      }

      static Logger& getGlobalLogger()
      {
        static Logger* wGlobal = nullptr;

        if (nullptr == wGlobal)
        {
          wGlobal = new Logger();
        }

        return *wGlobal;
      }

    private:
      std::string mFileName;
      std::string mFilePath;
      std::list<std::string> mLogQueue;

      std::map<callback_FPtr, unsigned int> mLogSucscribers;
    };

  }
}

#define LOG(LOG_TYPE, LOG_MESSAGE)                                                                            \
{                                                                                                             \
    std::stringstream wLogStream;                                                                             \
    wLogStream << __FUNCTION__<< " : " << LOG_MESSAGE;                                                        \
    XSC::Log::Logger::getGlobalLogger().logMessage(LOG_TYPE, wLogStream.str());                               \
}                                                                                                             \

#define LOG_DISPLAY( LOG_MESSAGE) LOG(XSC::Log::eDisplay,  LOG_MESSAGE)
#define LOG_ERROR( LOG_MESSAGE)   LOG(XSC::Log::eError,    LOG_MESSAGE)
#define LOG_WARN( LOG_MESSAGE)    LOG(XSC::Log::eWarnning, LOG_MESSAGE)
#define LOG_TRACE( LOG_MESSAGE)   LOG(XSC::Log::eTrace,    LOG_MESSAGE)
#define LOG_DEBUG1( LOG_MESSAGE)  LOG(XSC::Log::eDebug1,   LOG_MESSAGE)
#define LOG_DEBUG2( LOG_MESSAGE)  LOG(XSC::Log::eDebug2,   LOG_MESSAGE)
#define LOG_DEBUG3( LOG_MESSAGE)  LOG(XSC::Log::eDebug3,   LOG_MESSAGE)

#endif