/*
 * ACE_event_test.h
 *
 *  Created on: Apr 21, 2015
 *      Author: bengteh
 */

#ifndef ACE_EVENT_TEST_H_
#define ACE_EVENT_TEST_H_


#include <signal.h>
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"

//Create our subclass to handle the signal events
//that we wish to handle. Since we know that this particular
//event handler is going to be using signals we only overload the
//handle_signal method.
/*singleton class (private constructor)*/
class MyEventHandler : public ACE_Event_Handler
{
	int handle_signal(int signum, siginfo_t*,ucontext_t*)//overload handle_signal in ace event handler
	{
		switch(signum)
		{
			case SIGWINCH:
				ACE_DEBUG((LM_DEBUG, "You pressed SIGWINCH \n"));
				break;

			case SIGINT:
				ACE_DEBUG((LM_DEBUG, "You pressed SIGINT \n"));
				break;
		}
		return 0;
	}
};



#endif /* ACE_EVENT_TEST_H_ */
