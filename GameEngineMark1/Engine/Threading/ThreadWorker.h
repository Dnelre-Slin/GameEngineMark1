#pragma once
#include <queue>
#include <mutex>

#include "ThreadWorker.h"

class ThreadJob;

class ThreadWorker 
{
public:
	ThreadWorker();
	~ThreadWorker();
	void WaitForWorkForever();
	void Queue(const ThreadJob* Job);
	_NODISCARD std::queue<const ThreadJob*>::size_type QueueLength() const;
	void Stop();

private:
	bool m_ShouldStop;

	std::queue<const ThreadJob*> m_FunctionQueue;
	std::mutex m_QueueMutex;
	std::condition_variable m_MutexLockWait;
	std::thread* m_Thread = nullptr;
};
