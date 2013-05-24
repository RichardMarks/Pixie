/**
 * \class	XMLObject
 * 
 * \ingroup	xml
 * \brief	Base class for all objects that needs access to xml data.
 * \author	Mattias Gustavsson
 * 
 * By inheriting from this class, any class can be fed xml data by
 * the text or binary parsers, or being able to stream itself to a
 * XMLWriteStream.
 *
 * The XML class can be used to load and parse any xml file (both in 
 * binary and text format) and feed the data to any class that inherits
 * from XMLObject, using the XML::Parse method. It also defines some 
 * helper macros (xmlCase, xmlAttributeValie and xmlValue) to make it 
 * easier to acces the	parsed data. 
 * Include XML.h in any .cpp file where you need xml parsing 
 * functionality.
 *
 * \todo	Implement XmlWriteStream
 */
  
#ifndef __XMLObject_H__
#define __XMLObject_H__

// Includes
#include "StringId.h"
#include "XMLVariant.h"
#include "XMLAttributeList.h"
#include "XMLVariant.h"

// Forward declares
class XMLWriteStream;
class Asset;

// XMLObject
class XMLObject
	{
	public:
		/// Loads object data from file
		virtual void LoadXML(const Asset& asset);

		/// Save all of the objects data to the specified XML stream
		virtual void SaveToStream(const XMLWriteStream& stream);
	protected:
		friend class XMLTextParser;

		/// Event receiver for xml elements
		virtual XMLObject* XML_Element(StringId _name, const XMLAttributeList& _attributes);

		/// Event receiver for the end of xml elements
		virtual void XML_ElementEnd(StringId _name);

		/// Event receiver for xml attributes
		virtual void XML_Attribute(StringId _name, const XMLVariant& _value);
		
		/// Event receiver for xml text
		virtual void XML_Text(StringId _name, const char* text);

	};


/**
 * \ingroup xml
 * \brief Macro to emulate a SWITCH-CASE operation for strings
 * 
 * Macro to simulate a SWITCH-CASE operation for strings
 */
#define xmlCase(x) \
	static StringId _duplicate_xmlCase_statement_##x(#x);\
	if (_duplicate_xmlCase_statement_##x==_name)


/**
 * \ingroup xml
 * \brief Macro to retrieve the value of a named attribute
 * 
 * Macro to retrieve the value of a named attribute
 */
#define xmlAttributeValue(x) (_attributes.GetValue(x))


/**
 * \ingroup xml
 * \brief Macro to retrieve the value of the current attribute
 * 
 * Macro to retrieve the value of the current attribute
 */
#define xmlValue (_value)

#endif /* __XMLObject__ */
