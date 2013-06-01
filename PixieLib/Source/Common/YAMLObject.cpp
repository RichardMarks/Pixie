//*** YAMLObject.cpp ***

#include "YAMLObject.h"
#include "DynamicBuffer.h"
#include "Asset.h"
#include "Array.h"

#include "libyaml/yaml.h"

static char YAMLindent[1024];

void DebugPrintYAML(yaml_event_t event);

int YAML_Read_Handler(void *data, unsigned char* buffer, size_t size, size_t* size_read)
	{
	const Asset* asset = (const Asset*)data;
	int read = asset->Read( buffer, size );
	(*size_read) = read;
	return 1;
	}

 bool getNextEvent(yaml_parser_t& parser, yaml_event_t& event, const Asset& asset)
	{
	int result = yaml_parser_parse(&parser, &event);
	if (!result)
		{
		#ifdef _DEBUG
		const char* filename=asset.GetFilename().GetString();
		if (filename)
			{
			char errorMessage[1024];
			SNPrintF(errorMessage,1024,"Error parsing YAML file: %s\nProblem: %s\nLine: %d Column: %d",filename,parser.problem,parser.problem_mark.line,parser.problem_mark.column);
			Assert(false,errorMessage);
			}
		else
			{
			Assert(false,"Error parsing YAML file.");
			}
		#endif
		return false;
		}

	//DebugPrintYAML(event);
	return true;
	}

enum YamlState
	{
	State_Unknown,
	State_Sequence,
	State_Mapping,
	State_Scalar,
	};

struct YamlNode
	{
	YamlState state;
	XMLVariant value;
	YAMLObject* object;
	};

void YamlState_Push(Array<YamlNode>& stack, YamlState state, YAMLObject* object, XMLVariant value = XMLVariant() )
	{
	YamlNode node={state, value, object};
	stack.Add(node);
	}

YamlNode YamlState_Pop(Array<YamlNode>& stack)
	{
	if (stack.GetItemCount()>0)
		{
		YamlNode node = stack.Get(stack.GetItemCount()-1);
		stack.RemoveLast();
		return node;
		}
	YamlNode node = { State_Unknown, 0 };
	return node;
	}

YamlNode YamlState_Top(Array<YamlNode>& stack)
	{
	if (stack.GetItemCount()>0)
		{
		return stack.Get(stack.GetItemCount()-1);
		}

	YamlNode node = { State_Unknown, 0 };
	return node;
	}


StringId YamlState_GetSequenceName(Array<YamlNode>& stack)
	{
	if (stack.GetItemCount()==0)
		{
		return StringId();
		}


	int i = stack.GetItemCount();
	i--;
	while (i >= 0)
		{
		YamlNode& node = stack.Get(i);
		if ( node.state == State_Sequence)
			{
			return StringId(node.value);
			}
		if ( stack.Get(i).state == State_Mapping)
			{
			return StringId();
			}
		i--;
		}

	return StringId();
	}


bool YamlState_InMapping(Array<YamlNode>& stack)
	{
	int i = stack.GetItemCount();
	i--;
	while (i >= 0)
		{
		if ( stack.Get(i).state == State_Mapping )
			{
			return true;
			}
		if ( stack.Get(i).state == State_Sequence )
			{
			return false;
			}
		i--;
		}
	return false;
	}

bool YamlState_InSequence(Array<YamlNode>& stack)
	{
	int i = stack.GetItemCount();
	i--;
	while (i >= 0)
		{
		if ( stack.Get(i).state == State_Sequence)
			{
			return true;
			}
		if ( stack.Get(i).state == State_Mapping)
			{
			return false;
			}
		i--;
		}
	return false;
	}

//*** LoadXML ***

