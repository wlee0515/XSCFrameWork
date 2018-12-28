#ifndef XSC_STRING_TOOLS
#define XSC_STRING_TOOLS

#include <string>
#include <sstream>

namespace XSC {
  namespace StrTools
  {
    inline std::string replaceAllSubString(const std::string& iString, const std::string iFind, const std::string iReplace)
    {
      std::string wReturnString(iString);
      size_t wIndex = 0;
      while (1) {
        wIndex = wReturnString.find(iFind, wIndex);
        if (wIndex != std::string::npos)
        {
          //          wReturnString.replace(wIndex, wIndex + iFind.length(), iReplace);
          wReturnString.erase(wIndex, iFind.length());
          wReturnString.insert(wIndex, iReplace);
          wIndex += iReplace.length();
        }
        else
        {
          break;
        }
      }
      return wReturnString;
    }

    inline std::string removeAllWhiteSpace(const std::string& iString)
    {
      std::string wRet;

      for (unsigned int wi = 0; wi < iString.length(); ++wi)
      {
        if (iString[wi] != ' ')
        {
          wRet += iString[wi];
        }
      }

      return wRet;
    }

    inline std::string removeBracingWhiteSpace(const std::string& iString)
    {
      unsigned int wStart = 0;
      unsigned int wEnd = static_cast<unsigned int>(iString.length());

      for (unsigned int wi = 0; wi < iString.length(); ++wi)
      {
        if (iString[wi] != ' ')
        {
          wStart = wi;
          break;
        }
      }

      for (unsigned int wi = static_cast<unsigned int>(iString.length()); wi != 0; --wi)
      {
        if (iString[wi] != ' ')
        {
          wEnd = wi;
          break;
        }
      }

      if (wStart < wEnd)
      {
        unsigned int wLength = wEnd - wStart;
        return iString.substr(wStart, wLength);
      }

      return iString;
    }
    inline bool removeBracingQuote(const std::string& iString, std::string& oValue)
    {
      if (iString.length() > 2)
      {
        unsigned int wStart = 0, wEnd = static_cast<unsigned int>(iString.length());
        if (iString[0] == '\'')
        {
          if (iString[iString.length() - 1] == '\'')
          {
            wStart = 1;
            wEnd = iString.length() - 1;
          }
        }
        else if (iString[0] == '\"')
        {
          if (iString[iString.length() - 1] == '\"')
          {
            wStart = 1;
            wEnd = iString.length() - 1;
          }
        }

        if (wEnd > wStart)
        {
          unsigned int wLength = wEnd - wStart;
          oValue = iString.substr(wStart, wLength);
          return true;
        }
      }
      else if (iString == "\"\"")
      {
        oValue = "";
        return true;
      }

      oValue = iString;
      return true;
    }

    inline bool removeBracingBracket(const std::string& iString, std::string& oValue)
    {
      if (iString.length() > 2)
      {
        unsigned int wStart = 0, wEnd = static_cast<unsigned int>(iString.length());
        if (iString[0] == '[')
        {
          if (iString[iString.length() - 1] == ']')
          {
            wStart = 1;
            wEnd = iString.length() - 1;
          }
        }
        else if (iString[0] == '(')
        {
          if (iString[iString.length() - 1] == ')')
          {
            wStart = 1;
            wEnd = iString.length() - 1;
          }
        }
        else if (iString[0] == '{')
        {
          if (iString[iString.length() - 1] == '}')
          {
            wStart = 1;
            wEnd = iString.length() - 1;
          }
        }

        if (wEnd > wStart)
        {
          unsigned int wLength = wEnd - wStart;
          oValue = iString.substr(wStart, wLength);
          return true;
        }
      }

      return false;
    }

    template<typename T_Value>
    inline std::string printValueToString(const T_Value& iValue)
    {
      std::stringstream wOut;
      wOut << iValue;
      return wOut.str();
    }

    template<>
    inline std::string printValueToString(const bool& iValue)
    {
      return true == iValue ? "true" : "false";
    }


    template<>
    inline std::string printValueToString(const char& iValue)
    {
      std::stringstream wOut;
      wOut << static_cast<int>(iValue);
      return wOut.str();
    }

    template<>
    inline std::string printValueToString(const unsigned char& iValue)
    {
      std::stringstream wOut;
      wOut << static_cast<unsigned int>(iValue);
      return wOut.str();
    }

    template<>
    inline std::string printValueToString(const std::string& iValue)
    {
      std::stringstream wOut;
      //wOut << "\"" << iValue << "\"";
      wOut << iValue;
      return wOut.str();
    }

    template<typename T, unsigned int N>
    inline std::string printValueToString(const std::array<T, N>& iValue)
    {
      if (N == 0)
      {
        return "[]";
      }

      std::stringstream wStream;
      wStream << "[" << printValueToString(iValue[0]);
      for (size_t wi = 1; wi < N; ++wi)
      {
        wStream << ", " << printValueToString(iValue[wi]);
      }
      wStream << "]";
      return wStream.str();
    }

