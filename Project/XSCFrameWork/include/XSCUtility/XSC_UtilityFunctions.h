#ifndef XSC_UTILITY_FUNCTIONS_H
#define XSC_UTILITY_FUNCTIONS_H

#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <ctime>

#include "BClass/BClass_BracketTree.h"

#ifdef __linux__

#define localtime_s(x, y) localtime_r(y, x)

#endif

namespace XSC
{
  namespace tools
  {
    inline long unsigned int getClock()
    {
      return static_cast<long unsigned int>(clock());
    }

    inline void getTIme(tm& ioTimeInfo)
    {
      time_t rawtime;
      std::time(&rawtime);

      // on platforms with POSIX localtime_r but no localtime_s, add this macro
      // #define localtime_s(x, y) localtime_r(y, x)
      localtime_s(&ioTimeInfo, &rawtime); // fills in your structure,
                                        // instead of returning a pointer to a static one
    }

    inline std::string printTimeToString(tm& iTimeInfo)
    {
      char wtime[100];
      memset(wtime, 0, sizeof(char) * 100);
      strftime(wtime, 100, "%Y/%m/%d %H:%M:%S ", &iTimeInfo);

      return wtime;
    }


    inline std::string printTime()
    {
      struct tm timeinfo;
      XSC::tools::getTIme(timeinfo);

      return printTimeToString(timeinfo);
    }
    /*
    template<typename T_Type>
    inline bool setStringValueToData(const std::string& iStringVal, T_Type& oValue)
    {
      std::stringstream wStream(iStringVal);
      T_Type wTemp;
      wStream >> wTemp;
      if (false == wStream.fail())
      {
        oValue = wTemp;
        return true;
      }
      return false;
    }

    template<>
    inline bool setStringValueToData(const std::string& iStringVal, std::string& oValue)
    {
      oValue = iStringVal;
      return true;
    }
    */
    inline std::string labelKeyGenerator(const std::string& iPrefix, const int iIndex, const unsigned int iWidth, const char iFill = '0')
    {
      std::stringstream wKey;
      wKey << iPrefix << std::setfill(iFill) << std::setw(iWidth) << iIndex;
      return wKey.str();
    }
    
    
    inline void splitCmdLine(const std::string& iCmdLine, std::string& oCommand, std::vector<std::string> & oArguments)
    {
      std::vector<BracketChar> wBracketCharList;
 /*     wBracketCharList.emplace_back('[', ']');
      wBracketCharList.emplace_back('(', ')');
      wBracketCharList.emplace_back('{', '}');*/
      wBracketCharList.emplace_back('\'', '\'');
      wBracketCharList.emplace_back('\"', '\"');
      wBracketCharList.emplace_back(' ');

      bool wBracketTreeFailed = true;
      BracketTree wBracketTree;
      if (true == getBracketTree(wBracketTree, wBracketCharList, iCmdLine))
      {
        if (0 != wBracketTree.mElements.size())
        {
          BracketTree& wBase = wBracketTree.mElements[0];

          if (0 != wBase.mElements.size())
          {
            wBracketTreeFailed = false;
            oCommand = wBase.mElements[0].mContent;
            oArguments.clear();
            for (unsigned int i = 1; i < wBase.mElements.size(); ++i)
            {
              oArguments.emplace_back(wBase.mElements[i].mContent);
            }

          }
        }
      }
      
      if(true == wBracketTreeFailed)
      {
        std::stringstream wCmdStream(iCmdLine);
        std::string wStrContainer;
        oArguments.clear();

        wCmdStream >> wStrContainer;
        if (false == wCmdStream.fail())
        {
          oCommand = wStrContainer;
        }

        while (false == wCmdStream.fail())
        {
          wCmdStream >> wStrContainer;
          if (false == wCmdStream.fail())
          {
            oArguments.push_back(wStrContainer);
          }
        }
      }

      return;
    }
    
    inline bool getPathTokens(std::vector<std::string>& oPathToken, const std::string& iPath)
    {
      unsigned int wOriginalSize = static_cast<unsigned int>(oPathToken.size());
      std::stringstream wPathStream(iPath);
      std::string wForwardArg;
      std::string wBackwardArg;

      while (std::getline(wPathStream, wForwardArg, '\\'))
      {
        std::stringstream wNewArgumentStream(wForwardArg);
        while (std::getline(wNewArgumentStream, wBackwardArg, '/'))
        {
          if ("" != wBackwardArg)
          {
            oPathToken.push_back(wBackwardArg);
          }
        }
      }

      if (oPathToken.size() - wOriginalSize == 0)
      {
        return false;
      }

      return true;
    }

    inline bool resolveRelativePath(std::string& oAbsolutePath, const std::string& iBaseDirectoryItem, const std::string& iRelativePath)
    {
      oAbsolutePath = iRelativePath;
      if (iBaseDirectoryItem == "")
      {
        return true;
      }

      std::vector<std::string> wRelativePathToken;

      wRelativePathToken.clear();

      if (false == getPathTokens(wRelativePathToken, iRelativePath))
      {
        return false;
      }
      else if ((wRelativePathToken.front() != ".") && (wRelativePathToken.front() != ".."))
      {
        return true;
      }

      std::vector<std::string> oBasePathToken;
      oBasePathToken.clear();
      if (false == getPathTokens(oBasePathToken, iBaseDirectoryItem))
      {
        return false;
      }

      std::stringstream wLastrgumentStream(oBasePathToken.back());
      std::string wToken;
      unsigned int wCounter = 0;
      while (std::getline(wLastrgumentStream, wToken, '.'))
      {
        ++wCounter;
      }

      if (wCounter > 1)
      {
        oBasePathToken.pop_back();
      }

      std::vector<std::string> wAbsolutePathToken;

      for (std::vector<std::string>::iterator wIt = oBasePathToken.begin(); wIt != oBasePathToken.end(); ++wIt)
      {
        if (*wIt == "..")
        {
          wAbsolutePathToken.pop_back();
        }
        else if (*wIt == ".")
        {
        }
        else
        {
          wAbsolutePathToken.push_back(*wIt);
        }
      }

      for (std::vector<std::string>::iterator wIt = wRelativePathToken.begin(); wIt != wRelativePathToken.end(); ++wIt)
      {
        if (*wIt == "..")
        {
          wAbsolutePathToken.pop_back();
        }
        else if (*wIt == ".")
        {
        }
        else
        {
          wAbsolutePathToken.push_back(*wIt);
        }
      }

      std::stringstream wAbsolutePathStream;
      for (unsigned int wi = 0; wi < wAbsolutePathToken.size() - 1; ++wi)
      {
        wAbsolutePathStream << wAbsolutePathToken[wi] << '\\';
      }

      wAbsolutePathStream << wAbsolutePathToken.back();

      oAbsolutePath = wAbsolutePathStream.str();
      return true;
    }
  }
}

#endif