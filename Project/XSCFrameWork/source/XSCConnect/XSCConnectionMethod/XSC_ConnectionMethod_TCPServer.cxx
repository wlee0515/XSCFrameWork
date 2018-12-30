#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_TCPServer.h"
#include "PracticalSockets/PracticalSocket.h"

#include "XSCEnum/XSC_ConnectionEnum.h"
#include  <thread>

namespace XSC {

  XSC_ConnectionMethod_TCPServer::XSC_ConnectionMethod_TCPServer(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup)
    : XSC_ConnectionMethod(iConnectionSetup)
    , mPortNumber(8080)
  {
    if (0 != iConnectionSetup.getPortNumber())
    {
      mPortNumber = iConnectionSetup.getPortNumber();
    }
  }

  eConnectionType XSC_ConnectionMethod_TCPServer::getConnectionType() const
  {
    return eTCPServer;
  }

  void XSC_ConnectionMethod_TCPServer::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    // send message to protocol, not available in Server Mode
  }

  void XSC_ConnectionMethod_TCPServer::SClassSetup()
  {
    return XSC_ConnectionMethod::SClassSetup();
  }

  bool XSC_ConnectionMethod_TCPServer::SClassStart()
  {
    LOG_DISPLAY("TCP at Port [" << mPortNumber << "] : TCP Server Start");

    bool wState = XSC_ConnectionMethod::SClassStart();

    LOG_DISPLAY("TCP at Port [" << mPortNumber << "] : TCP Server Parent Start end");
    if (true == wState)
    {
      try {
        if (NULL == mSocket)
        {

          LOG_DISPLAY("TCP at Port [" << mPortNumber << "] : TCP Server Create Socket");
          mSocket = new TCPServerSocket(mPortNumber);
          LOG_DISPLAY("TCP at Port [" << mPortNumber << "] : TCP Server Create Socket End");
        }
      }
      catch (SocketException &e) {
        LOG_TRACE(e.what());
        return false;
      }

      LOG_DISPLAY("TCP at Port [" << mPortNumber << "] : TCP Server Create Thread");
      mMainServerThread = std::thread(&XSC_ConnectionMethod_TCPServer::TCPServerThread, this);

      LOG_DISPLAY("TCP at Port [" << mPortNumber << "] : TCP Server Thread Created");
      return true;
    }

    LOG_DISPLAY("TCP at Port [" << mPortNumber << "] : TCP Server Start Failed");
    return false;
  }

  bool XSC_ConnectionMethod_TCPServer::SClassStop()
  {
    if (nullptr != mSocket)
    {
      delete mSocket;
      mSocket = nullptr;
    }

    mMainServerThread.join();
    return XSC_ConnectionMethod::SClassStop();
  }

  void XSC_ConnectionMethod_TCPServer::TCPServerThread()
  {
    try {
      if (NULL == mSocket)
      {
        mSocket = new TCPServerSocket(mPortNumber);
      }

      try {

        while (1)
        {
          LOG_DISPLAY("Accepting TCP at Port [" << mPortNumber << "]");
          TCPSocket *wClient = mSocket->accept();
          std::thread wClientThread(&XSC_ConnectionMethod_TCPServer::TCPClientThread, this, wClient);
          wClientThread.detach();

        }
      }
      catch (SocketException &e) {
        LOG_TRACE(e.what());
      }
    }
    catch (SocketException &e) {
      LOG_TRACE(e.what());
      return;
    }

    if (nullptr != mSocket)
    {
      delete mSocket;
      mSocket = nullptr;
    }
  }

  void XSC_ConnectionMethod_TCPServer::TCPClientThread(TCPSocket * iClientSocket)
  {
    if (NULL == iClientSocket)
    {
      LOG_ERROR("Client Socket for TCP server at Port [" << mPortNumber << "] not available");
      return;
    }

    XSC_ConnectionProxy* wConnection = mConnectedProxy.getPtr();
    if (nullptr == wConnection)
    {
      LOG_ERROR("Connection Proxy for TCP server at Port [" << mPortNumber << "] not available");
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
          std::vector<std::string> wResponse;

          bool wCompleted = wConnection->sendConnectionRequest(wRequest, wResponse);

          for (unsigned int wi = 0; wi < wResponse.size(); ++wi)
          {
            if (0 != wResponse[wi].length())
            {
              iClientSocket->send(wResponse[wi].data(), static_cast<int>(wResponse[wi].size()));
            }
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
