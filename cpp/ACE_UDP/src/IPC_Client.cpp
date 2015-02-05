/*
 * ICP_Client.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: sveigri
 */

#ifndef IPC_CLIENT_CPP_
#define IPC_CLIENT_CPP_

#include <iostream>
#include <string>
#include "IPC_Client.h"


namespace IPC_Client_Unicast
{
	Client::Client (const char * remote_host_and_port)
		:remote_addr_(remote_host_and_port),local_addr_((u_short)0),socket_(local_addr_),dgram_byte_size(32)
		{
			this->data_buff = new char[4096];
		}

	Client::Client (const char * remote_host_and_port, const unsigned short data_buffer_size, const unsigned short dgram_byte_size)
		:remote_addr_(remote_host_and_port),local_addr_((u_short)0),socket_(local_addr_),dgram_byte_size(dgram_byte_size)
	{
		this->data_buff = new char[data_buffer_size];
	}

	Client::~Client()
	{
		delete this->data_buff;
		socket_.close();
	}

	int Client::accept_data()
	{
		ACE_Time_Value timout(3,0);
		if(socket_.recv(this->data_buff,this->dgram_byte_size,this->remote_addr_,0,&timout)!=-1)
		{
			ACE_DEBUG((LM_DEBUG,
					"Data received from remote server %s was: %s \n" ,
					remote_addr_.get_host_addr(), this->data_buff));

			return 0;
		}
		else
		{
			ACE_ERROR ((LM_ERROR,"%p\n","recv"));
			return -1;
		}
	}

	int Client::send_data()
	{
		ACE_DEBUG((LM_DEBUG,"Preparing to send data to server %s:%d\n",
				this->remote_addr_.get_host_addr(),this->remote_addr_.get_port_number()));
		ACE_OS::sprintf(this->data_buff,"Heil from unicast client\n");

		while(this->socket_.send(this->data_buff,ACE_OS::strlen(this->data_buff),this->remote_addr_)!=-1)
		{
			ACE_OS::sleep(1);
			if(this->accept_data()==-1)
				break;
		}

		this->accept_data();

		return -1;
	}
}

namespace IPC_Client_Broadcast
{
	Client::Client (const unsigned short remote_port)
		:remote_addr_(static_cast<unsigned short>(10101)),local_addr_((u_short)0),socket_(local_addr_),dgram_byte_size(32),remote_port(remote_port)
		{
			this->data_buff = new char[4096];
		}

	Client::Client (const unsigned short remote_port, const unsigned short data_buffer_size, const unsigned short dgram_byte_size)
		:remote_addr_(static_cast<unsigned short>(10101)),local_addr_((u_short)0),socket_(local_addr_),dgram_byte_size(dgram_byte_size),remote_port(remote_port)
	{
		this->data_buff = new char[data_buffer_size];
	}

	Client::~Client()
	{
		delete this->data_buff;
		socket_.close();
	}

	int Client::open_socket()
	{
		  if (this->socket_.open (local_addr_) == -1)
		    ACE_ERROR_RETURN ((LM_ERROR,
		                       "%p\n",
		                       "open"),
		                      0);
		  else
			  return 0;
	}

	int Client::accept_data()
		{
			ACE_Time_Value timout(3,0);
			if(socket_.recv(this->data_buff,this->dgram_byte_size,this->remote_addr_,0,&timout)!=-1)
			{

				ACE_DEBUG((LM_DEBUG,
						"Data received from remote server %s was: %s \n" ,
						remote_addr_.get_host_addr(), this->data_buff));

				return 0;
			}
			else
			{
				ACE_ERROR ((LM_ERROR,"%p\n","recv"));
				return -1;
			}
		}

	int Client::send_data()
	{
		ACE_DEBUG((LM_DEBUG,"Preparing to broadcast data over port %hu\n",
				this->remote_port));

		ACE_OS::sprintf(this->data_buff,"$Heil from broadcast client€\n");

		if (socket_.send (this->data_buff, ACE_OS::strlen (this->data_buff) + 1, this->remote_port) == -1)
		    ACE_ERROR_RETURN ((LM_ERROR,
		                       "%p\n",
		                       "send"),
		                      -1);

		this->accept_data();

		return -1;
	}
}
#endif /* IPC_CLIENT_CPP_ */
