#include "ThreadPoolManager.h"

#include <iostream>

ThreadPoolManager::ThreadPoolManager() {
	// Collect max amount of threads that can be run on the CPU
	// leaving one thread for main thread that spawns the executable
	m_MaxThreadCount = std::thread::hardware_concurrency() - 1;

	std::cout << "Max threads: " << m_MaxThreadCount << std::endl;

	// Start thread pool worker threads
	// Max threads can be 0, if concurrency is 1
	if (m_MaxThreadCount > 0)
	{
		m_Workers.resize(m_MaxThreadCount);

		for (unsigned int i = 0; i < m_MaxThreadCount; ++i)
		{
			m_Workers[i] = std::make_shared<ThreadWorker>();
		}
	}
}

ThreadPoolManager::~ThreadPoolManager()
{
	ShutDown();
}

void ThreadPoolManager::Run(const ThreadJob* const Job) const
{
	std::shared_ptr<ThreadWorker> CurrentMinQueuedWorker = m_Workers[0];
	for (unsigned int i = 1; i < m_MaxThreadCount; ++i)
	{
		std::shared_ptr<ThreadWorker> Worker = m_Workers[i];
		if (Worker != nullptr && Worker->QueueLength() < CurrentMinQueuedWorker->QueueLength())
		{
			CurrentMinQueuedWorker = Worker;
		}
	}
	if (CurrentMinQueuedWorker != nullptr)
	{
		CurrentMinQueuedWorker->Queue(Job);
	}
}

void ThreadPoolManager::ShutDown()
{
	for (unsigned int i = 0; i < m_MaxThreadCount; ++i)
	{
		const std::shared_ptr<ThreadWorker> Worker = m_Workers[i];
		Worker->Stop();
	}
	m_Workers.clear();
}