    template<typename T>
    inline std::string printValueToString(const std::vector<T>& iValue)
    {
      if (iValue.size() == 0)
      {
        return "[]";
      }

      std::stringstream wStream;
      wStream << "[" << printValueToString(iValue[0]);
      for (size_t wi = 1; wi < iValue.size(); ++wi)
      {
        wStream << ", " << printValueToString(iValue[wi]);
      }
      wStream << "]";
      return wStream.str();
    }

    template<typename T>
    inline std::string printValueToString(const T iValue[], unsigned int iArraySize)
    {
      if (iArraySize == 0)
      {
        return "[]";
      }

      std::stringstream wStream;
      wStream << "[" << printValueToString(iValue[0]);
      for (size_t wi = 1; wi < iArraySize; ++wi)
      {
        wStream << ", " << printValueToString(iValue[wi]);
      }
      wStream << "]";
      return wStream.str();
    }

    template<typename T_Value>
    inline bool setStringToValue(const std::string& iString, T_Value& oValue)
    {
      std::stringstream wIn(iString);
      T_Value wTemp;
      wIn >> wTemp;

      if (false == wIn.fail())
      {
        oValue = wTemp;
        return true;
      }
      return false;
    }

    template<>
    inline bool setStringToValue(const std::string& iString, char& oValue)
    {
      std::stringstream wIn(iString);
      int wTemp;
      wIn >> wTemp;

      if (false == wIn.fail())
      {
        oValue = static_cast<char>(wTemp);
        return true;
      }
      return false;
    }

    template<>
    inline bool setStringToValue(const std::string& iString, unsigned char& oValue)
    {
      std::stringstream wIn(iString);
      unsigned int wTemp;
      wIn >> wTemp;

      if (false == wIn.fail())
      {
        oValue = static_cast<unsigned int>(wTemp);
        return true;
      }
      return false;
    }

    template<>
    inline bool setStringToValue(const std::string& iString, bool& oValue)
    {
      if ((iString == "true") || (iString == "TRUE"))
      {
        oValue = true;
        return true;
      }
      else if ((iString == "false") || (iString == "FALSE"))
      {
        oValue = false;
        return true;
      }

      return false;
    }

    template<>
    inline bool setStringToValue(const std::string& iString, std::string& oValue)
    {
      return removeBracingQuote(iString, oValue);
    }

    template<typename T, unsigned int N>
    inline bool setStringToValue(const std::string& iString, std::array<T, N>& oValue)
    {
      std::string wSubstance;
      if (true == removeBracingBracket(iString, wSubstance))
      {
        std::stringstream wStream1(wSubstance);
        std::string wNewArg;

        size_t wCount = 0;

        while ((wCount < N) && (std::getline(wStream1, wNewArg, ',')))
        {
          if ("" != wNewArg)
          {
            T wData;
            if (false == setStringToValue(wNewArg, wData))
            {
              std::cout << "Input is not of Suitable Type" << std::endl;
              return false;
            }
            oValue[wCount] = wData;
          }
          wCount++;
        }
        return true;
      }

      return false;
    }


    template<typename T>
    inline bool setStringToValue(const std::string& iString, T oValue[], unsigned int  iArraySize)
    {
      if (0 == iArraySize)
      {
        return false;
      }

      std::string wSubstance;
      if (true == removeBracingBracket(iString, wSubstance))
      {
        std::stringstream wStream1(wSubstance);
        std::string wNewArg;

        size_t wCount = 0;

        while ((wCount < iArraySize) && (std::getline(wStream1, wNewArg, ',')))
        {
          if ("" != wNewArg)
          {
            T wData;
            if (false == setStringToValue(wNewArg, wData))
            {
              std::cout << "Input is not of Suitable Type" << std::endl;
              return false;
            }
            oValue[wCount] = wData;
          }
          wCount++;
        }
        return true;
      }

      return false;
    }


    template<typename T>
    inline bool setStringToValue(const std::string& iString, std::vector<T>& oValue)
    {
      std::string wSubstance;
      if (true == removeBracingBracket(iString, wSubstance))
      {
        std::stringstream wStream1(wSubstance);
        std::string wNewArg;

        while (std::getline(wStream1, wNewArg, ','))
        {
          if ("" != wNewArg)
          {
            T wData;
            if (false == setStringToValue(wNewArg, wData))
            {
              std::cout << "Input is not of Suitable Type" << std::endl;
              return false;
            }
            oValue.push_back(wData);
          }
        }
        return true;
      }

      return false;
    }

    inline std::string printCStringToString(const char iValue[], unsigned int iArraySize)
    {

      std::stringstream wStream;
      for (size_t wi = 0; wi < iArraySize; ++wi)
      {
        if ('\0' == iValue[wi])
        {
          break;
        }

        wStream << iValue[wi];
      }
      return wStream.str();
    }

    inline bool setStringToCString(const std::string& iString, char iValue[], unsigned int iArraySize)
    {

      std::string wSubstance;
      if (true == removeBracingQuote(iString, wSubstance))
      {
        for (size_t wi = 0; wi < iArraySize; ++wi)
        {
          if (wi < wSubstance.length())
          {
            iValue[wi] = wSubstance.at(wi);
          }
          else
          {
            iValue[wi] = '\0';
          }
        }

        return true;
      }

      return false;
    }

  }
}

#endif
