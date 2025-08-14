#include "JsonRequestPacketSerializer.h"
#include "Helper.h"


using nlohmann::json;



Buffer JsonRequestPacketSerializer::serializeResponse(ErrorResponse response)
{
    json json = {};
    json["message"] = response.message;
    return Buffer(createBuffer(json, code_error));
}

Buffer JsonRequestPacketSerializer::serializeResponse(LoginResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    return Buffer(createBuffer(jsn, code_logIn));
}

Buffer JsonRequestPacketSerializer::serializeResponse(SignupResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    return Buffer(createBuffer(jsn, code_signUp));
}

Buffer JsonRequestPacketSerializer::serializeResponse(LogoutResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    return Buffer(createBuffer(jsn, code_logOut));
}

Buffer JsonRequestPacketSerializer::serializeResponse(GetRoomsResponse response)
{
    json jsn = {};
    response.to_json(jsn, response);
    return Buffer(createBuffer(jsn, code_getRooms));
}

Buffer JsonRequestPacketSerializer::serializeResponse(GetPlayersInRoomResponse response)
{
    json jsn = {};
    response.to_json(jsn, response);
    return Buffer(createBuffer(jsn, code_getPlayersInRoom));
}

Buffer JsonRequestPacketSerializer::serializeResponse(GetHighScoreResponse response)
{
    json jsn = {};
    response.to_json(jsn, response);
    return Buffer(createBuffer(jsn, code_getHighScore));
}

Buffer JsonRequestPacketSerializer::serializeResponse(GetPersonalStatsResponse response)
{
    json jsn = {};
    response.to_json(jsn, response);
    return Buffer(createBuffer(jsn, code_getPersonalStats));
}

Buffer JsonRequestPacketSerializer::serializeResponse(JoinRoomResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    return Buffer(createBuffer(jsn, code_joinRoom));
}

Buffer JsonRequestPacketSerializer::serializeResponse(CreateRoomResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    jsn["roomId"] = response.roomId;
    return Buffer(createBuffer(jsn, code_createRoom));
}

Buffer JsonRequestPacketSerializer::serializeResponse(ChangeRoomSettingsResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;

    return Buffer(createBuffer(jsn, code_changeSettings));
}

Buffer JsonRequestPacketSerializer::serializeResponse(CloseRoomResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    return Buffer(createBuffer(jsn, code_closeRoom));
}

Buffer JsonRequestPacketSerializer::serializeResponse(StartGameResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    return Buffer(createBuffer(jsn, code_startGame));
}

Buffer JsonRequestPacketSerializer::serializeResponse(LeaveRoomResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    return Buffer(createBuffer(jsn, code_leaveRoom));
}

Buffer JsonRequestPacketSerializer::serializeResponse(GetRoomStateResponse response)
{
    json jsn = {};
    response.to_json(jsn, response);
    return Buffer(createBuffer(jsn, code_getRoomState));
}





Buffer JsonRequestPacketSerializer::serializeResponse(LeaveGameResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    return Buffer(createBuffer(jsn, code_leaveGame));
}

Buffer JsonRequestPacketSerializer::serializeResponse(GetQuestionResponse response)
{
    json jsn = {};
    response.to_json(jsn, response);
    return Buffer(createBuffer(jsn, code_getQuestion));
}

Buffer JsonRequestPacketSerializer::serializeResponse(SubmitAnswerResponse response)
{
    json jsn = {};
    jsn["status"] = response.status;
    jsn["correctAnswerId"] = response.correctAnswerId;
    return Buffer(createBuffer(jsn, code_submitAnswer));
}

Buffer JsonRequestPacketSerializer::serializeResponse(GetGameResultsResponse response)
{
    json jsn = {};
    response.to_json(jsn, response);
    return Buffer(createBuffer(jsn, code_getGameResults));
}



std::string JsonRequestPacketSerializer::createBuffer(nlohmann::json jsn, int code)
{
    std::string json_data = jsn.dump();
    std::string data_length = Helper::getPaddedNumber(json_data.size(), 4);
    
    return Helper::getPaddedNumber(code, 2) + data_length + json_data;
}
