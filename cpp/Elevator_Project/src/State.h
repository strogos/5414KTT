/*
 * State.h
 *
 *  Created on: Apr 22, 2015
 *      Author: bengteh
 */

#ifndef STATE_H_
#define STATE_H_

#include "time.h"
#include "io.h"

#include <utility>
#include <map>
#include <ace/Message_Block.h>
#include <ace/CDR_Stream.h>//TODO: should use this to minimize traffic


namespace elevator
{
	typedef std::pair<clock_time::msec,bool> Button; //includes time stamp!

	/*FWD DECLARATIONS*/
	enum tag_lamp_type : int;
	typedef tag_lamp_type button_type_t;

	struct Command_Button_State { Button call[N_FLOORS];};

	struct State //task
	{
		  int direction;
		  Button call[N_BUTTONS][N_FLOORS];
		  std::map<uint32_t,Command_Button_State> remote;
		  button_type_t button_type;
		  int floor;


//		  bool dequeue_state();
//		  bool enqueue_state();

//		  ACE_InputCDR ace_icdr()
//		  bool deserialize(const QByteArray &state);
//		  QByteArray serialize();

		private:
		  //queue
	};


//	struct State; //: public ACE_Data_Block
//	{
//		//use DAta block object and change udp to send message_block (essentially pointer to data blocks)?
//		//
//		//dataBlock=direction;
//		//dataBlock2=floor;
//	};
}




#endif /* STATE_H_ */
