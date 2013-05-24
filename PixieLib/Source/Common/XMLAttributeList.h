/**
 * \class	XMLAttributeList
 * 
 * \ingroup	xml
 * \brief	Contains a list of xml attributes.
 * \author	Mattias Gustavsson
 * 
 */
 
#ifndef __XMLAttributeList_H__
#define __XMLAttributeList_H__

// Includes
#include "Array.h"
#include "StringId.h"
#include "XMLVariant.h"

// Forward declares

// XMLAttributeList
class XMLAttributeList
	{
	public:
		void Add(StringId name, const XMLVariant& value);

		int GetCount() const;
		
		StringId GetName(int index) const;

		const XMLVariant& GetValue(int index) const;

		const XMLVariant& GetValue(StringId name) const;

		int GetIndex(StringId name) const;


	private:
		struct Attribute
			{
			StringId name; ///< Attributes name
			XMLVariant value; ///< Attributes value
			};

		Array<Attribute> attributes_;
	};

#endif /* __XMLAttributeList_H__ */
