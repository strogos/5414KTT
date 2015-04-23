/*
 * Elevator.h
 *
 *  Created on: Apr 22, 2015
 *      Author: bengteh
 */

#ifndef ELEVATOR_H_
#define ELEVATOR_H_

#include "tools/signalslot/W_Slot.h"
#include "tools/signalslot/W_Signal.h"

#include "ace/Task.h"
//#include <ace/Reactor.h>
//#include <ace/Event_Handler.h>

namespace Elevator
{
	/*FWD DECLARATIONS*/
	class Control;
	class Driver;
	enum tag_lamp_type : int;
	typedef tag_lamp_type button_type_t;

	struct Control_Signals /*thread-safe*/
	{
		W::Signal<button_type_t>button_press;  //signal for user input
		W::Signal<int> floor_prox;              //signal for floor proximity sensors
		W::Signal<int> stop_prox;               //signal for stop proximity sensors
		W::Signal<int> obstruct_prox;           //signal for obstruction proximity sensor
	};

	/*ELEVATOR [ace]TASK */
	class Elevator : public ACE_Task<ACE_MT_SYNCH>,
	                 public W::Slot
	{
		public:
			Elevator(Control * ctrl_handle);
			~Elevator(void);

			//Implement the ACE specific service init/termination methods
			int open(void*);
			int close (u_long);
			int svc(void);

		private:
			/*VARIABLES*/
			bool elevator_running_=false;
			Control_Signals *ctrl_signal_=nullptr;
			Control * ctrl_handle_=nullptr;
			Driver * handle_driver_=nullptr;

			/*FUNCTIONS*/
			void poll_sensor_status();
			void read_floor_prox();
			void read_buttons();
			void read_stop_prox();
			void read_obstruct_prox();

	};
}
#endif /* ELEVATOR_H_ */
