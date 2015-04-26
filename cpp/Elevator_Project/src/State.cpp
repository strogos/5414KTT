/*
 * State.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: bengteh
 */

#include "State.h"

namespace elevator
{


	void State::manage_call_containers()
	{
		if (serialize_)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					call_container_frst[i][j]=call[i][j].first;
					call_container_sec[i][j]=call[i][j].second;
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
						for (int j = 0; j < 4; j++)
						{
							call[i][j].first=call_container_frst[i][j];
							call[i][j].second=call_container_sec[i][j];
						}
			}
		}
	}

	void State::manage_remote_containers(Command_Button_State &int_state)
	{
		int j=0;
		std::map <uint32_t, Command_Button_State>::iterator it;
		if (serialize_)
		{
			for (it = remote.begin(); it != remote.end(); ++it)
			{
				remote_container_frst[j]=it->first;
				remote_container_sec[j]=it->second.call[j];
				j++;
			}
		}
		else
		{
			for (it = remote.begin(); it != remote.end(); ++it)
			{
				//remote[it->first]=remote_container_frst[j];
				remote[it->first].call[j]=remote_container_sec[j];
				j++;
			}
		}
	}

	void State::set_serialize(){serialize_=true;}
	void State::set_deserialize(){serialize_=false;}


	//struct State_Scheduler


}
