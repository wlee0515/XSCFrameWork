#ifndef XSC_TYPENAME_H
#define XSC_TYPENAME_H

#include <string>

namespace XSC
{
  namespace tools
  {
    template<typename T>
    inline std::string getTypeName_Imp(const T*)
    {
      return typeid(T).name();
    }

    inline std::string getTypeName_Imp(const std::string*)
    {
      return "string";
    }

    template<typename T>
    inline std::string getTypeName_Imp(const std::vector<T>*)
    {
      std::stringstream wName;
      wName << "vector<" << getTypeName_Imp(static_cast<T*>(nullptr)) << ">";
      return wName.str();
    }

    template<typename T, unsigned int TSize>
    inline std::string getTypeName_Imp(const std::array<T, TSize>*)
    {
      std::stringstream wName;
      wName << "array<" << static_cast<T*>(nullptr) << "," << TSize << ">";
      return wName.str();
    }

    template<typename T_Key, typename T_Val>
    std::string getTypeName_Imp(const std::map<T_Key, T_Val>*)
    {
      std::stringstream wName;
      wName << "map<" << static_cast<T_Key*>(nullptr) << "," << static_cast<T_Val*>(nullptr) << ">";
      return wName.str();
    }


    template<typename T_first, typename T_second>
    std::string getTypeName_Imp(const std::pair<T_first, T_second>*)
    {
      std::stringstream wName;
      wName << "pair<" << static_cast<T_first*>(nullptr) << "," << static_cast<T_second*>(nullptr) << ">";
      return wName.str();
    }

    template<typename T>
    inline std::string getTypeName()
    {
      return getTypeName_Imp(static_cast<T*>(nullptr));
    }

  }
}


#endif // !XSC_TYPENAME_H
