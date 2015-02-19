//============================================================================
// Name        : IPC_Server.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice--> If you wanna copy this: you don't recognize shit when you see it...
// Description : ACE IPC wrapper
//============================================================================

#ifndef IPC_SERVER_CPP_
#define IPC_SERVER_CPP_

#include "IPC_Server.h"

#include <iostream>

#include <cstring>

namespace IPC_Server
{
	Server::Server(const unsigned short local_port)
		:local_addr_(local_port),socket_(local_addr_),dgram_byte_size_(32)
	{
		this->data_buff_=new char[4096];/*Set 4096=default max data buffer size*/

	}

	Server::Server(const unsigned short local_port,unsigned short data_buffer_size,const unsigned short dgram_byte_size)
		:local_addr_(local_port),socket_(local_addr_),dgram_byte_size_(dgram_byte_size)
	{
		this->data_buff_=new char[data_buffer_size];

	}

	Server::~Server()
	{
		delete [] this->data_buff_;
		this->socket_.close();
	}

	std::string Server::get_data()
	{
		return this->data_string_;
	}

	int Server::accept_data()
	{
		int found_msg_end=0;
		int found_msg_start=0;
		ssize_t bytes_received=0;

		std::memset(this->data_buff_,0,ACE_OS::strlen(this->data_buff_));//Clean data buffer

		std::cout<<"...listening on host: "<<this->remote_addr_.get_host_addr()<<std::endl;
		if((bytes_received = this->socket_.recv(this->data_buff_,this->dgram_byte_size_,this->remote_addr_))!=-1)//!=-1)
		{
			data_buff_[bytes_received]=0;
			ACE_DEBUG((LM_DEBUG, "Server received %d bytes from client %s; DATA_MSG was: %s \n",
					bytes_received, this->remote_addr_.get_host_addr(), this->data_buff_));
			#if ACE_NDEBUG == 0
				ACE_OS::sleep(1);
			#endif
			data_string_.assign(this->data_buff_,ACE_OS::strlen (this->data_buff_));

			found_msg_start=data_string_.find(this->data_msg_start_);
			found_msg_end=data_string_.find(this->data_msg_end_);
			if ((found_msg_start>-1) && (found_msg_end>-1))//is message valid?
			{
				ACE_DEBUG((LM_DEBUG,"Server got a complete message!\n"));
				if(this->send_data()==-1)
					return 1;//break;
			}
		}
		return 1;
	}

	int Server::send_data()
	{
		ACE_DEBUG((LM_DEBUG,"Preparing to send reply to client %s:%d\n",
					this->remote_addr_.get_host_addr(),this->remote_addr_.get_port_number()));

		if(this->socket_.send(this->data_buff_, ACE_OS::strlen(data_buff_)+1,this->remote_addr_)==-1)
		{
			ACE_ERROR_RETURN ((LM_ERROR,
							   "%p\n",
							   "send"),
							  -1);
		}
		else
		{
			ACE_DEBUG((LM_DEBUG,"Server succ sent ack to client!\n"));
			return 0;
		}
	}
}
#endif /* IPC_SERVER_CPP_ */