bool YAMLObject::LoadYAML(const Asset& asset)
	{

	if (!asset.Open())
		{
		#ifdef _DEBUG
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
		#endif
		return false;
		}

	yaml_parser_t parser;
    yaml_event_t event;

	if ( yaml_parser_initialize(&parser) == 0)
		{
		asset.Close();
		return false;
		}
	yaml_parser_set_input( &parser, YAML_Read_Handler, (void*)(&asset) );
	YAMLindent[0]=0;

	Array<YamlNode> stack;

	YamlState_Push(stack,State_Unknown,this);

	bool done = false;
	bool nocontent = true;
	while (!done)
		{
		if (!getNextEvent(parser, event, asset))
			{
			yaml_event_delete(&event);
			yaml_parser_delete(&parser);
			asset.Close();
			return false;
			}

		switch(event.type)
			{ 
			case YAML_NO_EVENT: break;
			case YAML_STREAM_START_EVENT: break;
			case YAML_STREAM_END_EVENT: done = true; break;
			case YAML_DOCUMENT_START_EVENT: break;
			case YAML_DOCUMENT_END_EVENT: break;

			case YAML_SEQUENCE_START_EVENT: 
				{
				nocontent = false;
				YamlNode top = YamlState_Top(stack);
				StringId name;
				if (top.state==State_Scalar)
					{
					if (YamlState_InSequence(stack))
						{
						YAMLObject* object=top.object;
						if (object)
							{
							object->YAML_Attribute(YamlState_GetSequenceName(stack),top.value);
							}
						YamlState_Pop(stack);
						top.value=XMLVariant();
						}
					else
						{
						name = StringId(top.value);
						YamlState_Pop(stack);
						}
					}
			
				YAMLObject* object=top.object;
				if (object)
					{
					object = object->YAML_Element(name,true);
					}
				YamlState_Push(stack, State_Sequence, object,top.value);
				
				
				} break;

			case YAML_SEQUENCE_END_EVENT:   
				{
				YamlNode top = YamlState_Pop(stack);
				if (top.state==State_Scalar)
					{
					YAMLObject* object=top.object;
					if (object)
						{
						object->YAML_Attribute(YamlState_GetSequenceName(stack),top.value);
						}
					top = YamlState_Pop(stack);
					}
				Assert(top.state==State_Sequence,"Stack invalid");

				StringId name = StringId(top.value);
				YAMLObject* object=top.object;
				if (object)
					{
					object->YAML_ElementEnd(name,true);
					}
				} break;

			case YAML_MAPPING_START_EVENT:  
				{
				nocontent = false;
				YamlNode top = YamlState_Top(stack);
				StringId name;
				if (top.state==State_Scalar)
					{
					if (YamlState_InSequence(stack))
						{
						YAMLObject* object=top.object;
						if (object)
							{
							object->YAML_Attribute(YamlState_GetSequenceName(stack),top.value);
							}
						YamlState_Pop(stack);
						top.value=XMLVariant();
						}
					else
						{
						name = StringId(top.value);
						YamlState_Pop(stack);
						}
					}
				YAMLObject* object=top.object;
				if (object)
					{
					object = object->YAML_Element(name,false);
					}
				YamlState_Push(stack, State_Mapping, object,name.GetString());
				} break;

			case YAML_MAPPING_END_EVENT:
				{			
				YamlNode top = YamlState_Pop(stack);
				Assert(top.state==State_Mapping,"Stack invalid");

				StringId name = StringId(top.value);
				YAMLObject* object=top.object;
				if (object)
					{
					object->YAML_ElementEnd(name,false);
					}
				} break;

			case YAML_ALIAS_EVENT:  
				{
				#ifdef _DEBUG
				const char* filename=asset.GetFilename().GetString();
				if (filename)
					{
					char errorMessage[1024];
					SNPrintF(errorMessage,1024,"Error parsing YAML file: %s\nProblem: %s\nLine: %d Column: %d",filename,"No support for anchors and aliases",parser.mark.line,parser.mark.column);
					Assert(false,errorMessage);
					}
				else
					{
					Assert(false,"Error parsing YAML file.");
					}
				#endif

				YamlNode top = YamlState_Top(stack);
				if (top.state == State_Scalar)
					{
					YamlState_Pop(stack);
					}
				} break;

			case YAML_SCALAR_EVENT: 
				{
				XMLVariant value = (char*)event.data.scalar.value;
				YamlNode top = YamlState_Top(stack);
				if (top.state == State_Scalar)
					{
					if (YamlState_InMapping(stack))
						{
						StringId name = top.value;
						top = YamlState_Pop(stack);
						YAMLObject* object=top.object;
						if (object)
							{
							object->YAML_Attribute(name,value);
							}
						}
					else
						{
						YAMLObject* object=top.object;
						if (object)
							{
							object->YAML_Attribute(YamlState_GetSequenceName(stack),top.value);
							}
						top = YamlState_Pop(stack);
						YamlState_Push(stack, State_Scalar, top.object, value);
						}
					}
				else
					{
					YamlState_Push(stack, State_Scalar, top.object, value);
					}

				//DebugPrint(("Got scalar (value %s)\n", event.data.scalar.value)); 
				} break;
			}

		yaml_event_delete(&event);
		}

	yaml_event_delete(&event);
	yaml_parser_delete(&parser);
	asset.Close();
	if (nocontent)
		{
		return false;
		}

	return true;
	}


