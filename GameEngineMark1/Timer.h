#pragma once

#include <vector>
#include <map>
#include <functional>
#include <ctime>
#include <chrono>

typedef std::function<void(void)> Action;
typedef std::function<bool(int, int)> Cmp;

class GameTime {
public:
	struct x {
		bool operator()(const int& a, const int& b) const {
			return a < b;
		}
	};

private:
	std::map<long long, Action, x> m_Callbacks;
	long long m_CurrentTimeStampNano;
	float m_DeltaTime;
public:
	long long QueueAction(long milliseconds, Action);
	long long QueueActionNS(long long nanoseconds, Action);
	bool Tick();
	GameTime();
	long long Now();
};

