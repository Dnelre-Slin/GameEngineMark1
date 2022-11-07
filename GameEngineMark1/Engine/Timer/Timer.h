#pragma once

#include <vector>
#include <map>
#include <functional>
#include <ctime>
#include <chrono>

typedef std::function<void(void)> Action;
typedef std::function<bool(int, int)> Cmp;

using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Nanoseconds = std::chrono::nanoseconds;

class GameTime {
public:
	GameTime();

	struct x {
		bool operator()(const int& a, const int& b) const {
			return a < b;
		}
	};

	template <class T = Seconds>
	static long long GetTimeElapsedSinceStart();
	
private:
	std::map<long long, Action, x> m_Callbacks;
	long long m_CurrentTimeStampNano;
	long long m_DeltaTime;
	inline static long long m_TimeElapsedSinceStart = 0;
public:
	long long QueueAction(long milliseconds, const Action&);
	long long QueueActionNS(long long nanoseconds, const Action&);
	bool Tick();
	_NODISCARD long long Now() const;
};

template <typename T>
long long GameTime::GetTimeElapsedSinceStart()
{
	const std::chrono::nanoseconds dur(m_TimeElapsedSinceStart);
	return std::chrono::duration_cast<T>(dur).count();
	// return m_TimeElapsedSinceStart;
}