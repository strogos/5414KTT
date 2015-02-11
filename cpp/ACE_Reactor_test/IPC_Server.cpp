/*
 * IPC_Client_Server.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: sveigri
 */


#ifndef ICP_SERVER_CPP_
#define ICP_SERVER_CPP_

#include "IPC_Server.h"

#include <iostream>

namespace IPC_Server
{
	Server::Server(const unsigned short local_port)
		:local_addr_(local_port),socket_(local_addr_),dgram_byte_size(32)
	{
		this->data_buff=new char[4096];//4096=default data buffer size

	}

	Server::Server(const unsigned short local_port,unsigned short data_buffer_size,const unsigned short dgram_byte_size)
		:local_addr_(local_port),socket_(local_addr_),dgram_byte_size(dgram_byte_size)
	{
		this->data_buff=new char[data_buffer_size];

	}

	Server::~Server()
	{
		delete this->data_buff;
		this->socket_.close();
	}

	int Server::accept_data()
	{
		std::cout<<"...listening on host: "<<this->remote_addr_.get_host_addr()<<std::endl;
		ssize_t bytes_received=0;
		while((bytes_received = this->socket_.recv(this->data_buff,this->dgram_byte_size,this->remote_addr_))!=-1)
		{
			data_buff[bytes_received]=0;
			std::cout<<bytes_received<<" bytes recv\n";
			ACE_DEBUG((LM_DEBUG, "Data received from client %s was %s \n",
						this->remote_addr_.get_host_addr(), this->data_buff));
			ACE_OS::sleep(1);

			if(this->send_data()==-1)
				break;
		}
		return 0;
	}

	int Server::send_data()
	{
		ACE_DEBUG((LM_DEBUG,"Preparing to send reply to client %s:%d\n",
					this->remote_addr_.get_host_addr(),this->remote_addr_.get_port_number()));
		ACE_OS::sprintf(this->data_buff,"Msg acknowledged by server");
		if(this->socket_.send(this->data_buff, ACE_OS::strlen(data_buff)+1,this->remote_addr_)==-1)
			return -1;
		else
			return 0;
	}
}
#endif /* ICP_SERVER_CPP_ */