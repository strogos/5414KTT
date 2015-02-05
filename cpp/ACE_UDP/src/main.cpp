//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello ACE  UDP in C++, Ansi-style
//============================================================================

#include "IPC_Client.h"
#include "IPC_Server.h"
//#include <pthread.h>
#include "ace/Thread_Manager.h"
#include <vector>
#include <iostream>
using namespace std;

void *thread_worker(void*)
{
	IPC_Client_Unicast::Client client_u("localhost:28000");
	client_u.send_data();
	client_u.~Client();
	return NULL;
}

int main()
{
	IPC_Server::Server server(20011);
	server.accept_data();
	server.~Server();

	IPC_Client_Broadcast::Client client(20011);
	client.send_data();
	client.~Client();

	return 0;
}
