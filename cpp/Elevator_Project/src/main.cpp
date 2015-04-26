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
void primary_service(int,std::stringstream&);
void new_primary_service(int,std::stringstream&);

int main()
{
	ACE_DEBUG((LM_DEBUG,
					   "in main\n"));

	std::stringstream ss;
	new_primary_service(0,ss);

	//Wait for all the tasks to exit.
	ACE_Thread_Manager::instance()->wait();
	return 0;
}

void new_primary_service(int new_pid,std::stringstream& ss)
{
	pid_t pid=fork();//vfork(); //create child process

	if (pid==0)
		primary_service(new_pid,ss); //child
	else if (pid>0)
		listener(pid);//parent
	else
		ACE_ERROR((LM_WARNING, "failed creating child(primary_service) process\n"));
}

void primary_service(int id,std::stringstream& ss)
{
	//connect signals to slots
//	signal(SIG_INTERVAL_TIMER,signal_int_timer_test);
//	signal(SIG_ONESHOT_TIMER,signal_oneshot_timer_test);

	//std::stringstream ss;
	elevator::State test;
	test.do_serialize(ss,test);
	std::cout<<"data in binary form: "<<ss.str()<<"\n";

	test.do_deserialize(ss,test);
	std::cout<<"data in converted back to normal form now?????\n";

	elevator::Control ctrl(elevator::SIMULATION,ss);
//	elevator::Control ctrl(elevator::COMEDI,ss);


	IPC_Client_Unicast::Client udp("localhost:42000");

	while(true)
	{
		udp.send_data("123456789012345678901324567");//std::to_string(count+1));//"ALIVE");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		id++;
	}
}

void listener(int child_pid)
{
	int count=0;
	ACE_DEBUG((LM_DEBUG, "LISTENER IS ALIVE\n"));
	IPC_Server::Server listener(42024);
	unsigned int miss=0;
	std::stringstream ss;

	/*check whether primary is alive*/
	while(true)
	{
		if (!(listener.accept_data()))// && listener.get_data()=="$ALIVE€"))
			miss++;

		//count=atoi(listener.get_data().c_str());
	}

	if (child_pid!=0)
	{
		/*Burn,loot, rape and kill primary_service (child process); it's not responding */
		kill(child_pid,SIGKILL);
		/*Wait for primary_service (child process to terminate)*/
		waitpid(child_pid,NULL,0);
	}
	new_primary_service(count,ss);//get the backup rolling as new
								//primary and init it with the last known counter status
}

