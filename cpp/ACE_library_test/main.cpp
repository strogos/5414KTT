/*
 * main.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: strogos
 */

//#include "ace/OS.h"

#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"

#include<iostream>
using namespace std;

int main(int argc, char *argv[])
{
	const char *pathname =
			argc > 1 ? argv[1] : "index.html";
	const char *server_hostname =
			argc > 2 ? argv[2] : "ace.ece.uci.edu";

	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;
	ACE_INET_Addr peer_addr;

	if (peer_addr.set (80, server_hostname) == -1)
		return 1;
	else if (connector.connect (peer, peer_addr) == -1)
		return 1;

	char buf[BUFSIZ];
	iovec iov [3];
	iov[0].iov_base = const_cast<char*>("GET");
	iov[0].iov_len = 4;// Length of "GET ".
	iov[1].iov_base = const_cast<char*>(pathname);
	iov[1].iov_len = strlen (pathname);
	iov[2].iov_base = const_cast<char*>(" HTTP/1.0\r\n\r\n");
	iov[2].iov_len = 13; // Length of " HTTP/1.0\r\n\r\n";

	if (peer.sendv_n (iov, 3) == -1)
		return 1;
	for (ssize_t n; (n = peer.recv (buf, sizeof buf)) > 0; )
		ACE::write_n (ACE_STDOUT, buf, n);

	return peer.close();

	cout <<"dfsgdfg"<<endl;

	return 0;
}
