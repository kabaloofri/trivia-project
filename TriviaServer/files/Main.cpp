#include <iostream>
#include "WSAInitializer.h"
#include "LoginManager.h"
#include "Server.h"

int main()
{
	try
	{
		WSAInitializer init;
		Server server;

		server.run(1500);
	}
	catch (std::exception& e)
	{
		std::cout << "cought error: " << e.what() << "\n";
	}
	catch (...)
	{
		std::cout << "cought an unknown error!\n";
	}

	return 0;
}