/*
 * port.h
 *
 *  Created on: Jan 28, 2015
 *      Author: strogos
 */

#ifndef PORT_H_
#define PORT_H_

/* SERVIVE_PORT defines the default port number for this service
 * replace the number with a unique number > 1024
 * a reasonable number may be obtained from, say, four
 * digits of your id + 1024
 */

#define SERVICE_PORT	21234	/* hard-coded port number */
#undef UDP_SEND //TEST!!
#define UDP_RECV //TEST!!

#endif /* PORT_H_ */
