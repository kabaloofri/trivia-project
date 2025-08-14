#include "GameRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <thread>
#include <sstream>

GameRequestHandler::GameRequestHandler(RequestHandlerFactory* RHF, Game& game, LoggedUser user):
    IRequestHandler(RHF) , m_game(game)
{
    this->m_user = user;
}




bool GameRequestHandler::isRequestRelevant(RequestInfo req) const
{
    switch (req.id)
    {
    case code_leaveGame:
        return true;
        break;
    case code_getQuestion:
        return true;
        break;
    case code_getGameResults:
        return true;
        break;
    case code_submitAnswer:
        return true;
        break;
    default:
        return false;
    }
}



RequestResult GameRequestHandler::handleRequest(RequestInfo req)
{
    RequestResult RS;

    if (!isRequestRelevant(req))
    {
        ErrorResponse ERR;
        ERR.message = "request code is invalid!";
        RS.newHandler = this;
        RS.responce = JsonRequestPacketSerializer::serializeResponse(ERR);
        return RS;
    }

    switch (req.id)
    {
    case code_getQuestion:
        RS = getQuestion(req);
        break;

    case code_submitAnswer:
        RS = submitAnswer(req);
        break;

    case code_getGameResults:
        RS = getGameResults(req);
        break;

    case code_leaveGame:
        RS = leaveGame(req);
        break;
    }

    return RS;
}




RequestResult GameRequestHandler::getQuestion(RequestInfo)
{

    Question Q = m_game.getQuestionForUser(m_user.getUsername());
    std::vector<std::string> possibleAnswers = Q.getPossibleAnswers();
    
    //wait for the players to finish theire question
    waitForPlayersToAnswer(m_game.getPlayers()[m_user.getUsername()].questionIndex); //wait for all the users to finish the final question
    
    GetQuestionResponse QR;
    QR.status = status_OK;
    QR.question = Q.getQuestion();


    //add the answers
    for (int i = 0; i < 4; i++)
    {
        QR.answers[std::to_string(i)] = (possibleAnswers[i]);
    }
    RequestResult RS;
    //seralize
    RS.responce = JsonRequestPacketSerializer::serializeResponse(QR);
    RS.newHandler = this;

    return RS;
}


RequestResult GameRequestHandler::submitAnswer(RequestInfo req)
{
    //deserialize request
    SubmitAnswerRequest answer = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(req.buffer.getStrBuffer());

    //create response for the client
    SubmitAnswerResponse SAR;
    SAR.status = status_OK;
    SAR.correctAnswerId = m_game.getAnswer(m_user.getUsername());

    //seralize
    RequestResult RS;
    RS.responce = JsonRequestPacketSerializer::serializeResponse(SAR);
    RS.newHandler = this;
    
    //send the answer to the manager
    m_game.submitAnswer(m_user.getUsername(), answer.AnswerID, answer.answerTime);

    //return requestResult
    return RS;
}

RequestResult GameRequestHandler::getGameResults(RequestInfo)
{
    m_game.increaseQuestionIndex(m_user.getUsername()); //update user finish the last question
    waitForPlayersToAnswer(m_game.getPlayers()[m_user.getUsername()].questionIndex); //wait for all the users to finish the final question

    //create response for the client
    std::map<std::string , GameData> playersResults = m_game.getPlayers();
    std::vector<PlayerResults> allPlayers;
    for (auto player : playersResults)
    {
        PlayerResults PR;

        std::stringstream s;
        s << std::setprecision(3) << player.second.averageAnswerTime;

        PR.averageAnswerTime = stod(s.str());
        PR.correctAnswerCount = player.second.correctAnswerCount;
        PR.wrongAnswerCount = player.second.wrongAnswerCount;
        PR.username = player.first;
        PR.score = player.second.currentScore;
        allPlayers.push_back(PR);
    }

    GetGameResultsResponse results;
    results.status = status_OK;
    results.results = allPlayers;


    //seralize
    RequestResult RS;
    RS.newHandler = nullptr;

    //change the room status
    if (m_handlerFactory->getRoomManager().getRoomState(m_game.getID()) != 0)
		m_handlerFactory->getRoomManager().getRoom(m_game.getID()).setStatus(state_waitingForPlayers);

	//if the game is in a room, check if the user is admin or member
    auto rooms = m_handlerFactory->getRoomManager().getRooms();
    for (auto roomData : rooms)
    {
        auto room = m_handlerFactory->getRoomManager().getRoom(roomData.id);
        auto roomUsers = room.getAllUsers();
        for (int i = 0; i < roomUsers.size(); i++)
        {
            if (roomUsers[i] == m_user.getUsername()) //if the player was found in a room
            {
                //admin
                if (i == 0)
                {
                    m_handlerFactory->getRoomManager().getRoom(roomData.id).setStatus(state_waitingForPlayers);
                    RS.newHandler = m_handlerFactory->createRoomAdminRequestHandler(m_user, m_handlerFactory->getRoomManager().getRoom(roomData.id));

                }
                
                //member
                else
                {
                    RS.newHandler = m_handlerFactory->createRoomMemberRequestHandler(m_user, m_handlerFactory->getRoomManager().getRoom(roomData.id));
                }

            }
        }
    }

    //if the room not exsist
    if (RS.newHandler == nullptr)
    {
        results.status = 1; //if not , refer to menu
        RS.newHandler = m_handlerFactory->CreateMenuRequestHandler(m_user.getUsername());
    }

    //delete the user
    m_game.removePlayer(m_user.getUsername());

    //if its the last player in the game
    if (m_game.getPlayers().size() == 0)
    {
        std::cout << "submited stats!\n";
        m_handlerFactory->getGameManager().submitQuestionsStatsToDB(allPlayers); //submit stats to the db

        m_handlerFactory->getGameManager().deleteGame(m_game.getID());     //delete the game
    }


    //return requestResult
    RS.responce = JsonRequestPacketSerializer::serializeResponse(results);
    return RS;
}

RequestResult GameRequestHandler::leaveGame(RequestInfo)
{
    RequestResult res;
    LeaveGameResponse LG;

    m_game.removePlayer(m_user.getUsername()); //remove the user

    //if its the last user
    if (m_game.getPlayers().size() == 0)
    {
        m_handlerFactory->getGameManager().deleteGame(m_game.getID()); //delete the game
    }

    //check if the user is admin if yes delete room
    auto rooms = m_handlerFactory->getRoomManager().getRooms();
    for (auto roomData : rooms)
    {
        auto room = m_handlerFactory->getRoomManager().getRoom(roomData.id);
        auto roomUsers = room.getAllUsers();
        for (int i = 0; i < roomUsers.size(); i++)
        {
            if (roomUsers[i] == m_user.getUsername()) //if the player was found in a room
            {
                //if its the admin
                if (i == 0)
                {
                    m_handlerFactory->getRoomManager().deleteRoom(roomData.id);
                }
            }
        }
    }

    LG.status = status_OK;
    res.responce = JsonRequestPacketSerializer::serializeResponse(LG);//serialzie response
    res.newHandler = m_handlerFactory->CreateMenuRequestHandler(m_user); //go back to menu
    return res;
}

void GameRequestHandler::waitForPlayersToAnswer(int currentQuestionIndex)
{
    bool approval = false;

    //while there is no same question to all the users the loop continue to run
    while (!approval)
    {
        approval = true;
        std::map<std::string, GameData> players = m_game.getPlayers();
        for (auto player : players)
        {
            if (player.second.questionIndex != currentQuestionIndex)
            {
                approval = false;
            }
        }
    }
}

