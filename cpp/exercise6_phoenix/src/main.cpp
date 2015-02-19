//============================================================================
// Name       : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define ACE_NDEBUG 0
#define ACE_NLOGGING 0

#include "IPC_Server.h"
#include "IPC_Client.h"

//#include <pthread.h> //Fuck this; will use c+11(boost) threads..
#include <thread>
#include <chrono>//thread sleep
#include <future>//TODO:channels between threads (shared state)->promise = "input from thread"; future = "output from thread"; Can also just use std::asynch instead
#include <vector>
#include <signal.h>
#include <memory>
#include "ace/Log_Msg.h"

#include <iostream>
using namespace std;

void  thread_worker1(int tid)
{
		IPC_Server::Server srv(42000);
		srv.accept_data();
		srv.~Server();
}

void  thread_worker2(int tid)
{
	IPC_Client_Broadcast::Client client(42000);
	while(1)
	{

		client.send_data("ALIVE");
	}
		client.~Client();
}

int thread_do_join(std::thread& t)
{
    t.join();

    return 1;
}

int thread_join_all(std::vector<std::thread>& my_threads)
{
    std::for_each(my_threads.begin(),my_threads.end(),thread_do_join);

    //	/*Join the threads with the main thread*/
    //	for(auto &thread : my_threads)
    //	{
    //		thread.join();
    //	}

    return 1;
}

void listener(int child_tid);
void primary_service(int);
void backup_service(int);
void new_primary_service(int);

int main(int argc, char* argv[])
{
	//const unsigned short nr_threads=2;
	std::vector<thread> my_threads;

	/*Launch a group of threads*/
	//my_threads.push_back(std::thread(thread_worker1,0));
	my_threads.push_back(std::thread(listener,0));
	my_threads.push_back(std::thread(thread_worker2,1));


	/*Synchronize threads*/
	if(!thread_join_all(my_threads))
		std::cout<<"error joining threads!\n";


//	pthread_t thread1;
//	pthread_t thread2;
//	pthread_create(&thread1,NULL,thread_worker1,NULL);
//	pthread_create(&thread2,NULL,thread_worker2,NULL);
//	/* wait for the second thread to finish */
//	if(pthread_join(thread1, NULL) && pthread_join(thread2, NULL))
//	{
//		cout<<"Error joining thread\n";
//		return -1;
//	}



	return 0;
}

void listener(int tid)
{
	ACE_DEBUG((LM_DEBUG, "LISTER IS ALIVE/n"));
	IPC_Server::Server listener(42000);
	unsigned int hit_rate_second=0;
	unsigned int hit=0;
	unsigned int hit_check_counter=0;

	/*wait to be primary*/
	while(true)
	{
		cout<<hit<<endl;
		/*check whether primary is alive*/
		if (listener.accept_data() && listener.get_data()=="$ALIVE€")
			hit++;
		cout<<"hits : "<<hit<<endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		hit_check_counter++;
		cout<<"hit check cnt: "<<hit_check_counter<<endl;
		/*Calculate hit_rate per second*/
		if (hit_check_counter==10)
		{
			cout<<"hit check==10!\n";
			hit_rate_second=(hit*100)/(hit_check_counter);
			cout<<"hit rate:"<<hit_rate_second<<"\n";
			hit_check_counter=0;
		}
		//cout<<"hit rate:"<<hit_rate_second<<"\n";
		/*50% hit rate acceptable?*/
		if (hit_rate_second>5)
		{
			ACE_DEBUG((LM_DEBUG, "Hit rate: %d Backup is assuming primary role/n",hit_rate_second));
			break;
		}
	}


/*	if (data_available)
		//check msg for  PRIM_ALIVE!!
		if (!PRIM_ALIVE)*/




}
