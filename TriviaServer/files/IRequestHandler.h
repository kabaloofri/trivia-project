#pragma once
#include <iostream>
#include "Buffer.h"
#include "JsonRequestPacketDeserializer.h"

class RequestHandlerFactory;
class IRequestHandler;


typedef struct RequestInfo
{
	unsigned char id; // replace std::byte with unsigned char (both a byte)
	time_t recivedTime;
	Buffer buffer;
} RequestInfo;


typedef struct RequestResult
{ 
	Buffer responce;
	IRequestHandler* newHandler;
} RequestResult;



// I-interface- pure virtual mini class
class IRequestHandler
{
public:
	IRequestHandler(RequestHandlerFactory*);
	virtual ~IRequestHandler() = 0;

	virtual bool isRequestRelevant(RequestInfo) const = 0;
	virtual RequestResult handleRequest(RequestInfo) = 0;

	RequestHandlerFactory* m_handlerFactory;
};






