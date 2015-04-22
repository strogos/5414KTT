/*
 * Control.h
 *
 *  Created on: Feb 11, 2015
 *      Author: bengteh
 */
#ifndef CONTROL_H_
#define CONTROL_H_

#include "Driver.h"
#include "Elevator.h"

#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Message_Block.h>
#include <ace/Reactor.h>
#include <ace/Event_Handler.h>
#include <ace/Thread_Manager.h>



namespace Elevator
{
	class Control : //public ACE_Task<ACE_MT_SYNCH>//, //Elevator control task
                   public ACE_Event_Handler
	{
		public:
			Control();
			~Control();

			int handle_signal(button_type_t button_pressed);
		private:
			Driver *driver_;
			Elevator *elevator_;

			//*local network socket handlers
			//*elevator network handler



	};
}
#endif /* CONTROL_H_ */
