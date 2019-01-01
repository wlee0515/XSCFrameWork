#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_TCPClient.h"
#include "PracticalSockets/PracticalSocket.h"

#include "XSCEnum/XSC_ConnectionEnum.h"
#include  <thread>

namespace XSC {

  XSC_ConnectionMethod_TCPClient::XSC_ConnectionMethod_TCPClient(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup)
    : XSC_ConnectionMethod(iConnectionSetup)
    , mPortNumber(0)
    , mServerAddress("")
  {
    if (0 != iConnectionSetup.getPortNumber())
    {
      mPortNumber = iConnectionSetup.getPortNumber();
      mServerAddress = iConnectionSetup.getIPAddress();
    }
  }

  eConnectionType XSC_ConnectionMethod_TCPClient::getConnectionType() const
  {
    return eTCPClient;
  }

  void XSC_ConnectionMethod_TCPClient::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    if ((0 == mPortNumber) || ("" == mServerAddress))
    {
      return;
    }

    TCPSocket* wSocket = nullptr;
    try {
      // Establish connection with the echo server
      wSocket = new TCPSocket(mServerAddress, mPortNumber);

      // Send the string to the echo server
      wSocket->send(iRequest.c_str(), iRequest.length());

    }
    catch (SocketException &e) {
      LOG_TRACE("TCP Client at Port [" << mPortNumber << "] :" << e.what());
      delete wSocket;
      wSocket = nullptr;
    }

    if (nullptr != wSocket)
    {
      std::thread wClientThread(&XSC_ConnectionMethod_TCPClient::TCPClientThread, this, wSocket, &iClient);
      wClientThread.detach();
      mThreadList[wSocket].swap(wClientThread);
    }

  }

  void XSC_ConnectionMethod_TCPClient::SClassSetup()
  {
    return XSC_ConnectionMethod::SClassSetup();
  }

  bool XSC_ConnectionMethod_TCPClient::SClassStart()
  {
    bool wState = XSC_ConnectionMethod::SClassStart();

    mThreadList.clear();

    return true;
  }

  bool XSC_ConnectionMethod_TCPClient::SClassStop()
  {
    std::map<TCPSocket*, std::thread> wThreadList;
    wThreadList.swap(mThreadList);

    for (std::map<TCPSocket*, std::thread>::iterator wIt = wThreadList.begin(); wIt != wThreadList.end(); ++wIt)
    {
      if (wIt->second.joinable())
      {
        if (nullptr != wIt->first)
        {
          std::cout << "Deleting Socket" << std::endl;
          delete wIt->first;
          std::cout << "Waiting for Thread" << std::endl;
          wIt->second.join();
          std::cout << "Thread join" << std::endl;
        }
      }
    }

    return XSC_ConnectionMethod::SClassStop();
  }

  void XSC_ConnectionMethod_TCPClient::TCPClientThread(TCPSocket * iClientSocket, XSC_ConnectionProxy::ISubscriber* iClient)
  {
    if((NULL == iClientSocket) || (NULL == iClient))
    {
      return;
    }

    XSC_ConnectionProxy* wConnection = mConnectedProxy.getPtr();
    if (nullptr == wConnection)
    {
      return;
    }

    const unsigned int cMsgBufferMax = 1000000;
    char wBuffer[cMsgBufferMax];
    int wResponseLength;

    try {

      while (NULL != iClientSocket)
      {
        if ((wResponseLength = iClientSocket->recv(wBuffer, cMsgBufferMax)) > 0)
        {
          if (wResponseLength > cMsgBufferMax)
          {
            break;
          }

          wBuffer[wResponseLength] = '\0';

          std::string wRequest(wBuffer, wResponseLength);
          std::string wResponse;

          bool wCompleted = iClient->processConnectionRequest(wRequest, wResponse);

          if (0 != wResponse.length())
          {
            iClientSocket->send(wResponse.data(), static_cast<int>(wResponse.size()));
          }

          if (true == wCompleted)
          {
            std::string wEnd = "";
            iClientSocket->send(wEnd.data(), static_cast<int>(wEnd.size()));
            break;
          }
        }
        else
        {
          break;
        }
      }

      try {
        delete iClientSocket;
      }
      catch (SocketException &e) {
        LOG_TRACE("TCP Client at Port [" << mPortNumber << "] :" << e.what());
        return;
      }
    }
    catch (SocketException &e) {
      LOG_TRACE("TCP Client at Port [" << mPortNumber << "] :" << e.what());
      return;
    }
  }
}
