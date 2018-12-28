#ifndef BCLASS_COMMAND_DATA_H
#define BCLASS_COMMAND_DATA_H

#include "BClass_UCharStr.h"
#include <string>

namespace XSC
{
  template<typename T_CmdEnum>
  class BClassCmdData
  {
  public:
    BClassCmdData()
    {
      mConnectionPortID = 0;
      mConnectionSourceStr = "";
      mDeviceID = 0;
      mTargetID = 0;
      mCommandID = 0;
      mDataLength = 0;
      mAuxLength = 0;
      mStatusLength = 0;
      mTimeTick = 0;
      mAddress = 0;

      mDataString.clear();
      mAuxString.clear();
    }

    int mDeviceID;
    int mTargetID;
    int mCommandID;
    int mDataLength;
    int mAuxLength;
    int mStatusLength;
    int mTimeTick;
    int mAddress;
    int mConnectionPortID;
    T_CmdEnum mCommandEnum;
    std::string mConnectionSourceStr;
    XSC::BClassUCharStr mDataString;
    XSC::BClassUCharStr mAuxString;
    XSC::BClassUCharStr mStatusString;


    friend std::ostream& operator<<(std::ostream& iOut, const BClassCmdData& iMsg)
    {
      iOut << std::endl;
      iOut << "Device ID : " << iMsg.mDeviceID << std::endl;
      iOut << "Target ID : " << iMsg.mTargetID << std::endl;
      iOut << "Command ID : " << iMsg.mCommandID << std::endl;
      iOut << "Data Length : " << iMsg.mDataLength << std::endl;
      iOut << "Aux Byte Length : " << iMsg.mAuxLength << std::endl;
      iOut << "Status Byte Length : " << iMsg.mStatusLength << std::endl;
      iOut << "Time Tick : " << iMsg.mTimeTick << std::endl;
      iOut << "Address : " << iMsg.mAddress << std::endl;
      iOut << "Connection Port ID : " << iMsg.mConnectionPortID << std::endl;
      iOut << "Connection Source Str : " << iMsg.mConnectionSourceStr << std::endl;
      iOut << "Data Set : " << iMsg.mDataString << std::endl;
      iOut << "Aux Byte Set : " << iMsg.mAuxString << std::endl;
      iOut << "Status Byte Set : " << iMsg.mStatusString << std::endl;
      iOut << std::endl;

      return iOut;
    }
  };

}

#endif