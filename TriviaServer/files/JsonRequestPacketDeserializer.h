#pragma once 

#include "JsonRequestPacketSerializer.h"

// structs
typedef struct LoginRequest
{
	std::string username;
	std::string password;
} LoginRequest;

typedef struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
} SignupRequest;

typedef struct GetQuestionRequest
{
	int status;
}GetQuestionRequest;
typedef struct GetPlayersInRoomRequest
{
	unsigned int roomID;
} GetPLayersInRoomRequest;

typedef struct JoinRoomRequest
{
	unsigned int roomID;
} JoinRoomRequest;

typedef struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionsCount;
	unsigned int answerTimeout;

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateRoomRequest, roomName, maxUsers, questionsCount, answerTimeout)
} CreateRoomRequest;

struct ChangeRoomSettings
{
	unsigned int questionCount;
	unsigned int answerTimeout;

	
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ChangeRoomSettings, questionCount, answerTimeout)
};


typedef struct SubmitAnswerRequest
{
	int AnswerID;
	float answerTime;

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SubmitAnswerRequest, AnswerID, answerTime)
} SubmitAnswerRequest;


typedef struct LeaveGameRequest
{
	unsigned int status;

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LeaveGameRequest, status)
} LeaveGameRequest;



// static class
class JsonRequestPacketDeserializer
{
public:

	static LoginRequest deserializeLoginRequest(std::string);
	static SignupRequest deserializeSignupRequest(std::string);
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(std::string);
	static JoinRoomRequest deserializeJoinRoomRequest(std::string);
	static ChangeRoomSettings deserializeChangeRoomRequest(std::string);
	static CreateRoomRequest deserializeCreateRoomRequest(std::string);
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(std::string);

	// NOTE: the data sent to these functions should be text,
	// converting the data to code, length and data and to text from binary 
	// should be done by the one calling these (I.E. the server)
};