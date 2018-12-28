#include "XSCBase/XSC_TEnum.hxx"

namespace XSC
{
  EnumTableProvider::EnumTableProvider()
  {

  }

  EnumTableProvider::~EnumTableProvider()
  {

  }

  EnumTableProvider& EnumTableProvider::getGlobal()
  {
    static EnumTableProvider* wProvider(nullptr);

    if (nullptr == wProvider)
    {
      wProvider = new EnumTableProvider();
    }

    return *wProvider;
  }
}