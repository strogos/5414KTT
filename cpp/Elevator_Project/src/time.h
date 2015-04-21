/*
 * time.h
 *
 *  Created on: Apr 22, 2015
 *      Author: bengteh
 */

#ifndef TIME_H_
#define TIME_H_

#include <chrono>
#include <cstdint>

namespace Elevator
{
	using time_m_seconds = int64_t;

	static inline time_m_seconds now()
	{
		return std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::steady_clock::now().time_since_epoch()).count();
	}

	static inline std::chrono::milliseconds toSystemTime(time_m_seconds mtime)
	{
		return std::chrono::milliseconds(mtime);
	}

	template<class Rep, class Period>
	static time_m_seconds fromSystemTime(const std::chrono::duration< Rep, Period >& d)
	{
		return std::chrono::duration_cast< std::chrono::milliseconds >(d).count();
	}
}
#endif /* TIME_H_ */
