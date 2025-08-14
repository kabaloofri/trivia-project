#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

#include "RequestHandlerFactory.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* RHF, LoginManager* LM) : IRequestHandler(RHF)
{
	m_loginManager = LM;
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo reqInf) const
{
	if (reqInf.id != code_logIn && reqInf.id != code_signUp)
	{
		return false; //not sure if this is correct but ok
	}
	return true; //return true only if user is trying to log in or signup
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo reqInfo)
{
	std::cout << "handling login request!\n";
	RequestResult reqRes;

	if (reqInfo.id == code_logIn)
	{
		reqRes = login(reqInfo);
	}
	else
	{
		reqRes = signUp(reqInfo);
	}

	return reqRes;
}

RequestResult LoginRequestHandler::login(RequestInfo reqInfo)
{
	// *** log in ***
	RequestResult reqRes;
	LoginResponse LR;

	LoginRequest logReq = JsonRequestPacketDeserializer::deserializeLoginRequest(reqInfo.buffer.getStrBuffer());

	// set the responce status based on the answer from LoginManger::login
	// and create the next handler
	
	//try add to logged users from LoginManager,
	if (m_loginManager->login(logReq.username, logReq.password))
	{
		auto user = LoggedUser(logReq.username);

		LR.status = login_success;
		reqRes.responce = JsonRequestPacketSerializer::serializeResponse(LR);
		reqRes.newHandler = m_handlerFactory->CreateMenuRequestHandler(user);
	}
	else
	{
		ErrorResponse ErrResp;
		ErrResp.message = "password is incorrect!";

		reqRes.responce = JsonRequestPacketSerializer::serializeResponse(ErrResp);
		reqRes.newHandler = this; //stay in login
	}
	return reqRes;
}

RequestResult LoginRequestHandler::signUp(RequestInfo reqInfo)
{
	// *** sign in ***
	RequestResult reqRes;
	SignupResponse SR;

	//create request struct
	SignupRequest signReq = JsonRequestPacketDeserializer::deserializeSignupRequest(reqInfo.buffer.getStrBuffer());
	
	//try to add to database and logged users
	//create next handler
	if (m_loginManager->signup(signReq.username, signReq.password, signReq.email))
	{
		auto user = LoggedUser(signReq.username);

		reqRes.newHandler = m_handlerFactory->CreateMenuRequestHandler(user);
		SR.status = signin_success;
		reqRes.responce = JsonRequestPacketSerializer::serializeResponse(SR);
	}
	else
	{
		ErrorResponse ErrResp;
		ErrResp.message = "error trying to sign in! username already exists!";

		reqRes.responce = JsonRequestPacketSerializer::serializeResponse(ErrResp);
		reqRes.newHandler = this; //stay in login
	}

	return reqRes;
}
