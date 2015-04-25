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
	enum  tag_elevator_type : int;
	typedef tag_elevator_type elevator_type;
	enum tag_lamp_type : int;
	typedef tag_lamp_type button_type_t;
	enum tag_motor_direction : int;
	typedef tag_motor_direction motor_direction_t;


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

			/*FUCNTIONS*/
			void set_floor_indicator(int floor);

		private:
			/*VARIABLES*/
			const int SPEED_ = 100;
			bool is_running_=false;
		    int floor_=-1;
			int requested_floor_=-1;
		    int direction_= DIRN_DOWN;
			Control * ctrl_handle_=nullptr;
			std::unique_ptr<Driver> handle_driver_;
			Control_Signals * signal_control_=nullptr;

			/*FUNCTIONS*/
			int poll_sensor_status();
			void read_floor_sensor();
			void read_buttons();
			void read_stop_sensor();
			void read_obstruct_sensor();

			void on_floor_sensor();

	};
}
#endif /* ELEVATOR_H_ */
