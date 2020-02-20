#pragma once
#include <queue>
#include <mutex>

class ThreadWorker 
{
public:
	ThreadWorker();
	~ThreadWorker();
	void WaitForWorkForever();
	void Queue(void (*)());
	int QueueLength();

private:
	bool m_ShouldStop;
	bool m_IsRunning;

	std::queue<void (*)()> m_FunctionQueue;
	std::mutex m_QueueMutex;
	std::condition_variable m_MutexLockWait;
	std::thread* m_Thread = nullptr;
};