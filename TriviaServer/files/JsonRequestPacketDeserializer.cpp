#include "JsonRequestPacketDeserializer.h"
#include <iostream>

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::string loginReq)
{
	LoginRequest LR;
	nlohmann::json jsn;
	try
	{
		
		jsn.clear();
		jsn = nlohmann::json::parse(loginReq);

		LR.username = jsn.at("username").get<std::string>();
		LR.password = jsn.at("password").get<std::string>(); //extract the data from the jason
		if (LR.username == "" || LR.password == "")
		{
			throw  std::exception(__FUNCTION__ "error deserializing client login request!");
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
		ErrorResponse errRes;
		errRes.message = "Error deserializing client login request!";
		//TODO: send back to client the error
	}
	return LR;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(std::string signReq)
{
	SignupRequest SR;
	nlohmann::json jsn;
	try
	{
		jsn.clear();
		jsn = nlohmann::json::parse(signReq);
		SR.username = jsn.at("username").get<std::string>();
		SR.password = jsn.at("password").get<std::string>(); //get the data
		SR.email = jsn.at("mail").get<std::string>();
		if (SR.username == "" || SR.password == "" || SR.email == "")
		{
			throw std::exception(__FUNCTION__ "error parsing signup msg!");
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
		ErrorResponse errRes;
		errRes.message = "Error getting client signup request!";
		//TODO: send the message
	}
	return SR;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(std::string buffer)
{
	nlohmann::json jsn;
	jsn = nlohmann::json::parse(buffer);
	GetPlayersInRoomRequest e;
	e.roomID = jsn.begin().value();
	return e;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(std::string buffer)
{
	nlohmann::json jsn;
	jsn = nlohmann::json::parse(buffer);
	JoinRoomRequest e;
	e.roomID = jsn.begin().value();
	return e;
}

ChangeRoomSettings JsonRequestPacketDeserializer::deserializeChangeRoomRequest(std::string buffer)
{
	nlohmann::json jsn;
	ChangeRoomSettings e;

	jsn = nlohmann::json::parse(buffer);
	e.from_json(jsn, e);

	return e;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::string buffer)
{
	nlohmann::json jsn;
	CreateRoomRequest e;

	jsn = nlohmann::json::parse(buffer);
	e.from_json(jsn, e);

	return e;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(std::string buffer)
{
	nlohmann::json jsn;
	SubmitAnswerRequest e;

	jsn = nlohmann::json::parse(buffer);
	e.from_json(jsn, e);

	return e;
}

