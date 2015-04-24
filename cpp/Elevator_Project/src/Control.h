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
#include <memory>
#include <atomic>


namespace elevator
{
	/*FWD DECLARATIONS*/
	class Elevator;
	enum tag_lamp_type : int;
	typedef tag_lamp_type button_type_t;

	enum class Serviced_Slot : int {ON_BUTTON, ON_FLOOR};

	struct Control_Signals /*thread-safe*/
	{
		W::Signal<button_type_t>button_press;     //signal for user input
		W::Signal<int> floor_sensor;              //signal for floor proximity sensors
		W::Signal<int> stop_sensor;               //signal for stop proximity sensors
		W::Signal<int> obstruct_sensor;           //signal for obstruction proximity sensor
	};

	/*CONTROL [ace]TASK */
	class Control : public ACE_Task<ACE_MT_SYNCH>,
		            public W::Slot

	{
		public:
			Control();
			~Control();

			//Implement the ACE specific service init/termination methods
			int open(void*);
			int close (u_long);
			int svc(void);

			/*FUNCTIONS*/
			//function slots to to launch on signal
			void slot_button_press(button_type_t button);
			void slot_floor_sensor(int floor);

			/*SIGNALS*/
	//		std::unique_ptr<Control_Signals> ctrl_signal=nullptr;

		private:
			std::unique_ptr<Elevator> elevator_;
			std::atomic<bool> servicing_;//(false);
			//*local network socket handlers
			//*elevator network handler

			/*FUNCTION*/
			void svc_on_button_press();
			void svc_on_floor_sensor();




	};
}
#endif /* CONTROL_H_ */

