//============================================================================
// Name        : ICP_Client.h
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : ACE IPC wrapper
//============================================================================
#ifndef IPC_CLIENT_H_
#define IPC_CLIENT_H_

#include "ace/Log_Msg.h"
#include "ace/SOCK_Dgram.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/INET_Addr.h"

#include <iostream>
#include <string>


namespace IPC_Client_Unicast
{
	class Client //: public ACE_Event_Handler
	{
		public:
			Client(const char * remote_host_and_port);
			Client (const char * remote_host_and_port, const unsigned short data_buffer_size,const unsigned short dgram_byte_size);
			//virtual ~Client();
			~Client();

			int send_data(std::string msg);


		private:
			ACE_INET_Addr remote_addr_;
			ACE_INET_Addr local_addr_;
			ACE_SOCK_Dgram socket_;
			char *data_buff;
			const unsigned short dgram_byte_size;

			int accept_data(const std::string msg_sent);
		};
}

namespace IPC_Client_Broadcast
{
	class Client //: public ACE_Event_Handler
	{
		public:
			Client(const unsigned short remote_port);
			Client (const unsigned short remote_port, const unsigned short data_buffer_size, const unsigned short dgram_byte_size);
			//virtual ~Client();
			~Client();

			int send_data(std::string msg);


		private:
			ACE_INET_Addr remote_addr_;
			ACE_INET_Addr local_addr_;
			ACE_SOCK_Dgram_Bcast socket_;
			char *data_buff;
			const unsigned short dgram_byte_size;
			const unsigned short remote_port;
			const std::string data_msg_end="€"; //this works as c++11 defines.. where's the smart pointers????
			const std::string data_msg_start="$";
			int found_msg_end=0;
			int found_msg_start=0;

			int accept_data(const std::string msg_sent);
			int open_socket();
		};
}

#endif /* IPC_CLIENT_H_ */
