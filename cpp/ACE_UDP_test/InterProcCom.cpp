/*
 * InterProcCom.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: bengteh
 */

#include "InterProcCom.h"

Client::Client(const ACE_INET_Addr &addr, const ACE_INET_Addr &remote_addr) : endpoint_ (addr), remote_addr_ (remote_addr)
{
	// TODO Auto-generated constructor stub
}

Client::~Client()
{
	// TODO Auto-generated destructor stub
}

ACE_HANDLE Client::get_handle (void) const
{
  return endpoint_.get_handle ();
}

int Client::handle_input(ACE_HANDLE)
{
	/*Do we need it?*/
}

int Client::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  this->endpoint_.close ();
  return 0;
}





Server::Server()
{
	// TODO Auto-generated constructor stub
}

Server::~Server()
{
	// TODO Auto-generated destructor stub
}
