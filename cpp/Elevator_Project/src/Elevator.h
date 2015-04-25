/*
 * Elevator.h
 *
 *  Created on: Apr 22, 2015
 *      Author: bengteh
 *
 *  Note:
 *  	This class is defined as an ACE task with a single thread which
 *  	polls sensor and button status
 */

#ifndef ELEVATOR_H_
#define ELEVATOR_H_

#include "tools/signalslot/W_Slot.h"
#include "tools/signalslot/W_Signal.h"

#include <memory>
#include "ace/Task.h"

namespace elevator
{
	/*FWD DECLARATIONS*/
	class Control;
	class Driver;
	enum ElevatorType : int;
	typedef ElevatorType ElevatorType;
	enum tag_lamp_type : int;
	typedef tag_lamp_type button_type_t;


	/*ELEVATOR [ace]TASK */
	class Elevator : public ACE_Task<ACE_MT_SYNCH>,
	                 public W::Slot
	{
		public:
			Elevator(Control * ctrl_handle);//TODO: implement Elevator
											//Object from virtual class
			~Elevator(void);

			//Implement the ACE task specific service init/term methods
			int open(void*);
			int close (u_long);
			int svc(void);

		private:
			/*VARIABLES*/
			bool elevator_running_=false;
			Control * ctrl_handle_=nullptr;
			std::unique_ptr<Driver> handle_driver_=nullptr;
			Control_Signals * signal_control_=nullptr;

			/*FUNCTIONS*/
			int poll_sensor_status();
			void read_floor_sensor();
			void read_buttons();
			void read_stop_sensor();
			void read_obstruct_sensor();

	};
}
#endif /* ELEVATOR_H_ */
