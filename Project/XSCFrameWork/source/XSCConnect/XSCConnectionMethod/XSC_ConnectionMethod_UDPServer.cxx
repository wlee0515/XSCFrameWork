#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_UDPServer.h"
#include "PracticalSockets/PracticalSocket.h"

#include "XSCEnum/XSC_ConnectionEnum.h"
#include  <thread>

namespace XSC {

  XSC_ConnectionMethod_UDPServer::XSC_ConnectionMethod_UDPServer(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup)
    : XSC_ConnectionMethod(iConnectionSetup)
    , mPortNumber(8080)
  {
    if (0 != iConnectionSetup.getPortNumber())
    {
      mPortNumber = iConnectionSetup.getPortNumber();
    }
  }

  eConnectionType XSC_ConnectionMethod_UDPServer::getConnectionType() const
  {
    return eUDPServer;
  }

  void XSC_ConnectionMethod_UDPServer::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    // send message to protocol, not available in Server Mode
  }

  void XSC_ConnectionMethod_UDPServer::SClassSetup()
  {
    return XSC_ConnectionMethod::SClassSetup();
  }

  bool XSC_ConnectionMethod_UDPServer::SClassStart()
  {
    bool wState = XSC_ConnectionMethod::SClassStart();

    if (true == wState)
    {
      try {
        if (NULL == mSocket)
        {
          mSocket = new UDPSocket(mPortNumber);
        }
      }
      catch (SocketException &e) {
        LOG_TRACE("UDP Server at Port [" << mPortNumber << "] :" << e.what());
        return false;
      }

      mMainServerThread = std::thread(&XSC_ConnectionMethod_UDPServer::UDPServerThread, this);

      return true;
    }

    return false;
  }

  bool XSC_ConnectionMethod_UDPServer::SClassStop()
  {
    if (nullptr != mSocket)
    {
      delete mSocket;
      mSocket = nullptr;
    }

    mMainServerThread.join();
    return XSC_ConnectionMethod::SClassStop();
  }

  void XSC_ConnectionMethod_UDPServer::UDPServerThread()
  {
    try {
      if (NULL == mSocket)
      {
        mSocket = new UDPSocket(mPortNumber);
      }

      const unsigned int cMsgBufferMax = 1000000;
      char wBuffer[cMsgBufferMax] = "";
      int wResponseLength = 0;
      std::string wSourceAddress = "";
      unsigned short wSourcePort = 0;

      try {

        while (1)
        {

          wResponseLength = mSocket->recvFrom(wBuffer, wResponseLength, wSourceAddress, wSourcePort);

          if ((cMsgBufferMax < wResponseLength) || (0 == wResponseLength))
          {
            continue;
          }

          XSC_ConnectionProxy* wConnection = mConnectedProxy.getPtr();
          if (nullptr != wConnection)
          {
            std::string wRequest(wBuffer, wResponseLength);
            std::vector<std::string> wResponse;

            bool wCompleted = wConnection->sendConnectionRequest(wRequest, wResponse);

            for (unsigned int wi = 0; wi < wResponse.size(); ++wi)
            {
              if (0 != wResponse[wi].length())
              {
                mSocket->sendTo(wResponse[wi].data(), static_cast<int>(wResponse[wi].size()), wSourceAddress, wSourcePort);
              }
            }
          }
        }
      }
      catch (SocketException &e) {
        LOG_TRACE("UDP Server at Port [" << mPortNumber << "] :" << e.what());
      }
    }
    catch (SocketException &e) {
      LOG_TRACE("UDP Server at Port [" << mPortNumber << "] :" << e.what());
      return;
    }

    if (nullptr != mSocket)
    {
      delete mSocket;
      mSocket = nullptr;
    }
  }
}
