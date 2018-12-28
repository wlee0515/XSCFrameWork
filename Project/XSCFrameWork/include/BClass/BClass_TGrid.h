#ifndef T_DATA_GRID_TEMPLATE_HPP
#define T_DATA_GRID_TEMPLATE_HPP

#include <vector>
#include <array>
#include <iostream>
#include "BClass_BitByteWord.h"

namespace XSC
{
  template<typename T>
  class BClassTGridBase
  {
  public:
    BClassTGridBase()
    {
      mSize[0] = 1;
      mSize[1] = 1;

      mMirrorModeFlag = false;
      mGrid.resize(1);
    }

    virtual ~BClassTGridBase()
    {
    }

    void resizeGrid(const unsigned int& iRow, const unsigned int&  iCol)
    {
      unsigned int wRow = iRow, wCol = iCol;

      if(wRow == 0)
      {
        wRow = 1;
      }

      if(wCol == 0)
      {
        wCol = 1;
      }

      unsigned int wNewSize = wRow*wCol;

      if(wNewSize > mGrid.size())
      {
        mGrid.reserve(wNewSize);
      }

      if(wCol > mSize[1])
      {
        unsigned int wDelta = wCol - mSize[1];
        vector<T> wTempVector;
        wTempVector.resize(wDelta);
        for(unsigned int wi = 0 ; wi < mSize[0] ; ++wi)
        {
          mGrid.insert(mGrid.begin() + wi*wCol + mSize[1],wTempVector.begin(),wTempVector.end());
        }
      }
      else if(wCol < mSize[1])
      {
        //unsigned int wDelta = mSize[1] - wCol;
        for(int wi = mSize[0] - 1 ; wi >= 0 ; --wi)
        {
          unsigned int wBase = wi*mSize[1];
          mGrid.erase(mGrid.begin() + wBase + wCol , mGrid.begin() + wBase + mSize[1]);
        }
      }

      mGrid.resize(wNewSize);
      mSize[0] = wRow;
      mSize[1] = wCol;
    }

    void resizeGrid(const unsigned int& iRow, const unsigned int&  iCol, T iValue)
    {
      unsigned int wRow = iRow, wCol = iCol;

      if (wRow == 0)
      {
        wRow = 1;
      }

      if (wCol == 0)
      {
        wCol = 1;
      }

      unsigned int wNewSize = wRow*wCol;

      if (wNewSize > mGrid.size())
      {
        mGrid.reserve(wNewSize);
      }

      if (wCol > mSize[1])
      {
        unsigned int wDelta = wCol - mSize[1];
        vector<T> wTempVector;
        wTempVector.resize(wDelta, iValue);
        for (unsigned int wi = 0; wi < mSize[0]; ++wi)
        {
          mGrid.insert(mGrid.begin() + wi*wCol + mSize[1], wTempVector.begin(), wTempVector.end());
        }
      }
      else if (wCol < mSize[1])
      {
        //unsigned int wDelta = mSize[1] - wCol;
        for (int wi = mSize[0] - 1; wi >= 0; --wi)
        {
          unsigned int wBase = wi*mSize[1];
          mGrid.erase(mGrid.begin() + wBase + wCol, mGrid.begin() + wBase + mSize[1]);
        }
      }

      mGrid.resize(wNewSize);
      mSize[0] = wRow;
      mSize[1] = wCol;

      setAllValue(iValue);
    }


    void resizeGrid(const array<unsigned int, 2>& iSize)
    {
      resizeGrid(iSize[0], iSize[1]);
    }

    void resizeGrid(const array<unsigned int, 2>& iSize, T iDefault)
    {
      resizeGrid(iSize[0], iSize[1], iDefault);
    }

    void matchSize(const array<unsigned int, 2>& iSize)
    {
      if((iSize[0] > mSize[0]) && (iSize[1] > mSize[1]))
      {
        resizeGrid(iSize[0], iSize[1]);
      }
      else if((iSize[0] > mSize[0]) && (iSize[1] <= mSize[1]))
      {
        setRowCount(iSize[0]);
      }
      else if((iSize[0] <= mSize[0]) && (iSize[1] > mSize[1]))
      {
        setColumnCount(iSize[1]);
      }

    }

    void setRowCount(const unsigned int& iRow)
    {
      resizeGrid(iRow, getColumnCount());
    }


    void setColumnCount(const unsigned int& iCol)
    {
      resizeGrid(getRowCount(), iCol);
    }

    const array<unsigned int,2>& getGridSize() const
    {
      return mSize;
    }

    inline const unsigned int getRowCount() const
    {
      return mSize[0];
    }

    inline const unsigned int getColumnCount() const
    {
      return mSize[1];
    }

    inline const vector<T>& get1DVector() const
    {
      return mGrid;
    }

    void setMirrorModeFlag(bool iFlag)
    {
      mMirrorModeFlag = iFlag;
    }

