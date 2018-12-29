#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_UDPClient.h"
#include "PracticalSockets/PracticalSocket.h"

#include "XSCEnum/XSC_ConnectionEnum.h"
#include  <thread>

namespace XSC {

  XSC_ConnectionMethod_UDPClient::XSC_ConnectionMethod_UDPClient(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup)
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

  eConnectionType XSC_ConnectionMethod_UDPClient::getConnectionType() const
  {
    return eUDPClient;
  }

  void XSC_ConnectionMethod_UDPClient::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    if ((0 == mPortNumber) || ("" == mServerAddress))
    {
      return;
    }

    UDPSocket* wSocket = nullptr;
    try {
      // Establish connection with the echo server
      wSocket = new UDPSocket();

      // Send the string to the echo server
      wSocket->sendTo(iRequest.c_str(), iRequest.length(), mServerAddress, mPortNumber);

    }
    catch (SocketException &e) {
      cerr << e.what() << endl;
      delete wSocket;
      wSocket = nullptr;
    }

    if (nullptr != wSocket)
    {
      std::thread wClientThread(&XSC_ConnectionMethod_UDPClient::UDPClientThread, this, wSocket, &iClient);
      wClientThread.detach();
      mThreadList[wSocket].swap(wClientThread);
    }

  }

  void XSC_ConnectionMethod_UDPClient::SClassSetup()
  {
    return XSC_ConnectionMethod::SClassSetup();
  }

  bool XSC_ConnectionMethod_UDPClient::SClassStart()
  {
    bool wState = XSC_ConnectionMethod::SClassStart();

    mThreadList.clear();

    return true;
  }

  bool XSC_ConnectionMethod_UDPClient::SClassStop()
  {
    std::map<UDPSocket*, std::thread> wThreadList;
    wThreadList.swap(mThreadList);

    for (std::map<UDPSocket*, std::thread>::iterator wIt = wThreadList.begin(); wIt != wThreadList.end(); ++wIt)
    {
      if (wIt->second.joinable())
      {
        if (nullptr != wIt->first)
        {
          delete wIt->first;
          wIt->second.join();
        }
      }
    }

    return XSC_ConnectionMethod::SClassStop();
  }

  void XSC_ConnectionMethod_UDPClient::UDPClientThread(UDPSocket * iClientSocket, XSC_ConnectionProxy::ISubscriber* iClient)
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
        LOG_TRACE(e.what());
        return;
      }
    }
    catch (SocketException &e) {
      LOG_TRACE(e.what());
      return;
    }
  }
}
