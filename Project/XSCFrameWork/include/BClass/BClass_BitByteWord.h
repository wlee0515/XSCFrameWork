#ifndef BCLASS_BIT_BYTE_WORD_H
#define BCLASS_BIT_BYTE_WORD_H

#include <array>
#include <iostream>

#define BITMASK0 0x01
#define BITMASK1 0x02
#define BITMASK2 0x04
#define BITMASK3 0x08
#define BITMASK4 0x10
#define BITMASK5 0x20
#define BITMASK6 0x40
#define BITMASK7 0x80

#define BITFUNC(NUM) bool bit##NUM () const {return (BITMASK##NUM & mData) != 0;} \
  void setbit##NUM (){ mData |= BITMASK##NUM;}                 \
  void clearbit##NUM (){ mData &= ~BITMASK##NUM;}               \
  void flipbit##NUM (){ mData ^= BITMASK##NUM;}

#define BITCOLFUNC(NUM) XSC::BClassEightBitSet bitCol##NUM () const { XSC::BClassEightBitSet wCol = 0; \
if(mData[0].bit##NUM ()) wCol.setbit0(); \
if(mData[1].bit##NUM ()) wCol.setbit1(); \
if(mData[2].bit##NUM ()) wCol.setbit2(); \
if(mData[3].bit##NUM ()) wCol.setbit3(); \
if(mData[4].bit##NUM ()) wCol.setbit4(); \
if(mData[5].bit##NUM ()) wCol.setbit5(); \
if(mData[6].bit##NUM ()) wCol.setbit6(); \
if(mData[7].bit##NUM ()) wCol.setbit7(); \
return wCol;} \

#define BITTOPPLANEFUNC(NUM) XSC::BClassEightByteSet bitTopPlane##NUM () const { XSC::BClassEightByteSet wPlane = 0; \
wPlane[0] = mData[0][NUM]; \
wPlane[1] = mData[1][NUM]; \
wPlane[2] = mData[2][NUM]; \
wPlane[3] = mData[3][NUM]; \
wPlane[4] = mData[4][NUM]; \
wPlane[5] = mData[5][NUM]; \
wPlane[6] = mData[6][NUM]; \
wPlane[7] = mData[7][NUM]; \
return wPlane;} \

#define BITSIDEPLANEFUNC(NUM) XSC::BClassEightByteSet bitSidePlane##NUM () const { XSC::BClassEightByteSet wPlane = 0; \
wPlane[0] = mData[0].bitCol##NUM (); \
wPlane[1] = mData[1].bitCol##NUM (); \
wPlane[2] = mData[2].bitCol##NUM (); \
wPlane[3] = mData[3].bitCol##NUM (); \
wPlane[4] = mData[4].bitCol##NUM (); \
wPlane[5] = mData[5].bitCol##NUM (); \
wPlane[6] = mData[6].bitCol##NUM (); \
wPlane[7] = mData[7].bitCol##NUM (); \
return wPlane;} \


namespace XSC
{
  class BClassEightBitSet
  {
  public:
    BITFUNC(0);
    BITFUNC(1);
    BITFUNC(2);
    BITFUNC(3);
    BITFUNC(4);
    BITFUNC(5);
    BITFUNC(6);
    BITFUNC(7);

    BClassEightBitSet()
    {
      mData = 0;
    }

    BClassEightBitSet(unsigned char iValue)
    {
      mData = iValue;
    }

    unsigned char& getData()
    {
      return mData;
    }

    const unsigned char getData() const
    {
      return mData;
    }

    operator unsigned char&()
    {
      return mData;
    }

    operator unsigned char() const
    {
      return mData;
    }

    void setbit(unsigned int iIndex)
    {
      iIndex = iIndex % 8;

      switch (iIndex)
      {
      case 0: setbit0(); break;
      case 1: setbit1(); break;
      case 2: setbit2(); break;
      case 3: setbit3(); break;
      case 4: setbit4(); break;
      case 5: setbit5(); break;
      case 6: setbit6(); break;
      case 7: setbit7(); break;
      default:
        break;
      }
    }

    void clearbit(unsigned int iIndex)
    {
      iIndex = iIndex % 8;

      switch (iIndex)
      {
      case 0: clearbit0(); break;
      case 1: clearbit1(); break;
      case 2: clearbit2(); break;
      case 3: clearbit3(); break;
      case 4: clearbit4(); break;
      case 5: clearbit5(); break;
      case 6: clearbit6(); break;
      case 7: clearbit7(); break;
      default:
        break;
      }
    }

    void flipbit(unsigned int iIndex)
    {
      iIndex = iIndex % 8;

      switch (iIndex)
      {
      case 0: flipbit0(); break;
      case 1: flipbit1(); break;
      case 2: flipbit2(); break;
      case 3: flipbit3(); break;
      case 4: flipbit4(); break;
      case 5: flipbit5(); break;
      case 6: flipbit6(); break;
      case 7: flipbit7(); break;
      default:
        break;
      }
    }
    const bool operator[](unsigned int iIndex) const
    {
      iIndex = iIndex % 8;

      switch (iIndex)
      {
      case 0: return bit0();
      case 1: return bit1();
      case 2: return bit2();
      case 3: return bit3();
      case 4: return bit4();
      case 5: return bit5();
      case 6: return bit6();
      case 7: return bit7();
      default:
        break;
      }
      return false;
    }

    const bool operator[](int iIndex) const
    {
      if (iIndex >= 0)
      {
        return operator[](static_cast<unsigned int>(iIndex % 8));
      }
      else
      {
        return operator[](static_cast<unsigned int>(8 + iIndex % 8));
      }
    }

    unsigned int countActiveBits()
    {
      unsigned int wbitCount = 0;

      if (bit0()) ++wbitCount;
      if (bit1()) ++wbitCount;
      if (bit2()) ++wbitCount;
      if (bit3()) ++wbitCount;
      if (bit4()) ++wbitCount;
      if (bit5()) ++wbitCount;
      if (bit6()) ++wbitCount;
      if (bit7()) ++wbitCount;
      return wbitCount;

    }

    friend std::ostream& operator<<(std::ostream& iCout, const BClassEightBitSet& iBits)
    {

      iCout << iBits.bit0();
      iCout << iBits.bit1();
      iCout << iBits.bit2();
      iCout << iBits.bit3();
      iCout << iBits.bit4();
      iCout << iBits.bit5();
      iCout << iBits.bit6();
      iCout << iBits.bit7();

      return iCout;
    }

  private:
    unsigned char mData;
  };

  class BClassEightByteSet
  {
  public:
    BClassEightByteSet()
    {
      getData() = 0;
    }

    BClassEightByteSet(unsigned long long int iValue)
    {
      getData() = iValue;
    }

    BITCOLFUNC(0);
    BITCOLFUNC(1);
    BITCOLFUNC(2);
    BITCOLFUNC(3);
    BITCOLFUNC(4);
    BITCOLFUNC(5);
    BITCOLFUNC(6);
    BITCOLFUNC(7);

    long long unsigned int& getData()
    {
      return reinterpret_cast<long long unsigned int&>(mData[0]);
    }

    const long long unsigned int getData() const
    {
      return reinterpret_cast<const long long unsigned int&>(mData[0]);
    }

    operator long long unsigned int&()
    {
      return getData();
    }

    operator long long unsigned int() const
    {
      return getData();
    }

    BClassEightBitSet& operator[](unsigned int iIndex)
    {
      return mData[iIndex % 8];
    }

    const BClassEightBitSet& operator[](unsigned int iIndex) const
    {
      return mData[iIndex % 8];
    }

    BClassEightBitSet& operator[](int iIndex)
    {
      if (iIndex >= 0)
      {
        return mData[iIndex % 8];
      }
      else
      {
        return mData[8 + iIndex % 8];
      }
    }

    unsigned int countActiveBits()
    {
      unsigned int wBitCount = 0;
      for (unsigned int wi = 0; wi < 8; ++wi)
      {
        wBitCount += mData[wi].countActiveBits();
      }

      return wBitCount;
    }

    const BClassEightBitSet& operator[](int iIndex) const
    {
      if (iIndex >= 0)
      {
        return mData[iIndex % 8];
      }
      else
      {
        return mData[8 + iIndex % 8];
      }
    }

    friend std::ostream& operator<<(std::ostream& iCout, const BClassEightByteSet& iBits)
    {
      for (unsigned int wi = 0; wi < 8; ++wi)
      {
        iCout << iBits[wi] << std::endl;
      }

      return iCout;
    }

    unsigned int getSize()
    {
      return 8u;
    }

  private:
    std::array<BClassEightBitSet, 8> mData;
  };


  class BClassEightWordSet
  {
  public:

    BITTOPPLANEFUNC(0);
    BITTOPPLANEFUNC(1);
    BITTOPPLANEFUNC(2);
    BITTOPPLANEFUNC(3);
    BITTOPPLANEFUNC(4);
    BITTOPPLANEFUNC(5);
    BITTOPPLANEFUNC(6);
    BITTOPPLANEFUNC(7);

    BITSIDEPLANEFUNC(0);
    BITSIDEPLANEFUNC(1);
    BITSIDEPLANEFUNC(2);
    BITSIDEPLANEFUNC(3);
    BITSIDEPLANEFUNC(4);
    BITSIDEPLANEFUNC(5);
    BITSIDEPLANEFUNC(6);
    BITSIDEPLANEFUNC(7);

    BClassEightByteSet& operator[](unsigned int iIndex)
    {
      return mData[iIndex % 8];
    }

    const BClassEightByteSet& operator[](unsigned int iIndex) const
    {
      return mData[iIndex % 8];
    }

    BClassEightByteSet& operator[](int iIndex)
    {
      if (iIndex >= 0)
      {
        return mData[iIndex % 8];
      }
      else
      {
        return mData[8 + iIndex % 8];
      }
    }

    const BClassEightByteSet& operator[](int iIndex) const
    {
      if (iIndex >= 0)
      {
        return mData[iIndex % 8];
      }
      else
      {
        return mData[8 + iIndex % 8];
      }
    }

    unsigned int getSize()
    {
      return 8u;
    }

    unsigned int countActiveBits()
    {
      unsigned int wBitCount = 0;
      for (unsigned int wi = 0; wi < 8; ++wi)
      {
        wBitCount += mData[wi].countActiveBits();
      }

      return wBitCount;
    }


    friend std::ostream& operator<<(std::ostream& iCout, const BClassEightWordSet& iBits)
    {
      for (unsigned int wi = 0; wi < 8; ++wi)
      {
        for (unsigned int wj = 0; wj < 8; ++wj)
        {
          iCout << iBits[wj][wi] << "  ";
        }

        iCout << std::endl;
      }

      return iCout;
    }

  private:
    std::array<BClassEightByteSet, 8> mData;
  };
}


#undef BITMASK0
#undef BITMASK1
#undef BITMASK2
#undef BITMASK3
#undef BITMASK4
#undef BITMASK5
#undef BITMASK6
#undef BITMASK7

#undef BITFUNC
#undef BITCOLFUNC
#undef BITTOPPLANEFUNC
#undef BITSIDEPLANEFUNC

#endif
