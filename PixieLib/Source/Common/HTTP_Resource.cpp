//*** HTTP_Resource.cpp ***

#include "HTTP_Resource.h"


//*** Constructor ***

HTTP_Resource::HTTP_Resource(int requestHandle, StringId type, const StaticBuffer& data):
	requestHandle_(requestHandle),
	type_(type),
	data_(data)
	{

	
	}


//*** Destructor ***

HTTP_Resource::~HTTP_Resource()
	{
	}


//*** GetType ***

StringId HTTP_Resource::GetType()
	{
	return type_;
	}


//*** GetRequestHandle ***

int HTTP_Resource::GetRequestHandle()
	{
	return requestHandle_;
	}


//*** GetData ***

const StaticBuffer& HTTP_Resource::GetData()
	{
	return data_;
	}
