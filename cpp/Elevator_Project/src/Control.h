/*
 * Control.h
 *
 *  Created on: Feb 11, 2015
 *      Author: bengteh
 */
#ifndef CONTROL_H_
#define CONTROL_H_


#include "tools/signalslot/W_Slot.h"
#include "tools/signalslot/W_Signal.h"


//#include <ace/OS.h>
#include "ace/Task.h"
//#include <ace/Message_Block.h>
//#include <ace/Reactor.h>
//#include <ace/Event_Handler.h>
//#include <ace/Thread_Manager.h>



namespace Elevator
{
	/*FWD DECLARATIONS*/
	class Elevator;
	enum tag_lamp_type : int;
	typedef tag_lamp_type button_type_t;

	/*CONTROL [ace]TASK */
	class Control : public ACE_Task<ACE_MT_SYNCH>,
		            public W::Slot
			     // public ACE_Event_Handler
	{
		public:
			Control();
			~Control();




			/*FUNCTIONS*/
			//function slots to to launch on signal
			void slot_button_press(button_type_t button);
			void slot_floor_prox(int floor);


		private:
			//Driver *driver_;
			Elevator *elevator_=nullptr;
		//	Control_Signals * ctrl_signals=nullptr;

			//*local network socket handlers
			//*elevator network handler




	};
}
#endif /* CONTROL_H_ */