    bool getMirrorModeFlag() const
    {
      return mMirrorModeFlag;
    }


    virtual unsigned int processIndex( unsigned int& iRow, unsigned int& iCol) const
    {
      return iRow%mSize[0]*mSize[1] + iCol%mSize[1];
    }

    virtual unsigned int processIndex( int iRow, int iCol) const
    {
      array<int,2> wSize;
      wSize[0] = static_cast<int>(mSize[0]);
      wSize[1] = static_cast<int>(mSize[1]);

      if(true == mMirrorModeFlag)
      {
        while((iRow < 0) || (iRow >= wSize[0]))
        {
          iRow = iRow < 0 ? 0 - iRow - 1 : iRow;
          iRow = iRow >= wSize[0] ? 2*(wSize[0]) - 1 - iRow : iRow;
        }

        while((iCol < 0) || (iCol >= wSize[1]))
        {
          iCol = iCol < 0 ? 0 - iCol - 1 : iCol;
          iCol = iCol >= wSize[1] ? 2*(wSize[1]) - 1 - iCol : iCol;
        }
      }
      else
      {
        while((iRow < 0) || (iRow >= wSize[0]))
        {
          iRow = iRow < 0 ? iRow + wSize[0] : iRow;
          iRow = iRow >= wSize[0] ? iRow - wSize[0] : iRow;
        }

        while((iCol < 0) || (iCol >= wSize[1]))
        {
          iCol = iCol < 0 ? iCol + wSize[1] : iCol;
          iCol = iCol >= wSize[1] ? iCol - wSize[1] : iCol;
        }
      }

      return static_cast<unsigned int>(iRow)*mSize[1] + static_cast<unsigned int>(iCol);
    }

    void setAllValue(T iValue)
    {
      for (unsigned int wi = 0; wi < mGrid.size(); ++wi)
      {
        mGrid[wi] = iValue;
      }
    }
  protected:
    bool mMirrorModeFlag;
    array<unsigned int,2> mSize;
    vector<T> mGrid;

  };


  template<typename T>
  class BClassTGrid : public BClassTGridBase<T>
  {
  public:
    inline const T& get(const unsigned int& iRow, const unsigned int& iCol) const
    {
      return this->mGrid[this->processIndex(iRow,iCol)];
    }

    inline const T& get(const int& iRow, const int& iCol) const
    {
      return this->mGrid[this->processIndex(iRow,iCol)];
    }

    inline T& get(const unsigned int& iRow, const unsigned int& iCol)
    {
      return this->mGrid[this->processIndex(iRow,iCol)];
    }

    inline T& get(const int& iRow, const int& iCol)
    {
      return this->mGrid[this->processIndex(iRow,iCol)];
    }

    friend std::ostream& operator<<(std::ostream& iOut, const XSC::BClassTGrid<T>& iGrid)
    {
      for (unsigned int wi = 0; wi < iGrid.getRowCount(); ++wi)
      {
        for (unsigned int wj = 0; wj < iGrid.getColumnCount(); ++wj)
        {
          iOut << iGrid.get(wi, wj) << " ";
        }
        iOut << std::endl;

      }
      return iOut;
    }
  };

  template<>
  class BClassTGrid<bool> : public BClassTGridBase<bool>
  {
  public:

    const BClassTGrid<bool>& operator=(const BClassTGridBase<XSC::BClassEightByteSet>& iUIntGrid)
    {
      mMirrorModeFlag = iUIntGrid.getMirrorModeFlag();
      resizeGrid(iUIntGrid.getRowCount()*8 , iUIntGrid.getColumnCount()*8);

      XSC::BClassEightByteSet wContainer;
      unsigned int wRowIndex, wColIndex;
      for(unsigned int wi = 0; wi < iUIntGrid.getRowCount(); ++wi)
      {
        for(unsigned int wj = 0; wj < iUIntGrid.getColumnCount(); ++wj)
        {
          wRowIndex = wi*8;
          wColIndex = wj*8;

          for (unsigned int wp = 0; wp < 8; ++wp)
          {
            for (unsigned int wq = 0; wq < 8; ++wq)
            {
              get(wRowIndex + wp, wColIndex + wq) = iUIntGrid.get1DVector()[iUIntGrid.processIndex(wi, wj)][wp][wq];
            }
          }
        }
      }
      return *this;
    }

    inline std::vector<bool>::const_reference get(const unsigned int& iRow, const unsigned int& iCol) const
    {
      return mGrid[processIndex(iRow,iCol)];
    }

    inline std::vector<bool>::const_reference get(const int& iRow, const int& iCol) const
    {
      return mGrid[processIndex(iRow,iCol)];
    }

    inline std::vector<bool>::reference get(const unsigned int& iRow, const unsigned int& iCol)
    {
      return mGrid[processIndex(iRow,iCol)];
    }

