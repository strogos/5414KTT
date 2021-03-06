/*
 * main.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: strogos
 */

#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h" /*for ACE_ERROR_RETURN and messaging*/
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
//#include "ace/"

#include <iostream>
using namespace std;


int tstTCP_client();
int tstTCP_srvr();


int main(int argc, char *argv[])
{
	const char *pathname =
					argc > 1 ? argv[1] : "index.html";
	const char *server_hostname =
					argc > 2 ? argv[2] : "ace.ece.uci.edu";

	tstTCP_client();
	//tstTCP_srvr();


	return 0;
}

int tstTCP_client()
{


		ACE_INET_Addr srvr("129.241.187.136:34933");
		ACE_SOCK_Connector connector;
		ACE_Time_Value timeout(10);//timout: 10uS
		ACE_SOCK_Stream peer;

		if (connector.connect(peer,srvr,&timeout)==-1)
		{
			if (ACE_OS::last_error()==ETIME)
			{
				ACE_DEBUG((LM_DEBUG,
						ACE_TEXT("(%P|%t) Timout while "),
						ACE_TEXT("connecting to server\n")));
			}
			else
			{
				ACE_ERROR((LM_ERROR,
						ACE_TEXT("%p\n"),
						ACE_TEXT("peer")));
			}

			return (101);
		}
		peer.send_n("uptime\0\n",9);



		char buff[64];
		int bc = peer.recv(buff,sizeof(buff));

		cout<<"output bc: "<<bc<<endl;
		cout<<"output buff: "<<buff<<"\n";

		return 0;
}

int tstTCP_srvr()
{
	ACE_INET_Addr port_to_listen("129.241.187.147:31234");//"HAStatus",31234);
	ACE_SOCK_Acceptor acceptor;
	if (acceptor.open(port_to_listen,1)==-1)
	{
		ACE_ERROR_RETURN((LM_ERROR,
							ACE_TEXT("%p\n"),
							ACE_TEXT("acceptor.open")),100);
	}

	while(true)
	{
		ACE_SOCK_Stream peer;
		ACE_INET_Addr peer_address;
		ACE_Time_Value timeout(10,0);

#if 0
		if (acceptor.accept(peer)==-1)
		{
			ACE_ERROR_RETURN((LM_ERROR,
										ACE_TEXT("%P|%t failed to accept"),
										ACE_TEXT("client connection\n")),100);
		}
#endif
		if (acceptor.accept(peer,&peer_address,&timeout,0)==-1)
		{
			if (ACE_OS::last_error()==EINTR)
			{
				ACE_DEBUG((LM_DEBUG,
						ACE_TEXT("(%P|%t) Interrupted while "),
						ACE_TEXT("connecting to server\n")));
			}
			else
			{
				if (ACE_OS::last_error()==ETIMEDOUT)
				ACE_DEBUG((LM_DEBUG,
						ACE_TEXT("(%P|%t) timout while "),
						ACE_TEXT("waiting for connection\n")));
			}
		}
		else
		{
			ACE_TCHAR peer_name[MAXHOSTNAMELEN];
			peer_address.addr_to_string(peer_name,MAXHOSTNAMELEN);
			ACE_DEBUG((LM_DEBUG,
						ACE_TEXT("(%P|%t) connection from %s \n"),
						peer_name));

			char buffer[4086];
			ssize_t bytes_received;


			while ((bytes_received=peer.recv(buffer,sizeof(buffer)))!=-1)
			{
				peer.send_n(buffer,bytes_received);

			}
			peer.close();
		}
	}

	return 0;
}
