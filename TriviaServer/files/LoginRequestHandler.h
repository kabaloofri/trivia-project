#pragma once
#include "IRequestHandler.h";
#include "LoginManager.h"


enum
{
	login_success,

	signin_success,
};


class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory*, LoginManager*);
	virtual ~LoginRequestHandler() = default;

	bool isRequestRelevant(RequestInfo reqInf) const override;
	RequestResult handleRequest(RequestInfo reqInf) override;

private:
	RequestResult login(RequestInfo);
	RequestResult signUp(RequestInfo);

	LoginManager* m_loginManager;
};

//handler list: 

//1-login/signup
//2-joinRoom/createRoom
//3-getPlayersInRoom
//4-submitAnswer