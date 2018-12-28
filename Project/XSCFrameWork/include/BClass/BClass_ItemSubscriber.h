#ifndef BCLASS_ITEM_SUBSCRIBER_H
#define BCLASS_ITEM_SUBSCRIBER_H

#include "BClass_ItemDeque.h"
#include <set>

namespace XSC
{
  template<typename T_Item>
  class BClassItemSubscriber
  {
  public:
    BClassItemSubscriber();
    virtual ~BClassItemSubscriber();

    virtual void pushToSubscriber(const T_Item& iItem);
    virtual void pushToSource(const T_Item& iItem);
    virtual void processDataFetchRequest(const T_Item& iRequestItem, std::vector<T_Item>& oReplyItemList);

    virtual bool subscribeForPushBack(BClassItemSubscriber<T_Item>& iSubscriber);
    virtual bool unsubscribeForPushBack(BClassItemSubscriber<T_Item>& iSubscriber);

    virtual const std::set<BClassItemSubscriber<T_Item>*>& getSubscriberList() const;

  private:
    virtual bool registerAsParent(BClassItemSubscriber<T_Item>* iParent);
    virtual BClassItemSubscriber<T_Item>* getParent() const;

    BClassItemSubscriber<T_Item>* mParent;
    std::set<BClassItemSubscriber<T_Item>*> mSubscriberList;
  };

  template<typename T_Item>
  class BClassItemSubscriberAccumulator : public BClassItemSubscriber<T_Item>
  {
  public:
    void getRcvdMgStack(std::vector<T_Item>& iItemStack)
    {
      mMsgStack.getItemStack(iItemStack);
    }

    virtual void pushToSubscriber(const T_Item& iItem)
    {
      mMsgStack.addItem(iItem);
    }

  private:
    BClassItemDeque<T_Item> mMsgStack;
  };

  template<typename T_Item>
  inline BClassItemSubscriber<T_Item>::BClassItemSubscriber()
    : mParent(NULL)
  {
    mSubscriberList.clear();
  }

  template<typename T_Item>
  inline BClassItemSubscriber<T_Item>::~BClassItemSubscriber()
  {
    if (NULL != mParent)
    {
      mParent->unsubscribeForPushBack(*this);
    }

    for (typename std::set<BClassItemSubscriber<T_Item>*>::iterator wIt = mSubscriberList.begin(); wIt != mSubscriberList.end(); ++wIt)
    {
      if (*wIt != NULL)
      {
        (*wIt)->registerAsParent(NULL);
      }
    }
  }

  template<typename T_Item>
  inline void BClassItemSubscriber<T_Item>::pushToSubscriber(const T_Item& iItem)
  {
    for (typename std::set<BClassItemSubscriber<T_Item>*>::iterator wIt = mSubscriberList.begin(); wIt != mSubscriberList.end(); ++wIt)
    {
      if (*wIt != NULL)
      {
        (*wIt)->pushToSubscriber(iItem);
      }
    }
  }

  template<typename T_Item>
  inline void BClassItemSubscriber<T_Item>::pushToSource(const T_Item& iMessage)
  {
    if (NULL != mParent)
    {
      mParent->pushToSource(iMessage);
    }
  }

  template<typename T_Item>
  inline void BClassItemSubscriber<T_Item>::processDataFetchRequest(const T_Item& iRequestItem, std::vector<T_Item>& oReplyItemList)
  {
    for (typename std::set<BClassItemSubscriber<T_Item>*>::iterator wIt = mSubscriberList.begin(); wIt != mSubscriberList.end(); ++wIt)
    {
      if (*wIt != NULL)
      {
        std::vector<T_Item> wReturnItemList;
        (*wIt)->processDataFetchRequest(iRequestItem, wReturnItemList);
        if (0 != wReturnItemList.size())
        {
          oReplyItemList.reserve(oReplyItemList.size() + wReturnItemList.size());
          oReplyItemList.insert(oReplyItemList.end(), wReturnItemList.begin(), wReturnItemList.end());
        }
      }
    }
  }

  template<typename T_Item>
  inline bool BClassItemSubscriber<T_Item>::subscribeForPushBack(BClassItemSubscriber<T_Item>& iSubscriber)
  {
    if (this == &iSubscriber)
    {
      return false;
    }

    if (NULL != iSubscriber.getParent())
    {
      return  false;
    }

    mSubscriberList.insert(&iSubscriber);
    iSubscriber.registerAsParent(this);
    return true;
  }

  template<typename T_Item>
  inline bool BClassItemSubscriber<T_Item>::unsubscribeForPushBack(BClassItemSubscriber<T_Item>& iSubscriber)
  {

    mSubscriberList.erase(&iSubscriber);

    if (this == iSubscriber.getParent())
    {
      iSubscriber.registerAsParent(NULL);
    }
    else
    {
      return false;
    }

    return true;
  }

  template<typename T_Item>
  inline const std::set<BClassItemSubscriber<T_Item>*>& BClassItemSubscriber<T_Item>::getSubscriberList() const
  {
    return mSubscriberList;
  }

  template<typename T_Item>
  inline bool BClassItemSubscriber<T_Item>::registerAsParent(BClassItemSubscriber<T_Item>* iParent)
  {
    if (this == iParent)
    {
      return false;
    }

    mParent = iParent;

    return true;
  }

  template<typename T_Item>
  inline BClassItemSubscriber<T_Item>* BClassItemSubscriber<T_Item>::getParent() const
  {
    return mParent;
  }
}

#endif
