/*
 * State.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: bengteh
 */

#include "State.h"

namespace elevator
{
	std::stringstream& State::do_serialize(std::stringstream &ss,State &state)
	{

		state.set_serialize();
		state.manage_call_containers();
		state.manage_remote_containers();

		//data_= std::unique_ptr<std::stringstream>(new std::stringstream);
		//do the actual serialization
		{
		//	cereal::BinaryOutputArchive oarchive(*state.data_);
			cereal::BinaryOutputArchive oarchive(ss);
			oarchive(state);
		}
		//data_=&ss;

		return ss;
	}

	bool State::do_deserialize(std::stringstream &ss, State &state)
	{
		state.set_deserialize();
		//do the actual deserialization
		std::string tmp= ss.str();
		data_<<tmp;
		{
			cereal::BinaryInputArchive iarchive(data_);
			iarchive(state);
		}

		state.manage_call_containers();
		state.manage_remote_containers();

		return true;
	}

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

	void State::manage_remote_containers()//Command_Button_State &int_state)
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
				remote[remote_container_frst[j]].call[j]=remote_container_sec[j];
				//remote[it->first].call[j]=remote_container_sec[j];
				j++;
			}
		}
	}

	void State::set_serialize(){serialize_=true;}
	void State::set_deserialize(){serialize_=false;}
}
