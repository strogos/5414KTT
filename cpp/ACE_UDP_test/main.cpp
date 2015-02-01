/*
* main.cpp
*
* Created on: Jan 29, 2015
* Author: bengteh
*/
#pragma once

#include "ace/ACE.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Thread_Manager.h"

#include "IPC_Client_Server.h"

#include <iostream>
//#include <string>
#include<sstream>
using namespace std;

int send_broadcast(unsigned short port,char *msg_to_send)
{
	ACE_INET_Addr my_addr(static_cast<unsigned short>(10101));
	ACE_SOCK_Dgram_Bcast udp(my_addr);
	ssize_t sent=udp.send((const char*)msg_to_send,ACE_OS_String::strlen(msg_to_send)+1,port);
	udp.close();
	if(sent==-1)
	{
		ACE_ERROR_RETURN((LM_ERROR,
		ACE_TEXT("%p\n"),
		ACE_TEXT("send")),-1);
	}
	return 0;
}
int send_unicast(const char *address_and_port,const char *msg_to_send)
{
	const ACE_INET_Addr addr(address_and_port);
	ACE_INET_Addr my_addr(static_cast<unsigned short>(10101));
	ACE_SOCK_Dgram udp(my_addr);
	ssize_t sent=udp.send((const char*)msg_to_send,ACE_OS_String::strlen(msg_to_send)+1,addr);
	udp.close();
	if(sent==-1)
	{
		ACE_ERROR_RETURN((LM_ERROR,
		ACE_TEXT("%p\n"),
		ACE_TEXT("send")),-1);
	}
	return 0;
}

int listen (const string port)
{//USE COD?? see chapter for udp(IPC)

	//ACE_TCHAR peer_name[MAXHOSTNAMELEN];
	ACE_INET_Addr myAddr_(static_cast<unsigned short>(10101));
	ACE_INET_Addr remAddr_(port.c_str());
	ACE_SOCK_Dgram udp_(myAddr_);

	char dataBuffer[4096];
	ssize_t bytes_received=0;

	cout<<"...listening on host: "<<remAddr_.get_host_name()<<endl;
	while((bytes_received = udp_.recv(dataBuffer,sizeof(dataBuffer), remAddr_)) != -1)
	{
		dataBuffer[bytes_received]=0;
		ACE_DEBUG((LM_DEBUG, "Data received from remote %s was %s \n"
						,remAddr_.get_host_name(), dataBuffer));
		cout<<"remote address ("<<remAddr_.get_host_name()<<") said: "<<dataBuffer<<endl;
		ACE_OS::sleep(1);

		stringstream concatStrings;
		concatStrings<<remAddr_.get_host_addr()<<":"<<remAddr_.get_port_number();
		//send ack
		if(send_unicast(concatStrings.str().c_str(),"Thank you!\n")==-1)
			break;

	}
	return -1;
}

ACE_THR_FUNC_RETURN fn(void * arg)
{
	IPC_Client_Server::Sender* se = (IPC_Client_Server::Sender*) arg;
	string cmd;

	while(true)
	{
		cout<<"Enter msg to send\n";
		cin>>cmd;
		if("exit"==cmd)
			break;
		se->send(cmd.c_str());
		ACE_OS::sleep(1);
	}

   ACE_Proactor::instance()->proactor_end_event_loop();
   return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{


	//IPC_Client_Server::Receiver re(ACE_Proactor::instance(),"localhost:20001");
	//IPC_Client_Server::Sender se(ACE_Proactor::instance(),"localhost:20001");
	//ACE_Thread_Manager tm;
	//tm.spawn(fn,&se);
	//ACE_Proactor::instance()->proactor_end_event_loop();
	//tm.wait();


	//IPC_Client_Server::Client client("localhost:31246");
	//client.send_data();
	IPC_Client_Server::Server srvr(31000);

	srvr.accept_data();



	char c='N';
	int i=0;
	while (c!='b')
	{

		send_broadcast(31000,"BROADCAST\n");
		//send_unicast("localhost:20001","suckie dickie");

		cout<<"sent msg no: "<<i<<endl;
		i++;
		cin>>c;

	}
	//listen("129.241.223.189:20001");



	return 0;
}
