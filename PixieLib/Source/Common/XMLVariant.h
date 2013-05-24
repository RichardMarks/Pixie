/**
 * \class	XMLVariant
 * 
 * \ingroup	xml
 * \brief	Variant data type_ for xml data
 * \author	Mattias Gustavsson
 * 
 * This data type_ is used to encapsulate data that is parsed by the
 * xml system, and enables the class interested in the data to
 * retrieve the data as any type it wants. The XMLVariant will perform
 * any necessary conversions, or if the value it is storing is already
 * in the requested format, it will simply return it, introducing only
 * a small overhead.
 *
 * \todo	Move implementation to .cpp file
 */

#ifndef __XMLVariant_H__
#define __XMLVariant_H__

// Includes
#include "StandardLibrary.h"
#include "Debug.h"


// Forward declares

// XMLVariant
class XMLVariant
	{
	friend class XMLAttribute;
	friend class XMLTextParser;
	public:
		enum TYPES
			{
			TYPE_STRING			= 0,
			TYPE_CHAR			= 1,
			TYPE_SHORT			= 2,
			TYPE_INT			= 3,
			TYPE_LONG			= 4,
			TYPE_UNSIGNEDCHAR	= 5,
			TYPE_UNSIGNEDSHORT	= 6,
			TYPE_UNSIGNEDINT	= 7,
			TYPE_UNSIGNEDLONG	= 8,
			TYPE_FLOAT			= 9,
			TYPE_DOUBLE			= 10,
			TYPE_BOOL			= 11,
			TYPE_NOTHING		= 255,
			};

		// Destructor
		 inline ~XMLVariant()
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			};

		 // Constructors

		 inline XMLVariant()
			{
			type_=TYPE_NOTHING;
			};
		

		 inline XMLVariant(const XMLVariant& value)
			{
			type_=value.type_;
			if (value.type_==TYPE_STRING)
				{
				value_.stringVal=StrDup(value.value_.stringVal);
				}
			else
				{
				value_=value.value_;
				}
			};
		
		 inline XMLVariant(char value)
			{
			value_.charVal=value;
			type_=TYPE_CHAR;
			};

		 inline XMLVariant(short value)
			{
			value_.shortVal=value;
			type_=TYPE_SHORT;
			};

		 inline XMLVariant(int value)
			{
			value_.intVal=value;
			type_=TYPE_INT;
			};

		 inline XMLVariant(long value)
			{
			value_.longVal=value;
			type_=TYPE_LONG;
			};

		 inline XMLVariant(unsigned char value)
			{
			value_.ucharVal=value;
			type_=TYPE_UNSIGNEDCHAR;
			};

		 inline XMLVariant(unsigned short value)
			{
			value_.ushortVal=value;
			type_=TYPE_UNSIGNEDSHORT;
			};

		 inline XMLVariant(unsigned int value)
			{
			value_.uintVal=value;
			type_=TYPE_UNSIGNEDINT;
			};

		 inline XMLVariant(unsigned long value)
			{
			value_.ulongVal=value;
			type_=TYPE_UNSIGNEDLONG;
			};
		
		 inline XMLVariant(float value)
			{
			value_.floatVal=value;
			type_=TYPE_FLOAT;
			};

		 inline XMLVariant(double value)
			{
			value_.doubleVal=value;
			type_=TYPE_DOUBLE;
			};

		 inline XMLVariant(bool value)
			{
			value_.boolVal=value;
			type_=TYPE_BOOL;
			};

		 inline XMLVariant(char* value)
			{
			value_.stringVal=StrDup(value);
			type_=TYPE_STRING;
			};

		 inline XMLVariant(const char* value)
			{
			value_.stringVal=StrDup(value);
			type_=TYPE_STRING;
			};

		 // "Set" operators
		 inline const XMLVariant& operator=(const XMLVariant& value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			type_=value.type_;
			if (value.type_==TYPE_STRING)
				{
				value_.stringVal=StrDup(value.value_.stringVal);
				}
			else
				{
				value_=value.value_;
				}

			return *this;
			};
		

		 inline char operator=(char value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.charVal=value;
			type_=TYPE_CHAR;		
			return value;
			};

		 inline short operator=(short value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.shortVal=value;
			type_=TYPE_SHORT;
			return value;
			};

		 inline int operator=(int value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.intVal=value;
			type_=TYPE_INT;
			return value;
			};

		 inline long operator=(long value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.longVal=value;
			type_=TYPE_LONG;
			return value;
			};

		 inline unsigned char operator=(unsigned char value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.ucharVal=value;
			type_=TYPE_UNSIGNEDCHAR;
			return value;
			};

		 inline unsigned short operator=(unsigned short value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.ushortVal=value;
			type_=TYPE_UNSIGNEDSHORT;
			return value;
			};

		 inline unsigned int operator=(unsigned int value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.uintVal=value;
			type_=TYPE_UNSIGNEDINT;
			return value;
			};

		 inline unsigned long operator=(unsigned long value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.ulongVal=value;
			type_=TYPE_UNSIGNEDLONG;
			return value;
			};
		
		 inline float operator=(float value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.floatVal=value;
			type_=TYPE_FLOAT;
			return value;
			};

		 inline double operator=(double value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.doubleVal=value;
			type_=TYPE_DOUBLE;
			return value;
			};

		 inline bool operator=(bool value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.boolVal=value;
			type_=TYPE_BOOL;
			return value;
			};

		 inline char* operator=(char* value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.stringVal=StrDup(value);
			type_=TYPE_STRING;
			return value;
			};

		 inline const char* operator=(const char* value)
			{
			if (type_==TYPE_STRING)
				{
				Free(value_.stringVal);
				}	
			value_.stringVal=StrDup(value);
			type_=TYPE_STRING;
			return value;
			};


		 // "Get" operators

		inline operator char() const
			{
			switch (this->type_)
				{
				case TYPE_CHAR:
					return value_.charVal;
				case TYPE_SHORT:
					return (char)value_.shortVal;
				case TYPE_INT:
					return (char)value_.intVal;
				case TYPE_LONG:
					return (char)value_.longVal;
				case TYPE_UNSIGNEDCHAR:
					return (char)value_.ucharVal;
				case TYPE_UNSIGNEDSHORT:
					return (char)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (char)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (char)value_.ulongVal;
				case TYPE_FLOAT:
					return (char)value_.floatVal;
				case TYPE_DOUBLE:
					return (char)value_.doubleVal;			
				case TYPE_BOOL:
					return (char)value_.boolVal;			
				case TYPE_STRING:
					return (char)(StringToInt(value_.stringVal));
				default:
					{
					return (char)0;
					}
				}
			};

		inline operator short() const
			{
			switch (this->type_)
				{
				case TYPE_SHORT:
					return value_.shortVal;
				case TYPE_CHAR:
					return (short)value_.charVal;
				case TYPE_INT:
					return (short)value_.intVal;
				case TYPE_LONG:
					return (short)value_.longVal;
				case TYPE_UNSIGNEDCHAR:
					return (short)value_.ucharVal;
				case TYPE_UNSIGNEDSHORT:
					return (short)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (short)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (short)value_.ulongVal;
				case TYPE_FLOAT:
					return (short)value_.floatVal;
				case TYPE_DOUBLE:
					return (short)value_.doubleVal;			
				case TYPE_BOOL:
					return (short)value_.boolVal;			
				case TYPE_STRING:
					return (short)(StringToInt(value_.stringVal));
				default:
					{
					return (short)0;
					}
				}
			};

		inline operator int() const
			{
			switch (this->type_)
				{
				case TYPE_INT:
					return value_.intVal;
				case TYPE_CHAR:
					return (int)value_.charVal;
				case TYPE_SHORT:
					return (int)value_.shortVal;
				case TYPE_LONG:
					return (int)value_.longVal;
				case TYPE_UNSIGNEDCHAR:
					return (int)value_.ucharVal;
				case TYPE_UNSIGNEDSHORT:
					return (int)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (int)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (int)value_.ulongVal;
				case TYPE_FLOAT:
					return (int)value_.floatVal;
				case TYPE_DOUBLE:
					return (int)value_.doubleVal;			
				case TYPE_BOOL:
					return (int)value_.boolVal;			
				case TYPE_STRING:
					return (int)(StringToInt(value_.stringVal));
				default:
					{
					return (int)0;
					}
				}
			};
	
		inline operator long() const
			{
			switch (this->type_)
				{
				case TYPE_LONG:
					return value_.longVal;
				case TYPE_CHAR:
					return (long)value_.charVal;
				case TYPE_SHORT:
					return (long)value_.shortVal;
				case TYPE_INT:
					return (long)value_.intVal;
				case TYPE_UNSIGNEDCHAR:
					return (long)value_.ucharVal;
				case TYPE_UNSIGNEDSHORT:
					return (long)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (long)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (long)value_.ulongVal;
				case TYPE_FLOAT:
					return (long)value_.floatVal;
				case TYPE_DOUBLE:
					return (long)value_.doubleVal;			
				case TYPE_BOOL:
					return (long)value_.boolVal;			
				case TYPE_STRING:
					return (long)(StringToLong(value_.stringVal));
				default:
					{
					return (long)0;
					}
				}
			};

		inline operator unsigned char() const
			{
			switch (this->type_)
				{
				case TYPE_UNSIGNEDCHAR:
					return value_.ucharVal;
				case TYPE_CHAR:
					return (unsigned char)value_.charVal;
				case TYPE_SHORT:
					return (unsigned char)value_.shortVal;
				case TYPE_INT:
					return (unsigned char)value_.intVal;
				case TYPE_LONG:
					return (unsigned char)value_.longVal;
				case TYPE_UNSIGNEDSHORT:
					return (unsigned char)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (unsigned char)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (unsigned char)value_.ulongVal;
				case TYPE_FLOAT:
					return (unsigned char)value_.floatVal;
				case TYPE_DOUBLE:
					return (unsigned char)value_.doubleVal;			
				case TYPE_BOOL:
					return (unsigned char)value_.boolVal;			
				case TYPE_STRING:
					return (unsigned char)(StringToInt(value_.stringVal));
				default:
					{
					return (unsigned char)0;
					}
				}
			};

		inline operator unsigned short() const
			{
			switch (this->type_)
				{
				case TYPE_UNSIGNEDSHORT:
					return value_.ushortVal;
				case TYPE_CHAR:
					return (unsigned short)value_.charVal;
				case TYPE_SHORT:
					return (unsigned short)value_.shortVal;
				case TYPE_INT:
					return (unsigned short)value_.intVal;
				case TYPE_LONG:
					return (unsigned short)value_.longVal;
				case TYPE_UNSIGNEDCHAR:
					return (unsigned short)value_.ucharVal;
				case TYPE_UNSIGNEDINT:
					return (unsigned short)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (unsigned short)value_.ulongVal;
				case TYPE_FLOAT:
					return (unsigned short)value_.floatVal;
				case TYPE_DOUBLE:
					return (unsigned short)value_.doubleVal;			
				case TYPE_BOOL:
					return (unsigned short)value_.boolVal;			
				case TYPE_STRING:
					return (unsigned short)(StringToInt(value_.stringVal));
				default:
					{
					return (short)0;
					}
				}
			};

		inline operator unsigned int() const
			{
			switch (this->type_)
				{
				case TYPE_INT:
					return value_.intVal;
				case TYPE_CHAR:
					return (unsigned int)value_.charVal;
				case TYPE_SHORT:
					return (unsigned int)value_.shortVal;
				case TYPE_LONG:
					return (unsigned int)value_.longVal;
				case TYPE_UNSIGNEDCHAR:
					return (unsigned int)value_.ucharVal;
				case TYPE_UNSIGNEDSHORT:
					return (unsigned int)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (unsigned int)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (unsigned int)value_.ulongVal;
				case TYPE_FLOAT:
					return (unsigned int)value_.floatVal;
				case TYPE_DOUBLE:
					return (unsigned int)value_.doubleVal;			
				case TYPE_BOOL:
					return (unsigned int)value_.boolVal;			
				case TYPE_STRING:
					return (unsigned int)(StringToInt(value_.stringVal));
				default:
					{
					return (unsigned int)0;
					}
				}
			};
	
		inline operator unsigned long() const
			{
			switch (this->type_)
				{
				case TYPE_UNSIGNEDLONG:
					return value_.ulongVal;
				case TYPE_CHAR:
					return (unsigned long)value_.charVal;
				case TYPE_SHORT:
					return (unsigned long)value_.shortVal;
				case TYPE_INT:
					return (unsigned long)value_.intVal;
				case TYPE_LONG:
					return (unsigned long)value_.longVal;
				case TYPE_UNSIGNEDCHAR:
					return (unsigned long)value_.ucharVal;
				case TYPE_UNSIGNEDSHORT:
					return (unsigned long)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (unsigned long)value_.uintVal;
				case TYPE_FLOAT:
					return (unsigned long)value_.floatVal;
				case TYPE_DOUBLE:
					return (unsigned long)value_.doubleVal;			
				case TYPE_STRING:
					return (unsigned long)(StringToInt(value_.stringVal));
				case TYPE_BOOL:
					return (unsigned long)value_.boolVal;			
				default:
					{
					return (unsigned long)0;
					}
				}
			};

		inline operator float() const
			{
			switch (this->type_)
				{
				case TYPE_FLOAT:
					return value_.floatVal;
				case TYPE_CHAR:
					return (float)value_.charVal;
				case TYPE_SHORT:
					return (float)value_.shortVal;
				case TYPE_INT:
					return (float)value_.intVal;
				case TYPE_LONG:
					return (float)value_.longVal;
				case TYPE_UNSIGNEDCHAR:
					return (float)value_.ucharVal;
				case TYPE_UNSIGNEDSHORT:
					return (float)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (float)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (float)(long)value_.ulongVal;
				case TYPE_DOUBLE:
					return (float)value_.doubleVal;			
				case TYPE_BOOL:
					return (float)value_.boolVal;			
				case TYPE_STRING:
					return (float)(StringToDouble(value_.stringVal));
				default:
					{
					return (float)0;
					}
				}
			};

		inline operator double() const
			{
			switch (this->type_)
				{
				case TYPE_DOUBLE:
					return value_.doubleVal;			
				case TYPE_CHAR:
					return (double)value_.charVal;
				case TYPE_SHORT:
					return (double)value_.shortVal;
				case TYPE_INT:
					return (double)value_.intVal;
				case TYPE_LONG:
					return (double)value_.longVal;
				case TYPE_UNSIGNEDCHAR:
					return (double)value_.ucharVal;
				case TYPE_UNSIGNEDSHORT:
					return (double)value_.ushortVal;
				case TYPE_UNSIGNEDINT:
					return (double)value_.uintVal;
				case TYPE_UNSIGNEDLONG:
					return (double)value_.ulongVal;
				case TYPE_FLOAT:
					return (double)value_.floatVal;
				case TYPE_BOOL:
					return (double)value_.boolVal;			
				case TYPE_STRING:
					return (double)(StringToDouble(value_.stringVal));

				default:
					{
					return (double)0;
					}
				}
			};

		inline operator bool() const
			{
			switch (this->type_)
				{
				case TYPE_BOOL:
					return value_.boolVal;			
				case TYPE_CHAR:
					return (value_.charVal!=0);
				case TYPE_SHORT:
					return (value_.shortVal!=0);
				case TYPE_INT:
					return (value_.intVal!=0);
				case TYPE_LONG:
					return (value_.longVal!=0);
				case TYPE_UNSIGNEDCHAR:
					return (value_.ucharVal!=0);
				case TYPE_UNSIGNEDSHORT:
					return (value_.ushortVal!=0);
				case TYPE_UNSIGNEDINT:
					return (value_.uintVal!=0);
				case TYPE_UNSIGNEDLONG:
					return (value_.ulongVal!=0);
				case TYPE_FLOAT:
					return (value_.floatVal!=0);
				case TYPE_DOUBLE:
					return (value_.doubleVal!=0);		
				case TYPE_STRING:
					if (StrICmp(value_.stringVal,"TRUE")==0)
						return true;
					else
						return false;
				default:
					{
					return false;
					}
				}
			};

		inline operator char*() const
			{
			static char buffer[35];
			switch (this->type_)
				{
				case TYPE_STRING:
					return value_.stringVal;
				case TYPE_CHAR:
					SNPrintF(buffer,34,"%d\0",value_.charVal);
					return buffer;
				case TYPE_SHORT:
					SNPrintF(buffer,34,"%d\0",value_.shortVal);
					return buffer;
				case TYPE_INT:
					SNPrintF(buffer,34,"%d\0",value_.intVal);
					return buffer;
				case TYPE_LONG:
					SNPrintF(buffer,34,"%d\0",value_.longVal);
					return buffer;
				case TYPE_UNSIGNEDCHAR:
					SNPrintF(buffer,34,"%d\0",value_.ucharVal);
					return buffer;
				case TYPE_UNSIGNEDSHORT:
					SNPrintF(buffer,34,"%d\0",value_.ushortVal);
					return buffer;
				case TYPE_UNSIGNEDINT:
					SNPrintF(buffer,34,"%d\0",value_.uintVal);
					return buffer;
				case TYPE_UNSIGNEDLONG:
					SNPrintF(buffer,34,"%d\0",value_.ulongVal);
					return buffer;
				case TYPE_FLOAT:
					static char fbuffer[256];
					SNPrintF(fbuffer,255,"%g\0",value_.floatVal);
					return fbuffer;
				case TYPE_DOUBLE:
					static char dbuffer[256];
					SNPrintF(dbuffer,255,"%g\0",value_.doubleVal);
					return dbuffer;
				case TYPE_BOOL:
					if (value_.boolVal)
						{
						SNPrintF(buffer,34,"True\0");
						return buffer;
						}
					else
						{
						SNPrintF(buffer,34,"False\0");
						return buffer;
						}
				default:
					buffer[0]='\0';
					return buffer;
				};
			};


		inline operator const char*() const
			{
			static char buffer[35];
			switch (this->type_)
				{
				case TYPE_STRING:
					return value_.stringVal;
				case TYPE_CHAR:
					SNPrintF(buffer,34,"%d\0",value_.charVal);
					return buffer;
				case TYPE_SHORT:
					SNPrintF(buffer,34,"%d\0",value_.shortVal);
					return buffer;
				case TYPE_INT:
					SNPrintF(buffer,34,"%d\0",value_.intVal);
					return buffer;
				case TYPE_LONG:
					SNPrintF(buffer,34,"%d\0",value_.longVal);
					return buffer;
				case TYPE_UNSIGNEDCHAR:
					SNPrintF(buffer,34,"%d\0",value_.ucharVal);
					return buffer;
				case TYPE_UNSIGNEDSHORT:
					SNPrintF(buffer,34,"%d\0",value_.ushortVal);
					return buffer;
				case TYPE_UNSIGNEDINT:
					SNPrintF(buffer,34,"%d\0",value_.uintVal);
					return buffer;
				case TYPE_UNSIGNEDLONG:
					SNPrintF(buffer,34,"%d\0",value_.ulongVal);
					return buffer;
				case TYPE_FLOAT:
					static char fbuffer[256];
					SNPrintF(fbuffer,255,"%g\0",value_.floatVal);
					return fbuffer;
				case TYPE_DOUBLE:
					static char dbuffer[256];
					SNPrintF(dbuffer,255,"%g\0",value_.doubleVal);
					return dbuffer;
				case TYPE_BOOL:
					if (value_.boolVal)
						return "True";
					else
						return "False";
				default:
					buffer[0]='\0';
					return buffer;
				};
			};
	private:
		TYPES type_;
		
		union 
			{
			char charVal;
			short shortVal;
			int intVal;
			long longVal;
			unsigned char ucharVal;
			unsigned short ushortVal;
			unsigned int uintVal;
			unsigned long ulongVal;
			float floatVal;
			double doubleVal;
			bool boolVal;
			char* stringVal;
			} value_;
		

	};

#endif /* __XMLVariant_H__ */
