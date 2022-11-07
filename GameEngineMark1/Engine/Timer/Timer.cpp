#include "Timer.h"
#include <iostream>
#include <limits>

GameTime::GameTime() {
	m_CurrentTimeStampNano = Now();
	m_DeltaTime = std::numeric_limits<long long>::min(); // Just in case
}

long long GameTime::QueueAction(long milliseconds, const Action& action)
{
	return QueueActionNS(static_cast<long long>(milliseconds * 1e6), action);
}

long long GameTime::QueueActionNS(long long nanoseconds, const Action& action)
{
	m_CurrentTimeStampNano = Now();
	const long long index = m_CurrentTimeStampNano + nanoseconds;
	std::cout << "TimeStamp: " << index << "  ns: " << nanoseconds << std::endl;
	m_Callbacks[index] = action;
	return nanoseconds;
}

bool GameTime::Tick()
{
	m_DeltaTime = Now() - m_CurrentTimeStampNano;
	m_TimeElapsedSinceStart += m_DeltaTime;
	m_CurrentTimeStampNano = Now();

	for(auto It = m_Callbacks.cbegin(); It != m_Callbacks.cend();)
	{
		if (It->first < m_CurrentTimeStampNano)
		{
			std::cout << "key: " << It->first << " time: " << m_CurrentTimeStampNano << std::endl;
			std::cout << "Invoking and removing at index " << It->first << std::endl;;
			if (It->second != nullptr)
			{
				It->second(); // Calling Action
			}
			m_Callbacks.erase(It++);
		}
		else
		{
			++It;
		}
	}

	return true;
}

long long GameTime::Now() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
