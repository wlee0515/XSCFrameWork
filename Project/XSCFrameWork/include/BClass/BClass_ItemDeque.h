#ifndef BCLASS_ITEM_DEQUE_H
#define BCLASS_ITEM_DEQUE_H

#include "BClass_UCharStr.h"
#include <array>


namespace XSC
{
  template<typename T_Item>
  class BClassItemDeque
  {
  public:

    BClassItemDeque();

    bool addItem(const T_Item& iMessage);
    bool getItemStack(std::vector<T_Item>& oMessageStack);

  private:
    bool mRouletteLock;
    std::array<std::vector<T_Item>, 2> mMsgRoulette;
  };

  template<typename T_Item>
  inline BClassItemDeque<T_Item>::BClassItemDeque()
  {
    mRouletteLock = true;
    for (unsigned int wi = 0; wi < mMsgRoulette.size(); ++wi)
    {
      mMsgRoulette[wi].clear();
      mMsgRoulette[wi].reserve(20);
    }
    mRouletteLock = false;
  }

  template<typename T_Item>
  inline bool BClassItemDeque<T_Item>::addItem(const T_Item& iMessage)
  {
    unsigned int wCount = 0;
    while (mRouletteLock == true)
    {
      if (wCount > 10)
      {
        return false;
      }
      wCount++;
    }

    if (mRouletteLock == false)
    {
      mRouletteLock = true;
      mMsgRoulette[1].emplace_back(iMessage);
      mRouletteLock = false;
    }
    return true;
  }

  template<typename T_Item>
  inline bool BClassItemDeque<T_Item>::getItemStack(std::vector<T_Item>& oMessageStack)
  {
    unsigned int wCount = 0;
    while (mRouletteLock == true)
    {
      if (wCount > 10)
      {
        return false;
      }
      wCount++;
    }

    oMessageStack.clear();
    oMessageStack.reserve(20);

    if (mRouletteLock == false)
    {
      mRouletteLock = true;
      mMsgRoulette[0].swap(oMessageStack);
      mMsgRoulette[1].swap(mMsgRoulette[0]);
      mRouletteLock = false;
    }
    return true;

  }
}
#endif