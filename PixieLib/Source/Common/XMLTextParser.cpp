//*** XMLTextParser.cpp ***

#include "XMLTextParser.h"
#include "XMLAttributeList.h"
#include "XMLObject.h"
#include "XMLVariant.h"

#include "Debug.h"
#include "Asset.h"

#include "irrXML/irrXML.h"
using namespace irr; // irrXML is located in the namespace irr::io
using namespace io;


class FileReadCallBack_Asset: public IFileReadCallBack
{
public:
	FileReadCallBack_Asset(const Asset& asset)
		{
		asset_=&asset;
		bool result=asset_->Open();
		// Report missing file
		#ifdef _DEBUG
		if (!result)
			{
			const char* filename=asset.GetFilename().GetString();
			if (filename)
				{
				char errorMessage[1024];
				SNPrintF(errorMessage,1024,"File not found: %s",filename);
				Assert(false,errorMessage);
				}
			else
				{
				Assert(false,"An asset could not be accessed.");
				}
			}
		#endif
		}
	
	~FileReadCallBack_Asset()
		{
		asset_->Close();
		}

private:
	virtual int read(void* buffer, int sizeToRead)
		{
		return asset_->Read(static_cast<char*>(buffer), sizeToRead);
		}

	virtual int getSize()
		{
		return asset_->GetSize();
		}
private:
	const Asset* asset_;
};


//*** PushXMLObject ***

void XMLTextParser::PushXMLObject(XMLObject* object, StringId name)
	{
	// Create a new stack entry, and make it the topmost one
	XMLObjectStackEntry& entry=xmlObjects_.Add(XMLObjectStackEntry());
	entry.object=object;
	entry.name=name;
	;
	}


//*** PopXMLObject ***

bool XMLTextParser::PopXMLObject()
	{
	// Check if we have any entries in the stack
	if (xmlObjects_.GetItemCount()>0)
		{
		xmlObjects_.RemoveLast();
		return true;
		}
	else
		{
		return false;
		}
	}


//*** TopXMLObject ***

XMLObject* XMLTextParser::TopXMLObject()
	{
	// Check if we have any entries in the stack
	if (xmlObjects_.GetItemCount()>0)
		{
		// Return the topmost object
		return xmlObjects_.Get(xmlObjects_.GetItemCount()-1).object;
		}
	else
		{
		// If stack is empty, return 0
		return 0;
		}
	}


//*** TopName ***

StringId XMLTextParser::TopName()
	{
	// Check if we have any entries in the stack
	if (xmlObjects_.GetItemCount()>0)
		{
		// Return the topmost object
		return xmlObjects_.Get(xmlObjects_.GetItemCount()-1).name;
		}
	else
		{
		// If stack is empty, return 0
		return 0;
		}
	}


//*** Parse ***

void XMLTextParser::Parse(const Asset& asset,XMLObject* xmlObject)
	{
	// Clear the stack
	xmlObjects_.Clear();

	// Push xmlObject on top of the stack
	PushXMLObject(xmlObject,"");

	// create the reader using one of the factory functions
	FileReadCallBack_Asset xmlFileCallback(asset);
	IrrXMLReader* xml = createIrrXMLReader(&xmlFileCallback);
	bool firstElement=true; // We don't want an Element event sent to the top level object

	// parse the file until end reached
	while(xml && xml->read())
		{
		switch(xml->getNodeType())
			{
			case EXN_ELEMENT:
				{
				// Read element name
				StringId elementName(xml->getNodeName());

				// Read all attributes
				XMLAttributeList attributes;
				for (int i=0; i<xml->getAttributeCount(); ++i)
					{
					attributes.Add(xml->getAttributeName(i),xml->getAttributeValue(i));
					}

				// If the topmost object of the stack is valid, it should receive an event
				if (TopXMLObject() && !firstElement)
					{
					XMLObject* newObject=TopXMLObject();
					newObject=TopXMLObject()->XML_Element(elementName,attributes);
					// Push the received object onto the stack, regardless of whether it is valid or not (because it will be popped later)
					PushXMLObject(newObject,elementName);
					}
				firstElement=false; // The remaining elements should be sent

					
				// Send XML_Attribute events to the topmost object if it is valid
				if (TopXMLObject())
					{
					// Loop through all attributes
					for (int i=0; i<attributes.GetCount(); ++i)
						{
						// Send XML_Attribute event
						TopXMLObject()->XML_Attribute(attributes.GetName(i),attributes.GetValue(i));
						}
					}

				
				if (xml->isEmptyElement())
					{
					// Cache the topmost object of the stack
					XMLObject* object=TopXMLObject();

						
					// Send the XML_ElementEnd event to the topmost object, if it is valid
					if (object)
						{
						object->XML_ElementEnd(elementName);
						}

					// Remove it from the stack
					PopXMLObject();

					// Get the parent object from the stack
					object=TopXMLObject();

					// Send the XML_ElementEnd event to the parent object, if it is valid
					if (object)
						{
						object->XML_ElementEnd(elementName);
						}
					}
					
				} break;

			case EXN_ELEMENT_END:
				{
				// Read element name
				StringId elementName(xml->getNodeName());

				// Cache the topmost object of the stack
				XMLObject* object=TopXMLObject();

				// Get buffered text
				DynamicBuffer* bufferedText=0;
				if (xmlObjects_.GetItemCount()>0)
					{
					bufferedText=&(xmlObjects_.Get(xmlObjects_.GetItemCount()-1).bufferedText);
					}

				// Send Text event
				if (bufferedText && object && bufferedText->GetSize()>0)
					{	
					char terminator='\0';
					bufferedText->Write(&terminator);
					object->XML_Text(elementName,static_cast<const char*>(bufferedText->GetPointer()));
					bufferedText->Clear(false);
					}

					
				// Send the XML_ElementEnd event to the topmost object, if it is valid
				if (object)
					{
					object->XML_ElementEnd(elementName);
					}

				// Remove it from the stack
				PopXMLObject();
				
				// Get the parent object from the stack
				object=TopXMLObject();

				// Send the XML_ElementEnd event to the parent object, if it is valid
				if (object)
					{
					object->XML_ElementEnd(elementName);
					}
								
				} break;


			case EXN_TEXT:
				{
				const char* text=xml->getNodeData();
				int length=StrLen(text);
				if (length<=0)
					{
					delete xml;
					return;
					}

				// Get buffered text
				DynamicBuffer* bufferedText=0;
				if (xmlObjects_.GetItemCount()>0)
					{
					bufferedText=&(xmlObjects_.Get(xmlObjects_.GetItemCount()-1).bufferedText);
					}

				if (!bufferedText)
					{
					delete xml;
					return;
					}
				
				bufferedText->Write(text,length);
				} break;

			case EXN_COMMENT:
				{
				} break;

			case EXN_CDATA:
				{
				} break;
			}
		}

	// delete the xml parser after usage
	delete xml;
	}

