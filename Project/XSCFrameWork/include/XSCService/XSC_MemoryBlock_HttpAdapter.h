#ifndef XSC_MEMORYBLOCK_HTTP_ADAPTER_H
#define XSC_MEMORYBLOCK_HTTP_ADAPTER_H

#include "XSCBase/XSC_Common.hxx"
#include "XSCServiceProvider/XSC_HttpProvider.h"
#include "BClass/BClass_JSON.h"

namespace XSC
{
  class XSC_MemoryBlockHttpAdapter : public XSC::XSC_HttpProvider::UriService
  {
  public:

    enum {
      eBinaryData = 1,
      eTextData = 2,
      eJSONData = 3
    };

    struct PacketHeader {
      PacketHeader();
      const char mPacketHead[sizeof(unsigned int)*4];
      unsigned int mHeaderLength;
      unsigned int mDataMapVersion;
      unsigned int mDataType;
      unsigned int mDataLength;
    };

    XSC_MemoryBlockHttpAdapter();

    virtual void SClassSetup();
    virtual bool SClassStart();

    virtual bool processHttpPOSTMethod(const std::string& iURI, const std::string& iRequest, std::string& oResponse);

  private:

    bool createDataMapResponse(std::string& oResponse);
    bool createDataBlockResponse(std::string& oResponse);
    bool createJSONSetDataListResponse(BClassJSON::JSONObjMap& iRequest, std::string& oResponse);
  };

}

#endif