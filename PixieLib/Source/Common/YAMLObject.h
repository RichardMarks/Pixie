/**
 * \class	YAMLObject
 * 
 * \ingroup	yaml
 * \brief	Base class for all objects that needs access to yaml data.
 * \author	Mattias Gustavsson
 *
 *
 */
  
#ifndef __YAMLObject_H__
#define __YAMLObject_H__

// Includes
#include "XMLAttributeList.h"
#include "XMLVariant.h"

// Forward declares
class Asset;

// YAMLObject
class YAMLObject
	{
	public:
		/// Loads object data from file
		virtual bool LoadYAML(const Asset& asset);

	private:
		virtual YAMLObject* YAML_Element(StringId _name, bool _sequence);

		virtual void YAML_ElementEnd(StringId _name, bool _sequence);

		virtual void YAML_Attribute(StringId _name, const XMLVariant& _value);

	};

#endif /* __YAMLObject__ */
