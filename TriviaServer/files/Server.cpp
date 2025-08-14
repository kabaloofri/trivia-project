#include "Server.h"
#include <exception>
#include <fstream>
#include <chrono>


Server::Server()
{
	m_database = new SqliteDatabase();
	m_requestHandler = RequestHandlerFactory(m_database);
	m_communicator = Communicator();
}

Server::~Server()
{
	delete m_database;
}


void Server::run(int port)
{
	m_communicator.startHandlingRequests(port, &m_requestHandler);
}

