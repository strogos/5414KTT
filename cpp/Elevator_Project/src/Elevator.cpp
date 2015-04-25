/*
 * Elevator.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: bengteh
 */

#include "Control.h"
#include "Driver.h"
#include "Elevator.h"

#include <unistd.h>//sleep
#include "ace/Log_Msg.h"

namespace elevator
{
	static const int SPEED = 100;

	Elevator::Elevator(Control * ctrl_handle)
	        : ctrl_handle_(ctrl_handle)
	{
		ACE_DEBUG((LM_DEBUG,
						   "in elevator constructor\n"));

		//Initiate the driver interface for communicating with HW
		handle_driver_=std::unique_ptr<Driver>(new Driver(1,4));

		//subscribe to signaling slots in the control task
		signal_control_=ctrl_handle_->signal_subscribe(signal_control_);
	}
	Elevator::~Elevator(void){}

	int Elevator::open(void*)
	{
		ACE_DEBUG((LM_DEBUG,
				            "(%t) Active Elevator Object opened \n"));
		//activate object with a thread in it
		activate(THR_NEW_LWP,1); //using a kernel thread
		return 0;
	}

	int Elevator::close(u_long)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) Active Elevator Object is being closed down \n"));

		//ACE_Thread_Manager::instance()->cancel_task(ctrl_handle_);
		signal_control_->stop_task.emit(0);

		return 0;
	}

	int Elevator::svc(void)
	{
		poll_sensor_status(); //service the sensor/switch IO (this runs in it's own thread)
		return 0; //service done; ACE_Task will call Elevator::close() and close off
				  //any threads in this object
	}

	int Elevator::poll_sensor_status()
	{
		ACE_Thread_Manager *mgr = this->thr_mgr();
		handle_driver_->init(ctrl_handle_->get_session());

		while(true)
		{
			//check if thread manager wants to abort the current thread.
			if (mgr->testcancel(mgr->thr_self()))
				  return 0;






			signal_control_->button_press.emit(button_type_t::BUTTON_CALL_UP);
			usleep(1000000);
			signal_control_->button_press.emit(button_type_t::BUTTON_CALL_DOWN);
			usleep(1000000);
			signal_control_->button_press.emit(button_type_t::BUTTON_COMMAND);
			usleep(1000000);
			break;
		}
		return 0;
	}

	void Elevator::read_floor_sensor()
	{}

	void Elevator::read_buttons()
	{}

	void Elevator::read_stop_sensor()
	{}

	void Elevator::read_obstruct_sensor()
	{}

}

