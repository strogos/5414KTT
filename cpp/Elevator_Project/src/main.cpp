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
#pragma once

#include "Elevator_Object.h"
#include "Control.h"
#include "Timer.h"
#include <unistd.h>//sleep
#include <iostream>
#include "Driver.h"//A quick class wrapper based on elev.c;
#include <mutex>
#include "io.h"

using namespace std;


int test_driver();
void signal_int_timer_test(int);
void signal_oneshot_timer_test(int);

int main()
{
	ACE_DEBUG((LM_DEBUG,
					   "in main\n"));
	//connect signals to slots
	signal(SIG_INTERVAL_TIMER,signal_int_timer_test);
	signal(SIG_ONESHOT_TIMER,signal_oneshot_timer_test);

	std::stringstream ss;
	State test;
	test.do_serialize(ss,test);
	std::cout<<"data in binary form: "<<ss.str()<<"\n";
	test.floor=1; test.direction=0;
	test.do_deserialize(ss,test);
	std::cout<<"data in converted back to normal form now?????\n";

		elevator::Control ctrl(elevator::tag_elevator_type::SIMULATION,ss);
	//	elevator::Control ctrl(elevator::tag_elevator_type::COMEDI,ss);


	//Timer tmr_test(Timer_Type::INTERVAL,2000);
	//elevator::Timer tm(elevator::Timer_Type::INTERVAL);

//	Timer tmr_test2(Timer_Type::INTERVAL,1000);
//	usleep(10000000);
//	tmr_test2.stop();
	//Wait for all the tasks to exit.
	ACE_Thread_Manager::instance()->wait();

	return 0;
}

int test_driver()
{
	elevator::Driver drv(1,4);

	 // Initialize hardware
	    if (!drv.init(elevator::SIMULATION))
	    {
	        cout<<"Unable to initialize elevator hardware!\n";
	        return 1;
	    }

	    cout<<"Press STOP button to stop elevator and exit program.\n";

	    drv.set_motor_direction(elevator::DIRN_UP);

	    while (1) {
	        // Change direction when we reach top/bottom floor
	        if (drv.get_floor_sensor_signal() == drv.get_max_floor() - 1)
	        {
	            drv.set_motor_direction(elevator::DIRN_DOWN);
	        } else if (drv.get_floor_sensor_signal() == 0)
	        {
	            drv.set_motor_direction(elevator::DIRN_UP);
	        }

	        // Stop elevator and exit program if the stop button is pressed
	        if (drv.get_stop_signal())
	        {
	            drv.set_motor_direction(elevator::DIRN_STOP);
	            break;
	        }
	        usleep(1000000); std::cout<<"counting sec\n";
	    }

	    cout<<"DONE!\n";

	    return 0;
}

std::mutex g_mutex;
int county=0;
void signal_int_timer_test(int signum)
{
	std::unique_lock<std::mutex> lock(g_mutex);
	county++;
	ACE_DEBUG((LM_DEBUG,
						   "timer interrupt no %d\n",
						   county));
}
void signal_oneshot_timer_test(int signum){std::cout<<"timer one shot\n";}

