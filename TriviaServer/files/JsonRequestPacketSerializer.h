#pragma once
#include "json.hpp"
#include <sstream>
#include <string>
#include "Buffer.h"
#include "Room.h"


enum // response/request codes
{
	code_error = -1,

	code_logIn = 1,
	code_signUp,

	code_logOut,
	code_getRooms,
	code_getPlayersInRoom,
	code_getHighScore,
	code_getPersonalStats,
	code_joinRoom,
	code_createRoom,
	
	code_changeSettings,
	code_closeRoom,
	code_startGame,
	code_getRoomState,
	code_leaveRoom,

	code_leaveGame,
	code_getQuestion,
	code_submitAnswer,
	code_getGameResults,
};

enum // status codes
{
	status_OK = 0
};

struct ErrorResponse
{
	std::string message;
};

struct SignupResponse
{
	unsigned int status;
};

struct LoginResponse
{
	unsigned int status;
};





struct LogoutResponse
{
	unsigned int status;
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetRoomsResponse, status, rooms)
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;


	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetPlayersInRoomResponse, players)
};

struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<std::string> statistics;

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetHighScoreResponse, status, statistics)
};

struct GetPersonalStatsResponse
{
	unsigned int status;
	std::vector<std::string> statistics;

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetPersonalStatsResponse, status, statistics)
};

struct JoinRoomResponse
{
	unsigned int status;
};


struct ChangeRoomSettingsResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
	unsigned int roomId;
};




struct CloseRoomResponse
{
	unsigned int status;
};

struct StartGameResponse
{
	unsigned int status;
};

struct LeaveRoomResponse
{
	unsigned int status;
};

struct GetRoomStateResponse
{
	unsigned int status;
	double timeUntilGameBegins;
	std::vector<std::string> players;
	unsigned int querstionsCount;
	unsigned int answerTimeout;	



	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetRoomStateResponse, status, timeUntilGameBegins, players, querstionsCount, answerTimeout)
};

struct LeaveGameResponse
{
	unsigned int status;
};

struct GetQuestionResponse
{
	int status;
	std::string question;
	std::map<std::string, std::string> answers;

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_ONLY_SERIALIZE(GetQuestionResponse , status , question , answers) 
};


struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};


struct PlayerResults
{
	std::string username;
	int score;
	int correctAnswerCount;
	int wrongAnswerCount;
	double averageAnswerTime;

	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerResults, username,score, correctAnswerCount, wrongAnswerCount, averageAnswerTime)
};


struct GetGameResultsResponse
{
	unsigned int status;
	std::vector<PlayerResults> results;


	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetGameResultsResponse, status, results)
};

class JsonRequestPacketSerializer
{
public:

	//serailize functions
	static Buffer serializeResponse(ErrorResponse);
	static Buffer serializeResponse(LoginResponse);
	static Buffer serializeResponse(SignupResponse);
	
	static Buffer serializeResponse(LogoutResponse);
	static Buffer serializeResponse(GetRoomsResponse);
	static Buffer serializeResponse(GetPlayersInRoomResponse);
	static Buffer serializeResponse(GetHighScoreResponse);
	static Buffer serializeResponse(GetPersonalStatsResponse);
	static Buffer serializeResponse(JoinRoomResponse);
	static Buffer serializeResponse(CreateRoomResponse);
	static Buffer serializeResponse(ChangeRoomSettingsResponse);

	static Buffer serializeResponse(CloseRoomResponse);
	static Buffer serializeResponse(StartGameResponse);
	static Buffer serializeResponse(LeaveRoomResponse);
	static Buffer serializeResponse(GetRoomStateResponse);


	static Buffer serializeResponse(LeaveGameResponse);
	static Buffer serializeResponse(GetQuestionResponse);
	static Buffer serializeResponse(SubmitAnswerResponse);
	static Buffer serializeResponse(GetGameResultsResponse);



	//auxiliary functions
	static std::string createBuffer(nlohmann::json json, int code);
};


 