/*
 * ICP_Client.h
 *
 *  Created on: Feb 5, 2015
 *      Author: sveigri
 */

#ifndef ICP_CLIENT_H_
#define ICP_CLIENT_H_

#include "ace/Log_Msg.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

namespace IPC_Client
{
	class Client //: public ACE_Event_Handler
	{
		public:
			Client(const char * remote_host_and_port);
			Client (const char * remote_host_and_port, unsigned short data_buffer_size);
			//virtual ~Client();
			~Client();

			int accept_data();
			int send_data();


		private:
			ACE_INET_Addr remote_addr_;
			ACE_INET_Addr local_addr_;
			ACE_SOCK_Dgram socket_;
			char *data_buff;
		};

#endif /* ICP_CLIENT_H_ */
