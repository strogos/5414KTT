/*
 * Timer.h
 *
 *  Created on: Apr 23, 2015
 *      Author: bengteh
 *
 *  Note: This is a timer which can be configured to signal (process-wide)
 *        on interrupt, one-shot or it can be configured to simply run and
 *        stop on client code request.
 *        FIY; because of time restrictions -> POSIX only for now
 *        and only one of each INTERVAL and ONESHOT timer running at a time
 *        as only two signals are implemented!!
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "ace/Task.h"

#include  <memory>
#include <signal.h>

#define SIGNAL_INTERVAL 10 //SIGUSR1
#define SIGNAL_ONESHOT 12 //SIGUSR2

enum class Timer_Type : int {ONE_SHOT=0,INTERVAL=1, RUNNING=2};

class Timer : public ACE_Task<ACE_MT_SYNCH>
{
public:
	Timer(Timer_Type tt);               //start stop timer
	Timer(Timer_Type tt, long time_ms);    //one-shot timer or interval timer



	virtual ~Timer();
//	Timer(Timer&& rhs); // move constructor
//	Timer& operator=(Timer&& rhs); // move assignment


	/*Implement the ACE specific service init/termination methods*/
	int open(void*);
	int close (u_long);
	int svc(void);

	/*FUNCTIONS*/
	int get_id();
	long long get_ms_time();
	bool is_running();
	void stop();

	private:
//		/*if we want to copy an object to a new object when using a unique_ptr we must do the following*/
//		Timer(const Timer&);
//		Timer& operator=(const Timer&);

		class Handler;

		//std::unique_ptr<Handler> handler_;
		Handler *handler_=nullptr;
		long time_ms_=0;
		Timer_Type timer_type_;
};

#endif /* TIMER_H_ */
