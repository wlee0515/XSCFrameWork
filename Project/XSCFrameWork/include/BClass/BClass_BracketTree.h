#ifndef BClASS_BRACKET_TREE_H
#define BClASS_BRACKET_TREE_H

#include <vector>
#include <map>
#include <iostream>

namespace XSC
{
    class BracketChar {

    public:
      BracketChar()
      {
        mStart = '\0';
        mEnd = '\0';
        mSeperator = '\0';
      }

      BracketChar(const BracketChar& iBracket) : mStart(iBracket.mStart), mEnd(iBracket.mEnd), mSeperator(iBracket.mSeperator)
      {
      }

      BracketChar(char iStart, char iEnd)
      {
        mStart = iStart;
        mEnd = iEnd;
        mSeperator = '\0';
      }

      BracketChar(char iSeperator)
      {
        mStart = '\0';
        mEnd = '\0';
        mSeperator = iSeperator;
      }

      BracketChar(char iStart, char iSeperator, char iEnd)
      {
        mStart = iStart;
        mEnd = iEnd;
        mSeperator = iSeperator;
      }

      bool operator==(const BracketChar& iBracketChar) const
      {
        return (iBracketChar.mStart == mStart) && (iBracketChar.mEnd == mEnd) && (iBracketChar.mSeperator == mSeperator);
      }
      bool operator!=(const BracketChar& iBracketChar) const
      {
        return !operator==(iBracketChar);
      }
      char mStart;
      char mEnd;
      char mSeperator;
    };

    struct SubStringData
    {
      unsigned int mStart;
      unsigned int mLength;
    };

    struct BracketProperties
    {
      BracketChar mType;
      SubStringData mLocation;
      std::vector<SubStringData> mSeperations;
      int mParentIndex;
      int mSelfIndex;
    };

    class BracketTree
    {
    public:

      BracketChar mType;
      std::string mContent;
      SubStringData mLocation;
      std::vector<BracketTree> mElements;
      int mIndex;

      std::string print(unsigned int iIndent = 0)
      {
        std::stringstream wOut;

        std::string wSpacing;
        for (unsigned int wi = 0; wi < iIndent; ++wi)
        {
          wSpacing += " ";
        }

        wOut << std::endl << wSpacing << "BracketType : " << mType.mStart << mType.mSeperator << mType.mEnd;
        wOut << std::endl << wSpacing << "Content : " << mContent;
        wOut << std::endl << wSpacing << "Location : " << mLocation.mStart << " - " << mLocation.mLength;
        wOut << std::endl << wSpacing << "Index : " << mIndex;

        for (unsigned int wi = 0; wi < mElements.size(); ++wi)
        {
          wOut << std::endl << mElements[wi].print(iIndent + 1);
        }

        return wOut.str();
      }
    };

    inline bool getBracketProperties(std::vector<BracketProperties>& oOutput, const std::vector<BracketChar>& iBracketTypes, const std::string iString)
    {
      oOutput.clear();
      int mActiveIndex = -1;
      int mFloatingActiveIndex = -1;
      bool wLastCharWasFloating = false;
      for (unsigned int wi = 0; wi < iString.length(); ++wi)
      {
        bool wCheckStartingChar = true;

        if (mActiveIndex != -1)
        {
          if (oOutput[mActiveIndex].mType.mEnd == oOutput[mActiveIndex].mType.mStart)
          {
            wCheckStartingChar = false;
          }
        }

        bool wIsStartingBracket = false;
        bool wIsFloatingChar = true;

        if (true == wCheckStartingChar)
        {
          for (const BracketChar& wBracketType : iBracketTypes)
          {
            if ((wBracketType.mStart == '\0') && (wBracketType.mEnd == '\0'))
            {
              if (iString[wi] == wBracketType.mSeperator)
              {
                wIsFloatingChar = false;
                break;
              }
            }
            if (iString[wi] == wBracketType.mStart)
            {
              oOutput.resize(oOutput.size() + 1);
              int wNewIndex = static_cast<int>(oOutput.size()) - 1;

              oOutput[wNewIndex].mType = wBracketType;
              oOutput[wNewIndex].mLocation.mStart = wi;
              oOutput[wNewIndex].mParentIndex = mActiveIndex;
              oOutput[wNewIndex].mSelfIndex = wNewIndex;
              mActiveIndex = wNewIndex;
              wIsStartingBracket = true;
              wIsFloatingChar = false;
              break;
            }
          }
        }

        if ((mActiveIndex != -1) && (false == wIsStartingBracket))
        {
          if (iString[wi] == oOutput[mActiveIndex].mType.mEnd)
          {

            SubStringData wData;
            if (oOutput[mActiveIndex].mSeperations.size() == 0)
            {
              wData.mStart = oOutput[mActiveIndex].mLocation.mStart + 1;
            }
            else
            {
              wData.mStart = oOutput[mActiveIndex].mSeperations.back().mStart + oOutput[mActiveIndex].mSeperations.back().mLength;
            }

            wData.mLength = wi - wData.mStart + 1;
            oOutput[mActiveIndex].mSeperations.push_back(wData);

            oOutput[mActiveIndex].mLocation.mLength = wi - oOutput[mActiveIndex].mLocation.mStart + 1;
            mActiveIndex = oOutput[mActiveIndex].mParentIndex;
            wIsFloatingChar = false;
          }
          else if (iString[wi] == oOutput[mActiveIndex].mType.mSeperator)
          {
            SubStringData wData;
            if (oOutput[mActiveIndex].mSeperations.size() == 0)
            {
              wData.mStart = oOutput[mActiveIndex].mLocation.mStart + 1;
            }
            else
            {
              wData.mStart = oOutput[mActiveIndex].mSeperations.back().mStart + oOutput[mActiveIndex].mSeperations.back().mLength;
            }

            wData.mLength = wi - wData.mStart + 1;
            oOutput[mActiveIndex].mSeperations.push_back(wData);
            wIsFloatingChar = false;
          }
        }

        if ((false == wLastCharWasFloating) && (true == wIsFloatingChar))
        {
          oOutput.resize(oOutput.size() + 1);
          int wNewIndex = static_cast<int>(oOutput.size()) - 1;

          oOutput[wNewIndex].mType = BracketChar();

          oOutput[wNewIndex].mLocation.mStart = wi;
          oOutput[wNewIndex].mParentIndex = mActiveIndex;
          oOutput[wNewIndex].mSelfIndex = wNewIndex;
          mFloatingActiveIndex = wNewIndex;
        }

        if ((true == wLastCharWasFloating) && (false == wIsFloatingChar))
        {
          if (mFloatingActiveIndex >= 0)
          {
            oOutput[mFloatingActiveIndex].mLocation.mLength = wi - oOutput[mFloatingActiveIndex].mLocation.mStart;
            mFloatingActiveIndex = -1;
            wIsFloatingChar = false;
          }
        }

        wLastCharWasFloating = wIsFloatingChar;
      }

      if (true == wLastCharWasFloating)
      {
        if (mFloatingActiveIndex >= 0)
        {
          oOutput[mFloatingActiveIndex].mLocation.mLength = iString.length() - oOutput[mFloatingActiveIndex].mLocation.mStart;
          mFloatingActiveIndex = -1;
        }
      }

      if (mActiveIndex == -1)
      {
        return true;
      }

      return false;
    }

