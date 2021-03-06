/*
 * ICP_Client.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: sveigri
 */

#ifndef ICP_CLIENT_CPP_
#define ICP_CLIENT_CPP_

#include "ICP_Client.h"
#include <iostream>

namespace IPC_Client_Server
{
	Client::Client (const char * remote_host_and_port)
		:remote_addr_(remote_host_and_port),local_addr_((u_short)0),socket_(local_addr_)
		{
			this->data_buff = new char[4096];
		}

	Client::Client (const char * remote_host_and_port, unsigned short data_buffer_size)
		:remote_addr_(remote_host_and_port),local_addr_((u_short)0),socket_(local_addr_)
	{
		this->data_buff = new char[data_buffer_size];
	}


	Client::~Client()
	{
		delete this->data_buff;
		socket_.close();
	}
}

#endif /* ICP_CLIENT_CPP_ */