YAMLObject* YAMLObject::YAML_Element(StringId _name, bool _sequence)
	{
//	DebugPrint(("ELEMENT %s\n",_name.GetString()));
	return this;
	}

void YAMLObject::YAML_ElementEnd(StringId _name, bool _sequence)
	{
//	DebugPrint(("END ELEMENT%s\n",_name.GetString()));

	}

void YAMLObject::YAML_Attribute(StringId _name, const XMLVariant& _value)
	{
//	DebugPrint(("ATTRIBUTE %s:%s\n",_name.GetString(),(char*)_value));
	}



void DebugPrintYAML(yaml_event_t event)
	{
	switch(event.type)
		{ 
		case YAML_NO_EVENT: 
			{
			} break;
		
		/* Stream start/end */
		case YAML_STREAM_START_EVENT: 
			{
			DebugPrint((YAMLindent));
			DebugPrint(("STREAM START\n")); 
			StrCat(YAMLindent,"    ");
			} break;

		case YAML_STREAM_END_EVENT:
			{
			YAMLindent[StrLen(YAMLindent)-4] = 0;
			DebugPrint((YAMLindent));
			DebugPrint(("STREAM END\n"));   
			} break;
		
			/* Block delimeters */
		case YAML_DOCUMENT_START_EVENT: 
			{
			DebugPrint((YAMLindent));
			DebugPrint(("Start Document\n")); 
			StrCat(YAMLindent,"    ");
			} break;

		case YAML_DOCUMENT_END_EVENT:   
			{
			YAMLindent[StrLen(YAMLindent)-4] = 0;
			DebugPrint((YAMLindent));
			DebugPrint(("End Document\n"));   
			} break;

		case YAML_SEQUENCE_START_EVENT: 
			{
			DebugPrint((YAMLindent));
			DebugPrint(("Start Sequence\n")); 
			StrCat(YAMLindent,"    ");
			} break;

		case YAML_SEQUENCE_END_EVENT:   
			{
			YAMLindent[StrLen(YAMLindent)-4] = 0;
			DebugPrint((YAMLindent));
			DebugPrint(("End Sequence\n"));   
			} break;

		case YAML_MAPPING_START_EVENT:  
			{
			DebugPrint((YAMLindent));
			DebugPrint(("Start Mapping\n"));  
			StrCat(YAMLindent,"    ");
			} break;

		case YAML_MAPPING_END_EVENT:
			{
			YAMLindent[StrLen(YAMLindent)-4] = 0;
			DebugPrint((YAMLindent));
			DebugPrint(("End Mapping\n"));    
			} break;

		/* Data */
		case YAML_ALIAS_EVENT:  
			{
			DebugPrint((YAMLindent));
			DebugPrint(("Got alias (anchor %s)\n", event.data.alias.anchor)); 
			} break;

		case YAML_SCALAR_EVENT: 
			{
			DebugPrint((YAMLindent));
			DebugPrint(("Got scalar (value %s)\n", event.data.scalar.value)); 
			} break;
		}
	}