#include "XSCConnect/XSCConnectionMethod/XSC_ConnectionMethod_RS232.h"
#include "XSCBase/XSC_SingletonProvider.hxx"
#include "RS-232/rs232.h"

#include "XSCEnum/XSC_ConnectionEnum.h"
#include  <thread>

namespace XSC {

  XSC_ConnectionMethod_RS232::XSC_ConnectionMethod_RS232(const XSC_ConnectionMethod::XSC_ConnectionSetup& iConnectionSetup)
    : XSC_ConnectionMethod(iConnectionSetup)
    , mCOMPortNumber(-1)
    , mBaudRate(-1)
  {
    mCOMPortNumber = iConnectionSetup.getComNumber();
    mBaudRate = iConnectionSetup.getBaudRate();
  }

  eConnectionType XSC_ConnectionMethod_RS232::getConnectionType() const
  {
    return eRS232;
  }

  void XSC_ConnectionMethod_RS232::processClientRequest(XSC_ConnectionProxy::ISubscriber& iClient, const std::string& iRequest)
  {
    std::vector<unsigned char> wBuffer;
    wBuffer.resize(iRequest.size(), '\0');

    memcpy(&wBuffer[0], &iRequest[0], iRequest.size());

    wBuffer.push_back('\0');
    wBuffer.push_back('\0');
    RS232_SendBuf(mCOMPortNumber, &wBuffer[0], static_cast<int>(wBuffer.size()));
  }

  void XSC_ConnectionMethod_RS232::SClassSetup()
  {
    return XSC_ConnectionMethod::SClassSetup();
  }

  bool XSC_ConnectionMethod_RS232::SClassStart()
  {
    if (-1 == mCOMPortNumber)
    {
      LOG_TRACE("Invalid COM Number [" << mCOMPortNumber << "]");
    }

    if (0 == mBaudRate)
    {
      LOG_TRACE("Invalid Baudrate [" << mBaudRate << "]");
    }

    if (RS232_OpenComport(mCOMPortNumber, mBaudRate, "8N1"))
    {
      LOG_TRACE("Can not open COM port [" << mCOMPortNumber << "] with BaudRate [" << mBaudRate << "]");
      return false;
    }

    LOG_TRACE("Open COM port [" << mCOMPortNumber << "] with BaudRate [" << mBaudRate << "]");

    mCOMPortIsOpen = true;
    return true;
  }

  bool XSC_ConnectionMethod_RS232::SClassStop()
  {
    XSC::Singleton::getSingleton<XSC::XSC_Scheduler>().removeEntryPoint( *this);

    if (true == mCOMPortIsOpen)
    {
      mCOMPortIsOpen = false;
      RS232_CloseComport(mCOMPortNumber);

      LOG_TRACE("Close COM port [" << mCOMPortNumber << "] with BaudRate [" << mBaudRate << "]");
    }

    return XSC_ConnectionMethod::SClassStop();
  }

  void XSC_ConnectionMethod_RS232::entrypoint(double iDt)
  {

    if (true == mCOMPortIsOpen)
    {
      int wResponseLength = 0;
      const unsigned int wMaxLength = 4096;

      std::vector<unsigned char> wBuffer;
      std::vector<char> wSignedBuffer;
      wBuffer.resize(wMaxLength, '\0');

      wResponseLength = RS232_PollComport(mCOMPortNumber, &wBuffer[0], wBuffer.size());

      if (wResponseLength < wMaxLength)
      {
        wBuffer.resize(wResponseLength);
      }

      wBuffer.push_back('\0');

      if (wResponseLength > 0)
      {
        XSC_ConnectionProxy* wConnection = mConnectedProxy.getPtr();
        if (nullptr != wConnection)
        {
          wSignedBuffer.resize(wBuffer.size(), '\0');
          memcpy(&wSignedBuffer[0], &wBuffer[0], wBuffer.size());


          std::string wRequest( &wSignedBuffer[0], wResponseLength);
          std::vector<std::string> wResponse;

          bool wCompleted = wConnection->sendConnectionRequest(wRequest, wResponse);

          for (unsigned int wi = 0; wi < wResponse.size(); ++wi)
          {
            if (0 != wResponse[wi].length())
            {
              std::string& wMessage = wResponse[wi];

              wBuffer.resize(wMessage.size(), '\0');
              memcpy(&wBuffer[0], &wMessage[0], wMessage.size());

              wBuffer.push_back('\0');
              RS232_SendBuf(mCOMPortNumber, &wBuffer[0], static_cast<int>(wBuffer.size()));
            }
          }
        }
      }
    }
  }
}
