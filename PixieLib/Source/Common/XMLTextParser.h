/**
 * \class	XMLTextParser
 * 
 * \ingroup	xml
 * \brief	Parses a text xml file.
 * \author	Mattias Gustavsson
 * 
 * This class parses a text xml file. 
 */
 
#ifndef __XMLTextParser_H__
#define __XMLTextParser_H__

// Includes
#include "Array.h"
#include "StringId.h"
#include "DynamicBuffer.h"

// Forward declares
class XMLObject;
class Asset;

// XMLTextParser
class XMLTextParser
	{
	public:

		/**
		 * Loads and parses the XML file, feeding data to the xmlObject
		 */
		void Parse(
			const Asset& asset, 
			XMLObject* xmlObject); 

	private:
		/**
		 * Pushes an xml object onto the top of the stack
		 */
		void PushXMLObject(XMLObject* object, StringId name); 

		/**
		 * Removes an xml object from the top of the stack, returns false if there isn't one
		 */
		bool PopXMLObject(); 

		/**
		 * Returns the XML object on the top, 0 if there isn't one
		 */
		XMLObject* TopXMLObject(); 

		/**
		 * Returns the tag name on the top, 0 if there isn't one
		 */
		StringId TopName(); 


	private:
		// XMLObjectStackEntry
		class XMLObjectStackEntry
			{
			public:
				XMLObject* object;
				StringId name;
				DynamicBuffer bufferedText;
			
			};
		Array<XMLObjectStackEntry> xmlObjects_; ///< XML object stack, pushed to and popped from during parsing

	};

#endif /* __XMLTextParser_H__ */
