#pragma once

#include <condition_variable>

#include "Communicator.h"
#include "SqliteDatabase.h"


class Server
{
public:
    Server();
    ~Server();

    void run(int port);


private:



    //fields
    IDatabase* m_database;
    Communicator m_communicator;
    RequestHandlerFactory m_requestHandler;
};

