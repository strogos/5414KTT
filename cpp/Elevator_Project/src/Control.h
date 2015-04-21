/*
 * Control.h
 *
 *  Created on: Feb 11, 2015
 *      Author: bengteh
 */
#ifndef CONTROL_H_
#define CONTROL_H_

#include "Driver.h"

#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Message_Block.h>


namespace Elevator
{
	class Control
	{
		public:
			Control();
			~Control();
		private:
			Elevator::Driver *driver_;

			//*local network socket handlers
			//*elevator network handler



	};
}
#endif /* CONTROL_H_ */

