/*
 * Control.cpp
 *
 *  Created on: Feb 11, 2015
 *      Author: bengteh
 *
 *      Note:
 *      This is an elevator control module.
 */

#include "Control.h"
#include "Driver.h"
#include "Elevator.h"

#include <unistd.h>//sleep
#include "ace/Log_Msg.h"

namespace elevator
{
	Control::Control(ElevatorType session)
	       : servicing_(false), session_(session)
	{
		ACE_DEBUG((LM_DEBUG,
						   "in control constructor\n"));

		//handle elevator IO using the signal/slot principle (inspired by Qt's implementation)
		ctrl_signal_ = std::unique_ptr<Control_Signals>(new Control_Signals);
		ctrl_signal_->button_press.connect(this,&Control::slot_button_press);
		ctrl_signal_->floor_sensor.connect(this,&Control::slot_floor_sensor);

		elevator_=std::unique_ptr<Elevator>(new Elevator(this));

		//start tasks (and their corresponding threads)
		elevator_->open(0);
		this->open(0);
	}

	Control::~Control()
	{
		// TODO Auto-generated destructor stub
	}

	int Control::open(void*)
	{
		ACE_DEBUG((LM_DEBUG,
							"(%t) Active Control Object opened \n"));
		//activate object with a thread in it
		//this->activate(THR_NEW_LWP | THR_SUSPENDED,1); //using a kernel thread
		activate();
		return 0;
	}

	int Control::close(u_long)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) Active Control Object is being closed down \n"));
		return 0;
	}

	int Control::svc(void)
	{
		ACE_Thread_Manager *mgr = this->thr_mgr ();

		while (true)
		{	//check whether thread manager wants to cancel thread
			if (mgr->testcancel (mgr->thr_self ()))
			          return 0;

			if (servicing_)
			{
				usleep(5000000);
				ACE_DEBUG((LM_DEBUG, "(%t) Control thread is servicing : %d\n"));
				servicing_=false;
			}
		}
		return 0; //service done; ACE_Task will call elevator::close() and close off
				  //any threads in this object
	}

	void Control::slot_button_press(button_type_t button)
	{
		ACE_DEBUG((LM_DEBUG,
							 "in slot for button press\n"));

		switch (button)
		{
			case BUTTON_CALL_DOWN:
				ACE_DEBUG((LM_DEBUG,
						   "User pressed CALL DOWN button\n"));
				servicing_=true;
				break;
			case BUTTON_CALL_UP:
							ACE_DEBUG((LM_DEBUG,
									   "User pressed CALL UP button\n"));
				servicing_=true;
							break;
			case BUTTON_COMMAND:
							ACE_DEBUG((LM_DEBUG,
									   "User pressed GO TO FLOOR button\n"));
				servicing_=true;
							break;
			default:
				break;
		}
	}

	void Control::slot_floor_sensor(int floor)
	{
		ACE_DEBUG((LM_DEBUG,
				   "Reached floor %d\n", floor));
	}

	void Control::svc_on_button_press()
	{
		usleep(1000000);
	}

	ElevatorType Control::get_session() {return session_;}

	Control_Signals * Control::signal_subscribe(Control_Signals * subscribe)
	{

		//memory mgmt on all signaling to slots in control shall be done in the control task itself
		subscribe=ctrl_signal_.get();
		if (subscribe)
			return subscribe;
		else
		{
			ACE_ERROR((LM_EMERGENCY,"PANIC: control SIGNAL subscription FAIL!\n"));
			throw std::bad_alloc();
		}

	}

}
