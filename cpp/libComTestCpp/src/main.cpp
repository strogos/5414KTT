//============================================================================
// Name        : libComTestCpp.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

extern "C"
{
	#include"elev.h"
}

#include <unistd.h>//sleep

#include <iostream>
using namespace std;


int main()
{

	 // Initialize hardware
	    if (!elev_init()) {
	        cout<<"Unable to initialize elevator hardware!\n";
	        return 1;
	    }

	    cout<<"Press STOP button to stop elevator and exit program.\n";

	    elev_set_motor_direction(DIRN_UP);

	    while (1) {
	        // Change direction when we reach top/bottom floor
	        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
	            elev_set_motor_direction(DIRN_DOWN);
	        } else if (elev_get_floor_sensor_signal() == 0) {
	            elev_set_motor_direction(DIRN_UP);
	        }

	        // Stop elevator and exit program if the stop button is pressed
	        if (elev_get_stop_signal()) {
	            elev_set_motor_direction(DIRN_STOP);
	            break;
	        }
	        usleep(1000);
	    }
	    cout<<"DONE!\n";

	return 0;
}
