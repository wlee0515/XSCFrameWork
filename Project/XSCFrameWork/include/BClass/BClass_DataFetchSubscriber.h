#ifndef BCLASS_DATA_FETCH_SUBSCRIBER_H
#define BCLASS_DATA_FETCH_SUBSCRIBER_H

#include <set>

namespace XSC
{
  template<typename T_RequestItem, typename T_ReturnItem>
  class BClassDataFetchSubscriber
  {
  public:
    BClassDataFetchSubscriber();
    virtual ~BClassDataFetchSubscriber();

    void processDataFetchRequest(const T_RequestItem& iRequestItem, std::vector<T_ReturnItem>& oReplyItemList);

    virtual bool subscribeForDataFetch(BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>& iSubscriber);
    virtual bool unsubscribeForDataFetch(BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>& iSubscriber);

    virtual const std::set<BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>*>& getSubscriberList() const;

  private:
    std::set<BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>*> mSubscriberList;
  };

  template<typename T_RequestItem, typename T_ReturnItem>
  inline BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>::BClassDataFetchSubscriber()
  {
    mSubscriberList.clear();
  }

  template<typename T_RequestItem, typename T_ReturnItem>
  inline BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>::~BClassDataFetchSubscriber()
  {
    mSubscriberList.clear();
  }

  template<typename T_RequestItem, typename T_ReturnItem>
  inline void BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>::processDataFetchRequest(const T_RequestItem& iRequestItem, std::vector<T_ReturnItem>& oReplyItemList)
  {
    for (typename std::set<BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>*>::iterator wIt = mSubscriberList.begin(); wIt != mSubscriberList.end(); ++wIt)
    {
      if (*wIt != NULL)
      {
        std::vector<T_ReturnItem> wReturnItemList
        (*wIt)->fetchDataFromSubscriber(iRequestItem, wReturnItemList);
        if (0 != wReturnItemList.size())
        {
          oReplyItemList.insert(oReplyItemList.end(), wReturnItemList.begin(), wReturnItemList.end());
        }
      }
    }
  }

  template<typename T_RequestItem, typename T_ReturnItem>
  inline bool BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>::subscribeForDataFetch(BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>& iSubscriber)
  {
    if (this == &iSubscriber)
    {
      return false;
    }

    mSubscriberList.insert(&iSubscriber);
    return true;
  }

  template<typename T_RequestItem, typename T_ReturnItem>
  inline bool BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>::unsubscribeForDataFetch(BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>& iSubscriber)
  {
    mSubscriberList.erase(&iSubscriber);
    return true;
  }

  template<typename T_RequestItem, typename T_ReturnItem>
  inline const std::set<BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>*>& BClassDataFetchSubscriber<T_RequestItem, T_ReturnItem>::getSubscriberList() const
  {
    return mSubscriberList;
  }
}

#endif
