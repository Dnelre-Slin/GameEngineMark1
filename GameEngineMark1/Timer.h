#pragma once

#include <vector>
#include <map>
#include <functional>
#include <ctime>
#include <chrono>

#ifndef UNIX_TIME_STAMP
#define UNIX_TIME_STAMP std::time(nullptr)
#endif

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
	std::map<int, Action, x> m_Callbacks;
	std::time_t m_CurrentTimeStamp;
	float m_DeltaTime;
public:
	int QueueAction(float ms, Action);
	bool Tick();
	GameTime();
};

