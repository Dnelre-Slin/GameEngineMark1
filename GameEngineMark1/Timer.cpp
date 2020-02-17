#include "Timer.h"
#include <iostream>
#include <limits>

GameTime::GameTime() {
	m_CurrentTimeStampNano = Now();
	m_DeltaTime = std::numeric_limits<float>::min(); // Just in case
}

long long GameTime::QueueAction(long milliseconds, Action action)
{
	return QueueActionNS((long long) (milliseconds * 1e6), action);
}

long long GameTime::QueueActionNS(long long nanoseconds, Action action)
{
	m_CurrentTimeStampNano = Now();
	long long index = m_CurrentTimeStampNano + nanoseconds;
	std::cout << "TimeStamp: " << index << "  ns: " << nanoseconds << std::endl;
	m_Callbacks[index] = action;
	return nanoseconds;
}

bool GameTime::Tick()
{
	m_DeltaTime = Now() - m_CurrentTimeStampNano;
	m_CurrentTimeStampNano = Now();

	for(auto it = m_Callbacks.cbegin(); it != m_Callbacks.cend();)
	{
		if (it->first < m_CurrentTimeStampNano)
		{
			std::cout << "key: " << it->first << " time: " << m_CurrentTimeStampNano << std::endl;
			std::cout << "Invoking and removing at index " << it->first << std::endl;;
			if (it->second != nullptr)
			{
				it->second(); // Calling Action
			}
			m_Callbacks.erase(it++);
		}
		else
		{
			++it;
		}
	}

	return true;
}

long long GameTime::Now() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}