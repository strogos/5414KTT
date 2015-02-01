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
#include "ace/Reactor.h" //ACE_Event_Handler

class Client : public ACE_Event_Handler
{
public:
	Client(const ACE_INET_Addr &addr, const ACE_INET_Addr &remote_addr);
	virtual ~Client();

	// = Override <ACE_Event_Handler> methods.
	  virtual ACE_HANDLE get_handle (void) const;
	  virtual int handle_input (ACE_HANDLE);
	  virtual int handle_close (ACE_HANDLE handle,
								ACE_Reactor_Mask close_mask);
private:
  ACE_SOCK_Dgram endpoint_;	// To send messages and receive responses.
  ACE_INET_Addr remote_addr_; // The address to send messages to.

};

class Server : public ACE_Event_Handler
{
public:
	Server();
	virtual ~Server();
};
