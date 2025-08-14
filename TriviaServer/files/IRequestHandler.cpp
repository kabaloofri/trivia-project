#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

IRequestHandler::IRequestHandler(RequestHandlerFactory* RHF)
{
	m_handlerFactory = RHF;
}

IRequestHandler::~IRequestHandler()
{
}
