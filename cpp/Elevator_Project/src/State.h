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

#include "tools/cereal/archives/binary.hpp"
#include <sstream>

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

	struct Command_Button_State
	{
		Button call[N_FLOORS];
		  template<class Archive>
			  void serialize(Archive & archive)
			  {
				  archive(call);
			//    archive( direction, call, remote, button_type,floor ); // serialize things by passing them to the archive
			  }
	};

	struct State //task
	{
		  int direction=0;
		  Button call[3][N_FLOORS];
		  clock_time::msec call_container_frst[3][N_FLOORS];
		  bool call_container_sec[3][N_FLOORS];


		  std::map<uint32_t,Command_Button_State> remote;
		  uint32_t remote_container_frst[N_FLOORS];
		  Button remote_container_sec[N_FLOORS];

		  button_type_t button_type;
		  int floor=3;

		  std::stringstream ss;
		  template<class Archive>
		  void serialize(Archive & archive)
		  {
			  archive(floor,direction,call_container_frst,call_container_sec); // serialize things by passing them to the archive

		  }

		  void manage_call_containers();
		  void manage_remote_containers(Command_Button_State &int_state);
		  void set_serialize();
		  void set_deserialize();

		 // std::stringstream serialize();
		  bool deserialize(const std::stringstream &ss);

		private:
		  bool serialize_=false;
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

