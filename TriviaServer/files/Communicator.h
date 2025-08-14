#pragma once

#include <WinSock2.h>
#include <map>
#include <thread>
#include <mutex>
#include "SqliteDatabase.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "Helper.h"
#include "RequestHandlerFactory.h"

enum RequestType
{
	join_room = code_joinRoom,
	create_room = code_createRoom,
	high_score = code_getHighScore,
};

class Communicator
{
public:
	Communicator();
	void startHandlingRequests(int port, RequestHandlerFactory*);
	
private:
	void bindAndListen(int port);
	void connectorThread();
	void handleNewClient(SOCKET);

	
	RequestInfo getRequestInfo(Buffer buffer, int code);


	std::map<SOCKET, IRequestHandler*> m_clients;


	 RequestHandlerFactory* m_handlerFactory;
	// ICryptoAlgorithem* m_cryptoAlgorithem;

	 SOCKET m_serverSocket;
	
	 
	std::map<RequestType, int> m_numberOfRequests;
	void addRequestToList(int code);
	void printNumberOfRequestsThread();
};