    inline std::vector<bool>::reference get(const int& iRow, const int& iCol)
    {
      return mGrid[processIndex(iRow,iCol)];
    }

    friend std::ostream& operator<<(std::ostream& iOut, const XSC::BClassTGrid<bool>& iGrid)
    {
      for (unsigned int wi = 0; wi < iGrid.getRowCount(); ++wi)
      {
        for (unsigned int wj = 0; wj < iGrid.getColumnCount(); ++wj)
        {
          iOut << iGrid.get(wi, wj) << " ";
        }
        iOut << std::endl;

      }
      return iOut;
    }
  };


  template<>
  class BClassTGrid<BClassEightByteSet>: public BClassTGridBase<BClassEightByteSet>
  {
  public:

    const BClassTGrid<BClassEightByteSet>& operator=(const BClassTGrid<bool>& iBoolGrid)
    {
      mMirrorModeFlag = iBoolGrid.getMirrorModeFlag();

      std::array<unsigned int,2> wSize;
      wSize[0] = iBoolGrid.getRowCount() / 8;
      wSize[0] = (iBoolGrid.getRowCount() % 8) == 0 ? wSize[0] : wSize[0] + 1;

      wSize[1] = iBoolGrid.getColumnCount() / 8;
      wSize[1] = (iBoolGrid.getColumnCount() % 8) == 0 ? wSize[1] : wSize[1] + 1;

      resizeGrid(wSize[0] , wSize[1]);

      unsigned int wRowIndex, wColIndex;
      for(unsigned int wi = 0; wi < getRowCount(); ++wi)
      {
        for(unsigned int wj = 0; wj < getColumnCount(); ++wj)
        {
          wRowIndex = wi * 8;
          wColIndex = wj * 8;

          for (unsigned int wp = 0; wp < 8; ++wp)
          {
            for (unsigned int wq = 0; wq < 8; ++wq)
            {

              if (true == iBoolGrid.get(wRowIndex + wp, wColIndex + wq))
              {
                get(wi, wj)[wp].setbit(wq);
              }
              else
              {
                get(wi, wj)[wp].clearbit(wq);

              }
            }
          }
        }
      }
      return *this;
    }

    inline const BClassEightByteSet& get(const unsigned int& iRow, const unsigned int& iCol) const
    {
      return mGrid[processIndex(iRow,iCol)];
    }

    inline const BClassEightByteSet& get(const int& iRow, const int& iCol) const
    {
      return mGrid[processIndex(iRow,iCol)];
    }

    inline BClassEightByteSet& get(const unsigned int& iRow, const unsigned int& iCol)
    {
      return mGrid[processIndex(iRow,iCol)];
    }

    inline BClassEightByteSet& get(const int& iRow, const int& iCol)
    {
      return mGrid[processIndex(iRow,iCol)];
    }
  };

  template<typename T>
  class BClassTOffsetGrid : public BClassTGrid<T>
  {
  public:
    BClassTOffsetGrid()
    {
      mOffset[0] = 0;
      mOffset[1] = 0;
    }

    BClassTOffsetGrid(const BClassTGrid<T>& iGrid)
    {
      this->mMirrorModeFlag = iGrid.mMirrorModeFlag;
      this->mSize = iGrid.mSize;
      this->mGrid = iGrid.mGrid;
      this->mOffset[0] = 0;
      this->mOffset[1] = 0;
    }

    BClassTOffsetGrid& operator=(const BClassTGrid<T>& iGrid)
    {
      this->mMirrorModeFlag = iGrid.mMirrorModeFlag;
      this->mSize = iGrid.mSize;
      this->mGrid = iGrid.mGrid;
      return *this;
    }

    void setOffset(const array<int, 2>& iOffset)
    {
      mOffset[0] = iOffset[0];
      mOffset[1] = iOffset[1];
    }

    void setOffset(const int& iRowOffset,const int& iColumnOffset)
    {
      mOffset[0] = iRowOffset;
      mOffset[1] = iColumnOffset;
    }

    void setRowOffset(const int& iOffset)
    {
      mOffset[0] = iOffset;
    }

    void setColumnOffset(const int& iOffset)
    {
      mOffset[1] = iOffset;
    }

    const array<int, 2>& getOffset() const
    {
      return mOffset;
    }

    const int& getRowOffset() const
    {
      return mOffset[0];
    }

    const int& getColumnOffset() const
    {
      return mOffset[1];
    }

  protected:

    virtual unsigned int processIndex( unsigned int& iRow, unsigned int& iCol) const
    {
      return BClassTGrid<T>::processIndex(static_cast<int>(iRow) + mOffset[0], static_cast<int>(iCol) + mOffset[1]);
    }

    virtual unsigned int processIndex( int iRow, int iCol) const
    {
      return typename BClassTGrid<T>::processIndex(iRow + mOffset[0], iCol + mOffset[1]);
    }

    array<int, 2> mOffset;
  };

}


#endif
