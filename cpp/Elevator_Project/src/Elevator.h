/*
 * Elevator.h
 *
 *  Created on: Apr 22, 2015
 *      Author: bengteh
 */

#ifndef ELEVATOR_H_
#define ELEVATOR_H_

#include <ace/Log_Msg.h>
#include <ace/Task.h>
#include <ace/Reactor.h>
#include <ace/Event_Handler.h>


namespace Elevator
{
	class Elevator : public ACE_Event_Handler
	{
		public:
			Elevator();
			~Elevator();



			void start();

		private:
	};
}




#endif /* ELEVATOR_H_ */
