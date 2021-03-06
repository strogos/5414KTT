/*
 * InterProcCom.h
 *
 *  Created on: Jan 29, 2015
 *      Author: bengteh
 */
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Reactor.h" //ACE_Event_Handler TO BE IMPL-
#include  <ace/Proactor.h>//ACE_Event_Handler TO Be IMPL-
#include  <ace/Asynch_IO.h>
#include  <ace/Message_Block.h>


namespace IPC_Client_Server
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

class Server //: public ACE_Event_Handler
{
public:
	Server(const unsigned short local_port);
	Server(const unsigned short local_port, const unsigned short data_buffer_size, const unsigned short dgram_byte_size);
	//virtual ~Server();
	~Server();

	int accept_data();
	int send_data();

private:
	ACE_INET_Addr remote_addr_;
	ACE_INET_Addr local_addr_;
	ACE_SOCK_Dgram socket_;
	char *data_buff;
	const unsigned short dgram_byte_size;

};

class Receiver : public ACE_Handler
{
public:
	Receiver(ACE_Proactor* actor, const char* ip_port_str);
	virtual ~Receiver();

private:
	//overrides
	virtual void handle_read_dgram (const ACE_Asynch_Read_Dgram::Result &result);

	ACE_Asynch_Read_Dgram asynch_read_dgram_;
    ACE_SOCK_Dgram socket_;
    ACE_INET_Addr addr_;
    ACE_Message_Block read_msg_;

   int recv_();
};

class Sender : public ACE_Handler
{
public:
    Sender(ACE_Proactor* actor, const  char* ipstr);
    virtual ~Sender();

	int send(const  char* data);
private:
	virtual  void handle_write_dgram (const ACE_Asynch_Write_Dgram::Result &result);

    ACE_Asynch_Write_Dgram write_require_;
    ACE_SOCK_Dgram socket_;
    ACE_INET_Addr addr_;

};

}// namespace IPC_Client_Server
