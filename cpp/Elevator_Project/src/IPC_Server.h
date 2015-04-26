//============================================================================
// Name        : IPC_Server.h
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : ACE IPC wrapper
//============================================================================
#ifndef ICP_SERVER_H_
#define ICP_SERVER_H_


//
//#ifndef ACE_NDEBUG
//#define ACE_NDEBUG 1
//#endif
//#ifndef ACE_NLOGGING
//#define ACE_NLOGGING 1
//#endif



#include "ace/Log_Msg.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

#include <string>

namespace IPC_Server
{

	class Server //: public ACE_Event_Handler
	{
	public:
		Server(const unsigned short local_port);
		Server(const unsigned short local_port, const unsigned short data_buffer_size, const unsigned short dgram_byte_size);
		//virtual ~Server();
		~Server();

		std::string get_data();
		int accept_data();
		int send_data();

	private:
		ACE_INET_Addr remote_addr_;
		ACE_INET_Addr local_addr_;
		ACE_SOCK_Dgram socket_;
		char *data_buff_;
		std::string data_string_;
		int found_msg_end_=0;
		int found_msg_start_=0;
		const unsigned short dgram_byte_size_;

	};
}

#endif /* ICP_SERVER_H_ */
