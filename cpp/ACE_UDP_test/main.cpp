/*
 * main.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: bengteh
 */


#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/SOCK_Dgram.h"

#include <iostream>
using namespace std;

int send_broadcast(unsigned short port,char *msg_to_send)
{
	ACE_INET_Addr my_addr(static_cast<unsigned short>(10101));
	ACE_SOCK_Dgram_Bcast udp(my_addr);
	ssize_t sent=udp.send((const char*)msg_to_send,ACE_OS_String::strlen(msg_to_send)+1,port);
	udp.close();
	if(sent==-1)
	{
		ACE_ERROR_RETURN((LM_ERROR,
									ACE_TEXT("%p\n"),
									ACE_TEXT("send")),-1);
	}
	return 0;
}

int send_unicast(char *address_and_port,char *msg_to_send)
{
	const ACE_INET_Addr addr(address_and_port);
	ACE_INET_Addr my_addr(static_cast<unsigned short>(10101));
	ACE_SOCK_Dgram udp(my_addr);
	ssize_t sent=udp.send((const char*)msg_to_send,ACE_OS_String::strlen(msg_to_send)+1,addr);
	udp.close();
		if(sent==-1)
		{
			ACE_ERROR_RETURN((LM_ERROR,
										ACE_TEXT("%p\n"),
										ACE_TEXT("send")),-1);
		}

	return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
	//send_broadcast(43210,"I hate myself");
	send_unicast("129.241.187.136:20004","suckie dickie");

	return 0;
}