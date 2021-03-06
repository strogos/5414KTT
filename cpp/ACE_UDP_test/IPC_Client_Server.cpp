/*
 * InterProcCom.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: bengteh
 */
#pragma once

#include "IPC_Client_Server.h"
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

int Client::accept_data()
{
	if(this->socket_.recv(this->data_buff,sizeof(data_buff),this->remote_addr_)!=-1)
	{
		ACE_DEBUG((LM_DEBUG, "Data received from remote server %s : %s\n" ,
						this->remote_addr_.get_host_name(), this->data_buff));
		return -1;
	}
	else
		return 0;
}

int Client::send_data()
{
	ACE_DEBUG((LM_DEBUG,"Preparing to send data to server %s:%d\n",
			this->remote_addr_.get_host_name(),this->remote_addr_.get_port_number()));
	ACE_OS::sprintf(this->data_buff,"Heil from client\n");

	while(this->socket_.send(this->data_buff,ACE_OS::strlen(this->data_buff),this->remote_addr_)!=-1)
	{
		ACE_OS::sleep(1);
		if(this->accept_data()==-1)
			break;
	}

	return -1;
}
Server::Server(unsigned short local_port)
	:local_addr_(local_port),socket_(local_addr_),dgram_byte_size(32)
{
	this->data_buff=new char[4096];//4096=default data buffer size

}

Server::Server(unsigned short local_port,unsigned short data_buffer_size,const unsigned short dgram_byte_size)
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

Receiver::Receiver(ACE_Proactor* actor, const  char* ip_port_str)
					:ACE_Handler(actor), addr_(ip_port_str), read_msg_(4096)
{
	this->socket_.open(addr_);
	this->recv_();
	this->asynch_read_dgram_.open(*this,this->socket_.get_handle(),0,this->proactor());
}
Receiver::~Receiver()
{
	this->asynch_read_dgram_.cancel();
	this->socket_.close();
}
int Receiver::recv_()
{
	this->read_msg_.reset();
    size_t sendsize(0);
	return  this->asynch_read_dgram_.recv(&this->read_msg_,sendsize,0, PF_INET);
}
void Receiver::handle_read_dgram(const ACE_Asynch_Read_Dgram::Result &result)
{
	 if(!result.success())
	 {
		 this->recv_();
		 std::cout<<"Receiver fail:"<<result.error()<<std::endl;
		 return;
	 }

	 std::cout<<"Receiver Data:"<<(const char*)(result.message_block()->rd_ptr())<<std::endl;
	 this->recv_();
}


Sender::Sender(ACE_Proactor* actor, const  char* ipstr)
				:ACE_Handler(actor),addr_(ipstr)
{
	this->socket_.open( ACE_Addr::sap_any);
	this->write_require_.open(*this, this->socket_.get_handle(),0, this->proactor());
}
Sender::~Sender(void)
{
	this->write_require_.cancel();
	this->socket_.close();
}
int Sender::send(const  char* data)
{
	size_t len = ACE_OS::strlen(data)+1;

	ACE_Message_Block* m;
	ACE_NEW_RETURN(m, ACE_Message_Block(len),-1);
	m->copy(data);

	size_t sendsize(0);
	int ret = this->write_require_.send( m, sendsize,0, this->addr_);
	if( 0!=ret && 1!=ret)
		m->release();

   return ret;
}
void Sender::handle_write_dgram (const ACE_Asynch_Write_Dgram::Result &result){
	result.message_block()->release();
	if(!result.success())
	{
		std::cout<<std::endl<<"Sender fail:"<<result.error();
		return;
	}
}


} //ICP_Client_Server namespace



