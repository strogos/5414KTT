/*
 * ICP_Client.h
 *
 *  Created on: Feb 5, 2015
 *      Author: sveigri
 */

#ifndef IPC_CLIENT_H_
#define IPC_CLIENT_H_

#include "ace/Log_Msg.h"
#include "ace/SOCK_Dgram.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/INET_Addr.h"

namespace IPC_Client_Unicast
{
	class Client //: public ACE_Event_Handler
	{
		public:
			Client(const char * remote_host_and_port);
			Client (const char * remote_host_and_port, const unsigned short data_buffer_size,const unsigned short dgram_byte_size);
			//virtual ~Client();
			~Client();

			int send_data();


		private:
			ACE_INET_Addr remote_addr_;
			ACE_INET_Addr local_addr_;
			ACE_SOCK_Dgram socket_;
			char *data_buff;
			const unsigned short dgram_byte_size;

			int accept_data();
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

			int send_data();


		private:
			ACE_INET_Addr remote_addr_;
			ACE_INET_Addr local_addr_;
			ACE_SOCK_Dgram_Bcast socket_;
			char *data_buff;
			const unsigned short dgram_byte_size;
			const unsigned short remote_port;

			int accept_data();
			int open_socket();
		};
}

#endif /* IPC_CLIENT_H_ */
