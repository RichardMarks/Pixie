//*** XMLObject.cpp ***

#include "XMLObject.h"
#include "XMLTextParser.h"
#include "Asset.h"

//*** XML_Element ***

XMLObject* XMLObject::XML_Element(StringId _name, const XMLAttributeList& _attributes) 
	{ 
	// If running a debug build, report unknown XML elements and stop the parsing,
	// if running a release build, just ignore them and continue parsing
#ifdef _DEBUG
	char buffer[256];
	SNPrintF(buffer,255,"Unexpected XML element \"%s\"",_name.GetString());
	Assert(false,buffer);
	return 0;
#else
	return this;
#endif
	} 


//*** XML_ElementEnd ***

void XMLObject::XML_ElementEnd(StringId _name) 
	{ 
	} 


//*** XML_Attribute ***

void XMLObject::XML_Attribute(StringId _name, const XMLVariant& _value) 
	{ 
	// If running a debug build, report unknown XML attributes
//#ifdef _DEBUG
//	char buffer[256];
//	SNPrintF(buffer,255,"Unexpected XML attribute \"%s\"",_name.GetString());
//	Assert(false,buffer);
//#endif
	} 


//*** XML_Text ***

void XMLObject::XML_Text(StringId _name, const char* text)
	{ 
	} 


//*** LoadXML ***

void XMLObject::LoadXML(const Asset& asset)
	{
	XMLTextParser parser;
	parser.Parse(asset,this);
	}


//*** SaveToStream ***

void XMLObject::SaveToStream(const XMLWriteStream& stream)
	{
	}


