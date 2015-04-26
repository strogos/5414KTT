/*
 * Control.cpp
 *
 *  Created on: Feb 11, 2015
 *      Author: bengteh
 *
 *      Note:
 *      This is an elevator control module.
 */

#include "Control.h"
#include "Driver.h"
#include "Timer.h"
#include "Elevator.h"
#include "IPC_Client.h"
#include "IPC_Server.h"


#include <unistd.h>//sleep
#include <algorithm>
#include "ace/Log_Msg.h"
#include "ace/Auto_Ptr.h"

namespace elevator
{
	Control::Control(elevator_type session,std::stringstream& init_state)
	       : servicing_(false), session_(session)
	{
		ACE_DEBUG((LM_DEBUG,
						   "in control constructor .. state: %s \n",init_state.str().c_str()));

		/*handle elevator IO using the signal/slot principle
		 * (inspired by Qt's implementation)*/
		ctrl_signal_ = std::unique_ptr<Control_Signals>(new Control_Signals);
		ctrl_signal_->button_press.connect(this,&Control::slot_button_press);
		ctrl_signal_->floor_sensor.connect(this,&Control::slot_floor_sensor);
		ctrl_signal_->stop_task.connect(this,&Control::slot_exit_task);
		ctrl_signal_->interval_timer.connect(this,&Control::slot_heartbeat_timer);
		ctrl_signal_->oneshot_timer.connect(this,&Control::slot_service_timer);


		dgram_localhost_= std::unique_ptr<IPC_Client_Unicast::Client>(
										new IPC_Client_Unicast::Client("localhost:42024"));

		dgram_lan_= std::unique_ptr<IPC_Client_Broadcast::Client>(
												new IPC_Client_Broadcast::Client((ushort)421124));

		elevator_=std::unique_ptr<Elevator>(new Elevator(this));

		/*start tasks (and their corresponding threads)
		 * Note the order in which tasks are opened
		 * [memory leaks can happen here if not careful]*/
		elevator_->open(0);
		this->open(0);

//		/*Update Elevator State*/
		if (state_.do_deserialize(init_state,state_))
		 {
			for (int j = 0; j < N_FLOORS; j++)
			{
				if (state_.call[BUTTON_CALL_UP][j].second)
					elevator_->set_button_indicator(BUTTON_CALL_UP, true,j);
				if (state_.call[BUTTON_CALL_DOWN][j].second)
					elevator_->set_button_indicator(BUTTON_CALL_DOWN,true, j);
				if (state_.call[BUTTON_COMMAND][j].second)
					elevator_->set_button_indicator(BUTTON_COMMAND, true,j);
			}

			elevator_->set_direction((tag_motor_direction)state_.direction);
			elevator_->set_floor_indicator(state_.floor);
		 }


		/*init timers*/
		heartbeat_timer_=std::unique_ptr<Timer>(new Timer(Timer_Type::INTERVAL,
													100,
													this));
		heartbeat_timer_->open(0);

		service_timer_=std::unique_ptr<Timer>(new Timer(Timer_Type::ONE_SHOT,
					static_cast<long>(SERVICE_TIME_),
					this));

		service_timer_->open(0);
	}

	Control::~Control()
	{
		// TODO Auto-generated destructor stub
	}

	int Control::open(void*)
	{
		ACE_DEBUG((LM_DEBUG,
							"(%t) Active Control Object opened \n"));
		//activate object with a thread in it
		activate();
		return 0;
	}

	int Control::close(u_long)
	{
		ACE_DEBUG((LM_DEBUG, "(%t) Active Control Object is being closed down \n"));

		return 0;
	}

	int Control::svc(void)
	{
		ACE_Thread_Manager *mgr = this->thr_mgr ();

		while (true)
		{
			if (servicing_)
			{
				ACE_DEBUG((LM_DEBUG, "(%t) Control thread IS servicing : %d\n",(int)servicing_));
				servicing_=false;
			}

			std::unique_ptr<ACE_Method_Request>
			request (this->slot_queue_.dequeue ()); // Dequeue the next method object

			if (request->call () == -1)  // Invoke the method request
				break;

			if (mgr->testcancel (mgr->thr_self ())) //check if thread manager wants to stop
				break;
		}
		ACE_DEBUG((LM_DEBUG, "(%t) Control thread GOING DOWN\n"));
		return 0; //service done; ACE_Task will call elevator::close() and close off
				  //any threads in this object
	}

	elevator_type Control::get_session() {return session_;}

