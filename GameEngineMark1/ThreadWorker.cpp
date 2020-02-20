#include "ThreadWorker.h"

#include <iostream>

ThreadWorker::ThreadWorker()
{
	m_ShouldStop = false;
	m_Thread = new std::thread([this]() { WaitForWorkForever(); });
}

ThreadWorker::~ThreadWorker()
{
	std::unique_lock<std::mutex> lock(m_QueueMutex);
	m_ShouldStop = true;
	
	m_Thread->join();
	delete m_Thread;
}

// https://stackoverflow.com/a/32593825/3911951
void ThreadWorker::WaitForWorkForever() {
	while (!m_ShouldStop)
	{
		// Function pointer
		void (*current_job)();
		{
			std::cout << "Waiting for work..." << std::endl;
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_MutexLockWait.wait(lock, [this] { return !m_FunctionQueue.empty() || m_ShouldStop; });
			current_job = m_FunctionQueue.front();
			m_FunctionQueue.pop();
		}
		if (current_job != nullptr)
		{
			std::cout << "Executing work..." << std::endl;
			current_job();
		}
	}
}

void ThreadWorker::Queue(void (*func)())
{
	if (!m_ShouldStop)
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_FunctionQueue.push(func);
		}
		m_MutexLockWait.notify_one();
	}
}

int ThreadWorker::QueueLength()
{
	return m_FunctionQueue.size();
}