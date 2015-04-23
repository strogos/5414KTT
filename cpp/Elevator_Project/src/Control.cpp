/*
 * elevator.cpp
 *	TODO: Implement elevator API state machine here
 *  Created on: Feb 11, 2015
 *      Author: bengteh
 */

#include "Control.h"
#include "Driver.h"
#include "Elevator.h"

#include "ace/Log_Msg.h"

namespace Elevator
{
	Control::Control()
	{
		ACE_DEBUG((LM_DEBUG,
						   "in control constructor\n"));
		elevator_=new Elevator(this);
		elevator_->open(0);


		//handle elevator IO using the signal/slots principle
	//	ctrl_signals->signal_button_press.connect()

	}

	Control::~Control()
	{
	//	delete elevator_;
		// TODO Auto-generated destructor stub
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
					break;
				case BUTTON_CALL_UP:
								ACE_DEBUG((LM_DEBUG,
										   "User pressed CALL UP button\n"));
								break;
				case BUTTON_COMMAND:
								ACE_DEBUG((LM_DEBUG,
										   "User pressed GO TO FLOOR button\n"));
								break;
				default:
					break;
			}
		}

	void Control::slot_floor_prox(int floor)
	{
		ACE_DEBUG((LM_DEBUG,
				   "Reached floor %d\n", floor));
	}
}
