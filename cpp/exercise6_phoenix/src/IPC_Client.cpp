/*
 * ICP_Client.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: sveigri
 */

#ifndef IPC_CLIENT_CPP_
#define IPC_CLIENT_CPP_

#include "IPC_Client.h"

#include <sstream>
#include <memory>

namespace IPC_Client_Unicast
{
	Client::Client (const char * remote_host_and_port)
		:remote_addr_(remote_host_and_port),local_addr_((u_short)0),socket_(local_addr_),dgram_byte_size_(32)
		{
			this->data_buff_ = new char[4096];
		}

	Client::Client (const char * remote_host_and_port, const unsigned short data_buffer_size, const unsigned short dgram_byte_size)
		:remote_addr_(remote_host_and_port),local_addr_((u_short)0),socket_(local_addr_),dgram_byte_size_(dgram_byte_size)
	{
		this->data_buff_ = new char[data_buffer_size];
	}

	Client::~Client()
	{
		delete [] this->data_buff_;
		socket_.close();
	}

	int Client::accept_data_(const std::string msg_sent)
	{
		std::string data_string=msg_sent;
		//ssize_t bytes_received=0;

		ACE_Time_Value timout(3,0);/*wait for 3 seconds wait for acknowledge from server*/
		if(socket_.recv(this->data_buff_,this->dgram_byte_size_,this->remote_addr_,0,&timout)!=-1)
		{
			data_string.assign(data_buff_,ACE_OS::strlen (this->data_buff_));

			found_msg_start_=data_string.find(data_msg_start_);
			found_msg_end_=data_string.find(data_msg_end_);
			if ((found_msg_start_>-1) && (found_msg_end_>-1))/*is message valid?*/
			{
				ACE_DEBUG((LM_DEBUG,
						"Data received from remote server %s was: %s \n" ,
						remote_addr_.get_host_addr(), this->data_buff_));
				return 0;
			}
			else
			{
				ACE_ERROR ((LM_WARNING,"%p\n","did not rcv ACK"));
				return -1;
			}
		}
		else
		{
			ACE_ERROR ((LM_ERROR,"%p\n","Client recv"));
			return -1;
		}
	}

	int Client::send_data(std::string msg)
	{
		/*add message integrity markers to message*/
		std::stringstream *ss = new std::stringstream;
		*ss<<"$"<<msg<<"€";
		msg=ss->str();

		delete ss;
		ACE_DEBUG((LM_DEBUG,"Preparing to send data to server %s:%d\n",
				this->remote_addr_.get_host_addr(),this->remote_addr_.get_port_number()));

//		/*Send message to server*/
//		while(this->socket_.send(msg.c_str(),ACE_OS::strlen(msg.c_str()),this->remote_addr_)!=-1)
//		{
//			ACE_OS::sleep(1);
//			if(this->accept_data_(msg)==-1)
//				break;
//		}

		/*Send msg to server; re-send msg if server does not acknowledge that msg was received*/
		for (int resend_count=3; resend_count>0; resend_count--)
		{
			if (socket_.send (msg.c_str(), ACE_OS::strlen (msg.c_str()) + 1, this->remote_addr_) == -1)
							ACE_ERROR_RETURN ((LM_ERROR,
														"%p\n",
														"send"),
														-1);
			if (this->accept_data_(msg)!=-1)
				break;

			if(resend_count==1)
			{
				ACE_ERROR_RETURN ((LM_ERROR,
										   "%p\n",
										   "Server not responding to hail from client"),
											-1);
			}
		}

		return 0;
	}
}

namespace IPC_Client_Broadcast
{
	Client::Client (const unsigned short remote_port)
		:remote_addr_(static_cast<unsigned short>(10101)),local_addr_((u_short)0),socket_(local_addr_),dgram_byte_size_(32),remote_port_(remote_port)
		{
			this->data_buff_ = new char[4096];
		}

	Client::Client (const unsigned short remote_port, const unsigned short data_buffer_size, const unsigned short dgram_byte_size)
		:remote_addr_(static_cast<unsigned short>(10101)),local_addr_((u_short)0),socket_(local_addr_),dgram_byte_size_(dgram_byte_size),remote_port_(remote_port)
	{
		this->data_buff_ = new char[data_buffer_size];
	}

	Client::~Client()
	{
		delete this->data_buff_;
		socket_.close();
	}

	int Client::accept_data_(const std::string msg_sent)
		{
			std::string data_string=msg_sent;
			//ssize_t bytes_received=0;

			ACE_Time_Value timout(3,0);
			if(socket_.recv(this->data_buff_,this->dgram_byte_size_,this->remote_addr_,0,&timout)!=-1)
			{
				data_string.assign(data_buff_,ACE_OS::strlen (this->data_buff_));

				found_msg_start_=data_string.find(data_msg_start_);
				found_msg_end_=data_string.find(data_msg_end_);
				if ((found_msg_start_>-1) && (found_msg_end_>-1))/*is message valid?*/
				{
					ACE_DEBUG((LM_DEBUG,
							"Client rcv data from remote server %s; DATA_MSG: %s \n" ,
							remote_addr_.get_host_addr(), this->data_buff_));
					return 0;
				}
				else
				{
					ACE_ERROR ((LM_WARNING,"%p\n","did not rcv ACK"));
					return -1;
				}
			}
			else
			{
				ACE_ERROR ((LM_ERROR,"%p\n","Client recv"));
				return -1;
			}
		}

	int Client::send_data(std::string msg)
	{
		/*add message integrity markers to message*/
		std::unique_ptr<std::stringstream> ss(new std::stringstream);
		//std::stringstream *ss = new std::stringstream;
		*ss<<"$"<<msg<<"€";
		msg=ss->str();
		//delete ss;

		ACE_DEBUG((LM_DEBUG,"Client preparing to broadcast data over port %hu\n",
				this->remote_port_));

		/*Send msg to server; re-send msg if server does not acknowledge that msg was received*/
		for (int resend_count=3; resend_count>0; resend_count--)
		{
			if (socket_.send (msg.c_str(), ACE_OS::strlen (msg.c_str()) + 1, this->remote_port_) == -1)
				ACE_ERROR_RETURN ((LM_ERROR,
											"%p\n",
											"send"),
											-1);
			if (this->accept_data_(msg)!=-1)
				break;

			if(resend_count==1)
			{
				ACE_ERROR_RETURN ((LM_ERROR,
										   "%p\n",
										   "Server not responding to hail from client"),
											-1);

				/*TODO: QUESTION SERVER INTEGRITY HERE! (...or maybe in main?)*/
					//->Wait for I am alive? then try re-sending?
			}
		}

		return 0;
	}
}
#endif /* IPC_CLIENT_CPP_ */