	Control_Signals * Control::signal_subscribe(Control_Signals * subscribe)
	{
		//memory mgmt on all signaling to slots in control shall be done in the control task itself
		subscribe=ctrl_signal_.get();
		if (subscribe)
			return subscribe;
		else
		{
			ACE_ERROR((LM_EMERGENCY,"PANIC: control task SIGNAL subscription FAIL!\n"));
			throw std::bad_alloc();
		}
	}

	class Control::On_Button_Press : public ACE_Method_Request
	{
		public:
			On_Button_Press(button_type_t button, int floor) : button_(button), floor_(floor)
			{
				ACE_DEBUG((LM_DEBUG,"Button type %d press ENQUED @ floor %d\n",
						(int)button,floor_));
			}
			virtual int call (void)
			{
				ACE_DEBUG((LM_DEBUG,"Button type pressed %d @ floor %d\n",
						(int)button_,floor_));
				return 0;
			}
		private:
			button_type_t button_;
			int floor_;
	};

	class Control::On_Floor_Sensor : public ACE_Method_Request
	{
		public:
			On_Floor_Sensor(int floor, Control * handle) : floor_(floor), handle_(handle)
			{
				ACE_DEBUG((LM_DEBUG,"on floor sensor enqued %d\n", floor));
			}
			virtual int call (void)
			{
				ACE_DEBUG((LM_DEBUG,"Reached floor %d\n", floor_));
				handle_->state_.floor=floor_;

			    // Check if the current floor should be serviced
			    if (handle_->should_service(floor_))
			    {
			        handle_->elevator_->stop();

			        // Service the floor
			        if (handle_->elevator_->get_direction() == DIRN_UP)
			        	handle_->service_floor(floor_,BUTTON_CALL_UP);
			        else
			        	handle_->service_floor(floor_,BUTTON_CALL_DOWN);
			    }
				return 0;
			}
		private:
			int floor_;
			Control * handle_;
	};

	struct Exit_Method : public ACE_Method_Request
	{
		virtual int call (void) {return -1;}
	};

	class Control::On_Service_Timer : public ACE_Method_Request
	{
		public:
			On_Service_Timer(Control *handle)
							: handle_(handle)
			{
				ACE_DEBUG((LM_DEBUG,"oneshot enqued %d\n", floor));
			}

			virtual int call (void)
			{
//				handle_->elevator_->set_door_open_indicator(false); //SIMULATOR IS CRAP

			    // Done servicing: more to do?
			    if (handle_->elevator_->get_direction() == DIRN_UP)
			    {
			        if (!handle_->is_call_up(handle_->elevator_->get_floor()))
			        	handle_->is_call_down(handle_->elevator_->get_floor());
			    }
			    else
			    {
			        if (!handle_->is_call_down(handle_->elevator_->get_floor()))
			        	handle_->is_call_up(handle_->elevator_->get_floor());
			    }
			    return 0;
			}

		private:
			Control *handle_=nullptr;
	};

	class Control::On_Heartbeat_Timer : public ACE_Method_Request
	{
		public:
			On_Heartbeat_Timer(Control * handle) : handle_(handle)
			{
			//	ACE_DEBUG((LM_DEBUG,"Interval enqued %d\n", floor));
			}

			virtual int call (void)
			{
				handle_->state_.direction = handle_->elevator_->get_direction();
				std::stringstream ss;
				handle_->state_.do_serialize(ss,handle_->state_);

			    // Send elevator state
			    handle_->dgram_localhost_->send_data(ss.str());

			    // Send elevator state to other elevators
			    handle_->dgram_lan_->send_data(ss.str());

				return 0;
			}

		private:
			Control * handle_;
	};

	bool Control::should_service(int floor)
	{
	    // Going up and there is a call up
	    if (elevator_->get_direction() == DIRN_UP && state_.call[BUTTON_CALL_UP][floor].second)
	        return true;

	    // Going down and there is a call down
	    if (elevator_->get_direction() == DIRN_DOWN && state_.call[BUTTON_CALL_DOWN][floor].second)
	        return true;

	    // Going up and there are no calls above, but there is call down
	    if (elevator_->get_direction() == DIRN_UP && !is_call_up(floor) && state_.call[BUTTON_CALL_DOWN][floor].second)
	    {
	        // change direction
	        elevator_->set_direction(DIRN_DOWN);
	        return true;
	    }

	    // Going down and there are not calls below, but there is call up
	    if (elevator_->get_direction() == DIRN_DOWN && !is_call_down(floor) && state_.call[BUTTON_CALL_UP][floor].second)
	    {
	        // change direction
	        elevator_->set_direction(DIRN_UP);
	        return true;
	    }

	    // There is an internal call
	    if (state_.call[BUTTON_COMMAND][floor].second)
	        return true;

	    // If there are no more requests, stop the elevator
	    if (elevator_->get_direction() == DIRN_DOWN && !is_call_down(floor) && !is_call_up(floor))
	        elevator_->stop();
	    else if (elevator_->get_direction() == DIRN_UP && !is_call_up(floor) && !is_call_down(floor))
	        elevator_->stop();

	    // default case is to not service
	    return false;
	}

