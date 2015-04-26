//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//extern "C"
//{
//	#include"elev.h"
//}
#pragma once

#include "Control.h"
#include "Driver.h"
#include "IPC_Client.h"
#include "IPC_Server.h"
#include "State.h"


#include "ace/Log_Msg.h"
#include <iostream>


void listener(int child_tid);
void primary_service(int,std::stringstream&,std::stringstream& ss);
void new_primary_service(int,std::stringstream&,std::stringstream&);

int main()
{
	ACE_DEBUG((LM_DEBUG,
					   "in main\n"));

	std::stringstream ss;
	std::stringstream ss_lan;

	new_primary_service(0,ss,ss_lan);

	//Wait for all the tasks to exit.
	ACE_Thread_Manager::instance()->wait();
	return 0;
}

void new_primary_service(int new_pid,std::stringstream& ss, std::stringstream& ss_lan)
{
	pid_t pid=fork();//vfork(); //create child process

	if (pid==0)
		primary_service(new_pid,ss,ss_lan); //child
	else if (pid>0)
		listener(pid);//parent
	else
		ACE_ERROR((LM_WARNING, "failed creating child(primary_service) process\n"));
}

void primary_service(int id,std::stringstream& ss,std::stringstream& ss_lan)
{

	elevator::State state;
	state.do_serialize(ss,state);
	std::cout<<"data in binary form: "<<ss.str()<<"\n";

	state.do_deserialize(ss,state);
	std::cout<<"data in converted back to normal form now?????\n";

	elevator::Control ctrl(elevator::SIMULATION,ss);
//	elevator::Control ctrl(elevator::COMEDI,ss);

	//Wait for all the tasks to exit.
//	ACE_Thread_Manager::instance()->wait();

	IPC_Client_Unicast::Client udp("localhost:42024");
	IPC_Client_Unicast::Client udp_2("localhost:42124");

	while(true)
	{
		udp.send_data(ss.str());
		udp_2.send_data(ss.str());
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		id++;
	}
}

void listener(int child_pid)
{
	int count=0;
	ACE_DEBUG((LM_DEBUG, "LISTENER IS ALIVE\n"));
	IPC_Server::Server listener(42024);
	IPC_Server::Server listener_lan(42124);
	unsigned int miss=0;
	std::stringstream ss;
	std::stringstream ss_lan;
	elevator::State state;

	/*check whether primary is alive*/
	while(true)
	{
		if ((listener.accept_data()))
		{
			ss<<listener.get_data();
			state.do_deserialize(ss,state);
		}
		else
			miss++;

		if ((listener_lan.accept_data()))
		{
			ss_lan<<listener_lan.get_data();
			state.do_deserialize(ss_lan,state);
		}
		else
			miss++;
		//count=atoi(listener.get_data().c_str());
//		if (miss==10)
//			break;
	}

	if (child_pid!=0)
	{
		/*Burn,loot, rape and kill primary_service (child process); it's not responding */
		kill(child_pid,SIGKILL);
		/*Wait for primary_service (child process to terminate)*/
		waitpid(child_pid,NULL,0);
	}
	new_primary_service(count,ss,ss_lan);//get the backup rolling as new
								//primary and send over the elevator state
}