    inline bool getBracketTree(BracketTree& oOutput, const std::vector<BracketChar>& iBracketTypes, const std::string iString)
    {
      std::vector<BracketProperties> wBracketProperties;
      if (false == getBracketProperties(wBracketProperties, iBracketTypes, iString))
      {
        return false;
      }

      std::vector<BracketTree*> wBracketStack;

      wBracketStack.reserve(10);
      wBracketStack.push_back(&oOutput);

      int mActiveIndex = -1;

      oOutput.mType = BracketChar();
      oOutput.mContent = iString;
      oOutput.mLocation.mStart = 0;
      oOutput.mLocation.mLength = static_cast<unsigned int>(iString.length());
      oOutput.mIndex = -1;
      oOutput.mElements.resize(1);
      oOutput.mElements[0].mLocation = oOutput.mLocation;
      oOutput.mElements[0].mContent = iString;
      oOutput.mElements[0].mIndex = oOutput.mIndex;

      //for(BracketProperties& wBracket : wBracketProperties)
      for (BracketProperties& wBracket : wBracketProperties)
      {
        if (mActiveIndex != wBracket.mParentIndex)
        {
          bool wIsFound = false;
          for (BracketTree& wElements : wBracketStack.back()->mElements)
          {
            for (BracketTree& wSubTree : wElements.mElements)
            {
              if (wSubTree.mIndex == wBracket.mParentIndex)
              {
                wBracketStack.push_back(&wSubTree);
                mActiveIndex = wSubTree.mIndex;
                wIsFound = true;
                break;
              }
            }

            if (true == wIsFound)
            {
              break;
            }
          }

          if (false == wIsFound)
          {
            while (wBracketStack.size() != 0)
            {
              if (wBracketStack.back()->mIndex == wBracket.mParentIndex)
              {
                break;
              }
              wBracketStack.pop_back();

            }

            if (wBracketStack.size() == 0)
            {
              return false;
            }
          }
        }

        {
          BracketTree wBranch;
          if (wBracket.mType == BracketChar())
          {
            wBranch.mContent = iString.substr(wBracket.mLocation.mStart, wBracket.mLocation.mLength);
          }
          else
          {
            if (wBracket.mLocation.mLength < 2)
            {
              wBranch.mContent = "";
            }
            else
            {
              wBranch.mContent = iString.substr(wBracket.mLocation.mStart + 1, wBracket.mLocation.mLength - 2);
            }
          }
          wBranch.mIndex = wBracket.mSelfIndex;
          wBranch.mType = wBracket.mType;

          wBranch.mElements.resize(wBracket.mSeperations.size());
          for (unsigned int wi = 0; wi < wBranch.mElements.size(); ++wi)
          {
            wBranch.mElements[wi].mType = wBranch.mType;
            wBranch.mElements[wi].mIndex = wBranch.mIndex;
            wBranch.mElements[wi].mLocation = wBracket.mSeperations[wi];

            if (wBranch.mElements[wi].mType == BracketChar())
            {
              wBranch.mElements[wi].mContent = iString.substr(wBranch.mElements[wi].mLocation.mStart, wBranch.mElements[wi].mLocation.mLength);
            }
            else
            {
              if (wBracket.mLocation.mLength < 2)
              {
                wBranch.mElements[wi].mContent = "";
              }
              else
              {
                wBranch.mElements[wi].mContent = iString.substr(wBranch.mElements[wi].mLocation.mStart, wBranch.mElements[wi].mLocation.mLength - 1);
              }
            }

          }

          wBranch.mLocation = wBracket.mLocation;

          bool wIsInserted = false;
          for (BracketTree& wElements : wBracketStack.back()->mElements)
          {
            unsigned int wStart = wElements.mLocation.mStart;
            unsigned int wEnd = wElements.mLocation.mStart + wElements.mLocation.mLength;
            if ((wBranch.mLocation.mStart >= wStart) && (wBranch.mLocation.mStart < wEnd))
            {
              wElements.mElements.push_back(wBranch);
              wIsInserted = true;
              break;
            }
          }

          if (false == wIsInserted)
          {
            return false;
          }
        }

      }

      return true;
    }
 
}
#endif