	void Control::service_floor(int floor, button_type_t button)
	{
		/* Note: setting time stamp to +service time -
		 * button presses when servicing should be ignored*/

		// Internal calls are always serviced
		elevator_->set_button_indicator(BUTTON_COMMAND,false,floor);
		state_.call[BUTTON_COMMAND][floor].first = clock_time::now()+SERVICE_TIME_;
		state_.call[BUTTON_COMMAND][floor].second = false;

		// If there is an external call, service it!
		elevator_->set_button_indicator(button,false, floor);
		state_.call[button][floor].first = clock_time::now()+SERVICE_TIME_;
		state_.call[button][floor].second = false;

		state_.button_type = button;

		// Set the open door lamp
		elevator_->set_door_open_indicator(true);

		// Start the service timer
		service_timer_->restart();
	}

	bool Control::is_call_up(int floor)
	{
	    // check if there is a call up at the current floor and service it
	    if (state_.call[BUTTON_CALL_UP][floor].second)
	    {
	        service_floor(floor,BUTTON_CALL_UP);
	        return true;
	    }

	    // check calls going up
	    for (int i = floor+1; i < elevator_->get_floor_count(); i++)
	    {
	        if (state_.call[BUTTON_COMMAND][i].second || state_.call[BUTTON_CALL_UP][i].second)
	        {
	            elevator_->go_to_floor(i);
	            return true;
	        }
	    }

	    // check calls going down
	    for (int i = floor+1; i < elevator_->get_floor_count(); i++)
	    {
	        if (state_.call[BUTTON_COMMAND][i].second || state_.call[BUTTON_CALL_DOWN][i].second)
	        {
	            elevator_->go_to_floor(i);
	            return true;
	        }
	    }

	    return false;
	}

	bool Control::is_call_down(int floor)
	{
		//check if there is a call down at the current floor and service it
		if (state_.call[BUTTON_CALL_DOWN][floor].second)
		{
			service_floor(floor,BUTTON_CALL_DOWN);
			return true;
		}

		// check calls going down
		for (int i = floor-1; i >=0; i--)
		{
			if (state_.call[BUTTON_COMMAND][i].second || state_.call[BUTTON_CALL_DOWN][i].second)
			{
				elevator_->go_to_floor(i);
				return true;
			}
		}

		// check calls going up
		for (int i = floor-1; i >= 0; i--)
		{
			if (state_.call[BUTTON_COMMAND][i].second || state_.call[BUTTON_CALL_UP][i].second)
			{
				elevator_->go_to_floor(i);
				return true;
			}
		}

		return false;

	}

//	int Control::thread_prepare_join(std::vector<std::thread>& my_threads)
//	{
//	    std::for_each(my_threads.begin(),my_threads.end(),thread_do_join());
//
//	    return 1;
//	}
//
//	void Control::thread_do_join(std::thread& t){t.join();}
//
//	void thread_dgram_worker(int tid)
//	{
//			IPC_Server::Server srv(42000);
//			srv.accept_data();
//	}

	void Control::slot_button_press(button_type_t button, int floor)
	{
		ACE_DEBUG((LM_DEBUG, "in slot for button press\n"));
		slot_queue_.enqueue (new On_Button_Press(button,floor));
		servicing_=true;
	}

	void Control::slot_floor_sensor(int floor)
	{
		slot_queue_.enqueue (new On_Floor_Sensor(floor,this));
	}

	void Control::slot_exit_task(void*)
	{
		slot_queue_.enqueue(new Exit_Method);
	}
	void Control::slot_service_timer(void*)
	{
		slot_queue_.enqueue(new On_Service_Timer(this));//this->elevator_.get()));
	}
	void Control::slot_heartbeat_timer(void*)
	{
		slot_queue_.enqueue(new On_Heartbeat_Timer(this));
	}

}
