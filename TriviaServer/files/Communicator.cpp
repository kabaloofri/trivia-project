#include "Communicator.h"
#include <string>
#include <bitset>
#include <chrono>
#include <typeinfo>
#include "LoginRequestHandler.h"

Communicator::Communicator()
{
	m_clients = std::map<SOCKET, IRequestHandler*>();
	m_handlerFactory = nullptr;

	m_numberOfRequests = std::map<RequestType, int>();
	m_numberOfRequests.emplace(RequestType::join_room, 0);
	m_numberOfRequests.emplace(RequestType::create_room, 0);
	m_numberOfRequests.emplace(RequestType::high_score, 0); //init the map
}	

void Communicator::startHandlingRequests(int port, RequestHandlerFactory* RHF)
{
	m_handlerFactory = RHF;

	//create server socket

	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
	{
		int errorCode = WSAGetLastError();
		std::cerr << "Socket creation failed with error: " << errorCode << std::endl;
		throw std::exception(__FUNCTION__ " - socket");
	}

	// listen for clients
	bindAndListen(port);
	

	// connect each client and create a new thread for him
	
	std::thread connectorThread(&Communicator::connectorThread, this);
	connectorThread.join(); //wait until the thread is done (never)
}

void Communicator::bindAndListen(int port)
{

	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;
}


void Communicator::connectorThread()
{
	std::cout << "Server is running! Waiting for client connection requests...\n";

	//start the print thread
	auto t = std::thread(&Communicator::printNumberOfRequestsThread, this);
	t.detach();


	while (true)
	{
		// this accepts the client and create a specific socket from server to this client
		// the process will not continue until a client connects to the server
		SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << " Client accepted. Server and client can speak\n";


		

		// the function that handle the conversation with the client
		std::thread clientThread(&Communicator::handleNewClient, this, clientSocket);
		clientThread.detach(); //make it run at its own
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	IRequestHandler* handler = nullptr;
	std::string username;
	try
	{
		//place the client in the list
		m_clients.emplace(clientSocket, m_handlerFactory->createLoginRequestHandler());
		//start with login
		handler = m_clients.find(clientSocket)->second;
		while (true)
		{

			// *** recive request from the client ***

			//get a msg from the client:
			//get the code from hex
			std::string temp = Helper::getPartFromSocket(clientSocket, 2);

			int code = atoi(temp.c_str()); //convert to int

			if (code == 0)
			{
				throw std::exception("client has suddnely disconnected!");
			}

			int len = atoi(Helper::getStringPartFromSocket(clientSocket, 4).c_str());
			
			
			// if length is 0 client is asking for update requests like get rooms
			RequestInfo info;
			
			if (len != 0)
			{
				Buffer buff(Helper::getPartFromSocket(clientSocket, len)); //take binary string

				std::cout << "request: " << code << "," << len << "," << buff.getStrBuffer() << "\n";

				//deserilize it
				info = getRequestInfo(buff, code);
			}
			else
			{
				//no need for deserialization if the request has no json
				info.id = code;
				info.recivedTime = time(0);

				std::cout << "request: " << code << "\n";
			}
			RequestResult res;
			


			//add requests to the counter
			addRequestToList(code);


			// *** handle client request ***

			if (handler != nullptr)
			{
				res = handler->handleRequest(info);
			}
			else
			{
				throw std::exception("handler was null");
			}

			// *** try to set username ***

			if (username.empty()) //if the username wasent set
			{
				auto menuHander = dynamic_cast<MenuRequestHandler*>(handler);

				//check to see if the handler is a menuHandler
				if (menuHander != nullptr)
				{
					//if it is, save the username
					username = menuHander->getUsername();
				}
			}

			// *** switch handlers ***

			//delete dynamicly allocated handler (if allocated new handler)
			if (res.newHandler != handler)
			{
				delete handler; 
			}

			//set new handler
			handler = res.newHandler; 
			std::cout << "server resopnse: " << res.responce.getStrBuffer() << "\n";


			// *** send responce ***
			Helper::sendData(clientSocket, res.responce.getStrBuffer());


		}
	}
	catch (const std::exception& e)
	{
		std::cout << "error: " << e.what() << "\n";

		// *** handle client disconnection ***



		if (!username.empty()) //if the username has been saved try to remove him from the servers data
		{
			//check if the client was in a room
			auto rooms =  m_handlerFactory->getRoomManager().getRooms();
			for (auto& roomData : rooms)
			{
				auto room = m_handlerFactory->getRoomManager().getRoom(roomData.id);
				auto roomUsers = room.getAllUsers();
				for (int i = 0; i < roomUsers.size(); i++)
				{
					if (roomUsers[i] == username) //if the player was found in a room
					{
						if (i == 0)
						{
							//if the index is 0 then the user was the admin, so close room
							if (typeid(*handler) == typeid(GameRequestHandler))
							{
								delete(handler); //in case its gameHandler change to admin handler
								handler = m_handlerFactory->createRoomAdminRequestHandler(username, room);
							}
							
							RequestInfo req;
							req.id = code_closeRoom;
							handler->handleRequest(req); //close the room
						}
						else
						{
							if (typeid(*handler) == typeid(GameRequestHandler))
							{
								delete(handler); //in case its gameHandler change to memerHandler handler
								handler = m_handlerFactory->createRoomMemberRequestHandler(username, room);
							}

							//if the user is just a reqular player, remove him from the room
							LoggedUser user(username);
							room.removeUser(user);
						}
					}
				}
			}
			//log user out from the loginManager
			std::cout << "user logged out: " << username << "\n";
			m_handlerFactory->getLoginManager().logout(username);
		}

		delete handler;
		m_clients.erase(clientSocket); //remove client from list

		closesocket(clientSocket);

		std::cout << "\nclient disconnected!\n";
	}
}

RequestInfo Communicator::getRequestInfo(Buffer buffer, int code)
{

	RequestInfo info;
	info.buffer = buffer;
	info.id = code;
	info.recivedTime = time(0);
	return info;
}





void Communicator::addRequestToList(int code)
{

	switch (code)
	{
	case RequestType::join_room:
		m_numberOfRequests[join_room] += 1;
		break;
	
	case RequestType::create_room:
		m_numberOfRequests[create_room] += 1;
		break;

	case RequestType::high_score:
		m_numberOfRequests[high_score] += 1;
		break;

	default:
		break;
	}
}

void Communicator::printNumberOfRequestsThread()
{
	while (true)
	{
		std::cout << "number of joinRoomRequests: " << m_numberOfRequests[join_room] << ", create room requests: " << m_numberOfRequests[create_room] << ", high scores request: " << m_numberOfRequests[high_score] << "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
}
