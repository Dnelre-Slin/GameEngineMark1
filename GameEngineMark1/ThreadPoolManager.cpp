#include "ThreadPoolManager.h"

#include <iostream>

ThreadPoolManager::ThreadPoolManager() {
	// Collect max amount of theads that can be run on the CPU
	// leaving one thread for main thread that spawns the executable
	m_MaxThreadCount = std::thread::hardware_concurrency() - 1;

	std::cout << "Max threads: " << m_MaxThreadCount << std::endl;

	// Start thread pool worker threads
	// Max threads can be 0, if concurrency is 1
	if (m_MaxThreadCount > 0)
	{
		m_Workers = new ThreadWorker[m_MaxThreadCount];
	}
}

ThreadPoolManager::~ThreadPoolManager()
{
	delete[] m_Workers;
}

int ThreadPoolManager::Run(void (*functionPointer)())
{
	ThreadWorker* currentMinQueuedWorker = &m_Workers[0];
	for (int i = 1; i < m_MaxThreadCount; i++)
	{
		ThreadWorker& worker = m_Workers[i];
		if (worker.QueueLength() < currentMinQueuedWorker->QueueLength())
		{
			currentMinQueuedWorker = &worker;
		}
	}
	currentMinQueuedWorker->Queue(functionPointer);
	return 0;
}