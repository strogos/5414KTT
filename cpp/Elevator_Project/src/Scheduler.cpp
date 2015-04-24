/*
 * Scheduler.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: bengteh
 */

#include "Scheduler.h"
#include "ace/Auto_Ptr.h"

Scheduler::Scheduler()
{
	ACE_TRACE (ACE_TEXT ("Scheduler::Scheduler"));//ACE_TRACE keeps track of
													//which methods are entered and exited
	this->activate();
}

int Scheduler::svc (void)
{
	ACE_TRACE (ACE_TEXT ("Scheduler::svc"));

	while (1)
	  {
		// Dequeue the next method object
		auto_ptr<ACE_Method_Request>
		  request (this->activation_queue_.dequeue ());

		// Invoke the method request.
		if (request->call () == -1)
		  break;
	  }

	return 0;
}

int Scheduler::enqueue (ACE_Method_Request *request)
{
	ACE_TRACE (ACE_TEXT ("Scheduler::enqueue"));
	return this->activation_queue_.enqueue (request);
}


