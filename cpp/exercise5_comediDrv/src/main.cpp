//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//extern "C"
//{
//	#include"elev.h"
//}

#include <unistd.h>//sleep
#include "elevator_driver.h"
#include <iostream>
using namespace std;


int main()
{
	Elevator::Driver drv(1,4);

	 // Initialize hardware
	    if (!drv.init(ET_simulation))
	    {
	        cout<<"Unable to initialize elevator hardware!\n";
	        return 1;
	    }

	    cout<<"Press STOP button to stop elevator and exit program.\n";

	    drv.set_motor_direction(Elevator::DIRN_UP);

	    while (1) {
	        // Change direction when we reach top/bottom floor
	        if (drv.get_floor_sensor_signal() == drv.get_max_floor() - 1)
	        {
	            drv.set_motor_direction(Elevator::DIRN_DOWN);
	        } else if (drv.get_floor_sensor_signal() == 0)
	        {
	            drv.set_motor_direction(Elevator::DIRN_UP);
	        }

	        // Stop elevator and exit program if the stop button is pressed
	        if (drv.get_stop_signal())
	        {
	            drv.set_motor_direction(Elevator::DIRN_STOP);
	            break;
	        }
	        usleep(1000);
	    }
	    cout<<"DONE!\n";

	return 0;
}
