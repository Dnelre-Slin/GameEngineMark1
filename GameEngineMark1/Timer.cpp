#include "Timer.h"
#include <iostream>
#include <limits>

GameTime::GameTime() {
	m_CurrentTimeStamp = UNIX_TIME_STAMP;
	m_DeltaTime = std::numeric_limits<float>::min(); // Just in case
}

int GameTime::QueueAction(float ms, Action action)
{
	m_CurrentTimeStamp = UNIX_TIME_STAMP;
	int index = (int)m_CurrentTimeStamp + (int)ms;
	std::cout << "TimeStamp: " << index << "  ms: " << ms << std::endl;
	m_Callbacks[index] = action;
	return ms;
}

bool GameTime::Tick()
{
	m_DeltaTime = UNIX_TIME_STAMP - m_CurrentTimeStamp;
	m_CurrentTimeStamp = UNIX_TIME_STAMP;
	for(auto it = m_Callbacks.cbegin(); it != m_Callbacks.cend();)
	{
		std::cout << "key: " << it->first << " time: " << m_CurrentTimeStamp << std::endl;
		if (it->first < m_CurrentTimeStamp)
		{
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