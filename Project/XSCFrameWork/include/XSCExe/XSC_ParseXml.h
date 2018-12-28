#ifndef XSC_PARSE_XML_H
#define XSC_PARSE_XML_H

#include "XSCBase/XSC_Base.hxx"
#include "tinyXML/tinyxml2.h"

#include <string>
#include <list>

namespace XSC
{
  namespace Parse
  {
    namespace XMLKeys
    {
      const std::string cXMLFileTag("FileRef");
      const std::string cCreateTag("Create");
      const std::string cClassDefTag("DataDef");
      const std::string cArrayDefTag("ElmDef");

      const std::string cFileReferenceTag("FileName");
      const std::string cChildName("Name");
      const std::string cTypeName("TypeName");
      const std::string cValueTag("Value");

    }

    bool parseXMLFile(XSC::XSC_Object& iObjectRef, const std::string& iFileName, std::list<std::string>& iFileStack, const std::string& iSubNodeName = "");
    bool parseXMLNode(XSC::XSC_Object& iObjectRef, tinyxml2::XMLNode& iXMLNode, std::list<std::string>& iFileStack);
    tinyxml2::XMLNode* findXMLNodeWithName(tinyxml2::XMLNode& iXMLNode, const std::string& iSubNodeName);

  }
}
#endif