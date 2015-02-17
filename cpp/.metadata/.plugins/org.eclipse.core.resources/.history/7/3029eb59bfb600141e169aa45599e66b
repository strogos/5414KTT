//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "IPC_Server.h"
#include "IPC_Client.h"

#include <iostream>
using namespace std;

int main()
{
//	IPC_Server::Server srv(42000);
//	srv.accept_data();
//	srv.~Server();
	IPC_Client_Broadcast::Client client(42000);
	client.send_data("shit");
	client.~Client();

	return 0;
}
