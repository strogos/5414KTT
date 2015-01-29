///*
// * main.c
// *
// *  Created on: Jan 22, 2015
// *      Author: sveigri
// *
// *Tutorial:      https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html
// */
//
//#define PORT 20145
//#define BUFSIZE 2048
//#define BROADCAST "129.241.187.255"
//#define BROADCAST_ON 0
//#define HOST "10.0.0.5"// "78.91.48.183"//"itk-d666-ubuntu"
////#define
//
//#include<stdio.h>
//#include <sys/types.h>
//#include<sys/socket.h>
//#include<netinet/in.h>
////#include<error.h>
//#include <netdb.h>
//#include <string.h> /* for memcpy */
//
//void printIP(struct hostent *hp)
//{
//	int i;
//	unsigned char* a;
//	for (i=0; hp->h_addr_list[i] != 0; i++)
//	{
//		a=(unsigned char*) hp->h_addr_list[i];
//		printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
//	}
//}
//
//
//int CreateSocket(int *fd)
//{
//
//	if((*fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)
//	{
//		perror("Thread creation: " );
//		return -1;
//	}
//
//	int on=1;
//	if(setsockopt(*fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(int)))
//	{
//		perror("Create socket :");
//		return -1;
//	}
//	if (BROADCAST_ON)
//	{
//		if (setsockopt(*fd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(int)))
//		{
//			perror("Create socket :");
//			return -1;
//		}
//
//	}
//	return 0;
//
//}
//
//int CreateClient(int *fd)
//{
//
//	struct sockaddr_in clientAddr;
//	memset((char*)&clientAddr,0,sizeof(clientAddr));
//	clientAddr.sin_family = AF_INET;
//	clientAddr.sin_port = htons(PORT);//htons(0); //system picks port number
//	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY); //system picks IP addr
//
//	if(bind(fd,(struct sockaddr *)&clientAddr,sizeof(clientAddr)))
//	{
//		perror("Bind socket: ");
//		return -1;
//	}
//	return 0;
//}
//
//int CreateServer(int *fd, struct sockaddr_in *serverAddr, struct hostent *hp)
//{
////	struct hostent *hp; //host info
////	struct sockaddr_in serverAddr; //server address info
//
//	/*fill server address*/
//	memset((char*)serverAddr,0,sizeof(*serverAddr));
//	serverAddr->sin_family=AF_INET;
//	serverAddr->sin_port=htons(PORT);
//	/*Find server address when hostname is known*/
//	hp=gethostbyname((char*)HOST);
//	if (!hp)
//	{
//		fprintf(stderr, "could not find the IP to host %s\n", (char*)HOST);
//		return -1;
//	}
//	/*put host IP into the server address structure serverAddr*/
//	memcpy((void*)&serverAddr->sin_addr,hp->h_addr_list[0],hp->h_length);
//
//	return 0;
//}
//
//int SendMsg(int *fd,struct sockaddr_in *serverAddr, char *msg)
//{
//	/*send msg to server*/
//	char *my_msg="I am Client!";
//	if(sendto(fd,my_msg,strlen(my_msg),0,(struct sockaddr*)serverAddr,sizeof(*serverAddr)) < 0)
//	{
//		perror("msg send to server error");
//		return -1;
//	}
//	return 0;
//}
//
//int ReceiveMsg()
//{
//	return 0;
//}
//
//
//int main(int argc, char *argv[])
//{
//
//	int fd_send; 	//socket for sending
//	int fd_receive; //socket for receiving
//	struct hostent *hp; //host info
//	//struct sockaddr_in clientAddr; //client address info
//	struct sockaddr_in serverAddr; //server address info
//
//	CreateSocket(&fd_send);
//	CreateSocket(&fd_receive);
//	CreateClient(fd_send);
//	CreateServer(fd_receive,&serverAddr,hp);
//
//
//	printIP(hp);
//
//
//	/*Receive msg from server*/
//	int recvlen;
//	unsigned char buffer[BUFSIZE];
//
//	while(1)
//	{
//		printf("waiting on port %d\n", PORT);
//		recvlen=recvfrom(fd_receive,buffer,BUFSIZE,0,(struct socketaddr*)&serverAddr,sizeof(serverAddr));
//		printf("received %d bytes\n", recvlen);
//		if (recvlen>0)
//		{
//			buffer[recvlen]=0;
//			printf("received message: \"%s\"\n", buffer);
//			break;
//		}
//	}
//	/*END Receive msg from server*/
//
//	return 0;
//}
//
