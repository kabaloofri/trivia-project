#include "MenuRequestHandler.h"

#include "RequestHandlerFactory.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory* RHF, LoggedUser user)
    : IRequestHandler(RHF) , m_user(user)
{

}

MenuRequestHandler::~MenuRequestHandler()
{

}

bool MenuRequestHandler::isRequestRelevant(RequestInfo reqInfo) const
{
    //check all the types requests
    if (reqInfo.id == code_logOut || reqInfo.id == code_getRooms || reqInfo.id == code_getPlayersInRoom || reqInfo.id == code_getPersonalStats || reqInfo.id == code_getHighScore || reqInfo.id == code_joinRoom || reqInfo.id == code_createRoom)
    {
        return true;
    }
    return false;
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo reqInfo)
{
    RequestResult reqRes;

    switch (reqInfo.id)
    {
    case code_logOut:
        reqRes = logout(reqInfo);
        break;
    case code_getRooms:
        reqRes = getRooms(reqInfo);
        break;
    case code_getPlayersInRoom:
        reqRes = getPlayersInRoom(reqInfo);
        break;
    case code_getPersonalStats:
        reqRes = getPersonalStats(reqInfo);
        break;
    case code_getHighScore:
        reqRes = getHighScore(reqInfo);
        break;
    case code_joinRoom:
        reqRes = joinRoom(reqInfo);
        break;
    case code_createRoom:
        reqRes = createRoom(reqInfo);
        break;
    case code_startGame:
        
    default:
        std::string err = __FUNCTION__ ": error idendefing code request in menuHandler! code: " + std::to_string(reqInfo.id) + "\n";
        throw std::exception(err.c_str());
    }
    

    return reqRes;
}



RequestResult MenuRequestHandler::logout(RequestInfo& reqInfo)
{
    // *** Log out ***
    LogoutResponse LR;
    RequestResult reqRes;

    //handle request
    m_handlerFactory->getLoginManager().logout(m_user.getUsername());
    LR.status = status_OK;

    //serialize response
    reqRes.responce = JsonRequestPacketSerializer::serializeResponse(LR);
    reqRes.newHandler = m_handlerFactory->createLoginRequestHandler();
    //go back to login stage

    return reqRes;
}

RequestResult MenuRequestHandler::getRooms(RequestInfo& reqInfo)
{
    GetRoomsResponse GR;
    RequestResult reqRes;

    //handle request
    GR.rooms = m_handlerFactory->getRoomManager().getRooms();
    GR.status = status_OK;

    //serialize response
    reqRes.responce = JsonRequestPacketSerializer::serializeResponse(GR);
    reqRes.newHandler = this; //stay in current handler

    return reqRes;
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo& reqInfo)
{
    GetPlayersInRoomResponse GPR;
    RequestResult reqRes;
    
    // deserialize request 
    GetPlayersInRoomRequest playerRoomReq = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(reqInfo.buffer.getStrBuffer());

    //handle request
    GPR.players = m_handlerFactory->getRoomManager().getRoom(playerRoomReq.roomID).getAllUsers();
    
    if (GPR.players.empty())
    {
        //if the list is empty- 
    }
    //TODO: command could fail if no such room exists
    
    // serilize response
    reqRes.responce = JsonRequestPacketSerializer::serializeResponse(GPR);
    reqRes.newHandler = this; //stay in curr handler

    return reqRes;
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo& reqInfo)
{
    GetPersonalStatsResponse GPSR;
    RequestResult reqRes;

    //handle request
    GPSR.statistics = m_handlerFactory->getStatisticsManager()->getUserStatistics(m_user.getUsername());
    GPSR.status = status_OK;
    //serialize response;
    reqRes.responce = JsonRequestPacketSerializer::serializeResponse(GPSR);
    reqRes.newHandler = this;

    return reqRes;
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo& reqInfo)
{
    GetHighScoreResponse GHSR;
    RequestResult reqRes;

    //handle requrest
    GHSR.statistics = m_handlerFactory->getStatisticsManager()->getHighScore();
    GHSR.status = status_OK;
    //TODO: handle edge case of no high score and return fail
    
    //serialize response
    reqRes.responce = JsonRequestPacketSerializer::serializeResponse(GHSR);
    reqRes.newHandler = this;

    return reqRes;
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo& reqInfo)
{
    JoinRoomResponse JR;
    RequestResult reqRes;

    //deserialize request
    JoinRoomRequest joinReq = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(reqInfo.buffer.getStrBuffer());

    //handle request
    bool roomAvailable = false;
    roomAvailable = m_handlerFactory->getRoomManager().getRoom(joinReq.roomID).addUser(m_user);
    if (roomAvailable)
    {
        JR.status = status_OK;

        //serialize reponse
        reqRes.responce = JsonRequestPacketSerializer::serializeResponse(JR);

        //move to room member handler
        reqRes.newHandler = m_handlerFactory->createRoomMemberRequestHandler(m_user, m_handlerFactory->getRoomManager().getRoom(joinReq.roomID));
    }
    else
    {
        //room is full and cant join!
        ErrorResponse ER;
        ER.message = "room is full!";
        reqRes.responce = JsonRequestPacketSerializer::serializeResponse(ER);
        reqRes.newHandler = this; //stay in menu if fail
    }
    
    return reqRes;
}

RequestResult MenuRequestHandler::createRoom(RequestInfo& reqInfo)
{
    CreateRoomResponse CR;
    RequestResult reqRes;

    //deserialize request
    CreateRoomRequest createReq = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(reqInfo.buffer.getStrBuffer());
    RoomData data;
    data.status = state_waitingForPlayers;
    data.maxPlayers = createReq.maxUsers;
    data.name = createReq.roomName;
    data.numOfQuestionsInGame = createReq.questionsCount;
    data.timePerQuestion = createReq.answerTimeout;
    data.currentAmountOfPlayers = 1;
    data.id = m_handlerFactory->getRoomManager().getCurrId(); //set the id

    //handle request
    if (m_handlerFactory->getRoomManager().createRoom(m_user, data))
    {
        //if created room successfuly
        CR.status = status_OK;

        CR.roomId = data.id;
        //serialize response
        reqRes.responce = JsonRequestPacketSerializer::serializeResponse(CR);

        //move to room admin handler
        reqRes.newHandler = m_handlerFactory->createRoomAdminRequestHandler(m_user, m_handlerFactory->getRoomManager().getRoom(data.id));
    }
    else
    {
        //if room creation failed and returned false then the room name already exists
        ErrorResponse ER;
        ER.message = "error creating room! room name already exists!";
        
        //serialize error
        reqRes.responce = JsonRequestPacketSerializer::serializeResponse(ER);
        reqRes.newHandler = this; //stay in curr handler
    }

    return reqRes;
}
