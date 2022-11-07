#include "ThreadWorker.h"
#include "ThreadJob.h"

#include <iostream>

ThreadWorker::ThreadWorker()
{
	m_ShouldStop = false;
	m_Thread = new std::thread([this]() { WaitForWorkForever(); });
}

ThreadWorker::~ThreadWorker()
{
	std::cout << "Worker destroyed" << std::endl;
}

// https://stackoverflow.com/a/32593825/3911951
void ThreadWorker::WaitForWorkForever() {
	while (!m_ShouldStop)
	{
		// Function pointer
		const ThreadJob* CurrentJob = nullptr;
		{
			std::cout << std::endl << "Waiting for work on thread " << std::this_thread::get_id() <<  "..." << std::endl;
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_MutexLockWait.wait(lock, [this] { return !m_FunctionQueue.empty() || m_ShouldStop; });
			if (!m_FunctionQueue.empty())
			{
				CurrentJob = m_FunctionQueue.front();
				m_FunctionQueue.pop();
			}
		}

		if (CurrentJob != nullptr)
		{
			std::cout << std::endl << "Received work on thread " << std::this_thread::get_id() <<  "..." << std::endl;
			if (const int ExitCode = CurrentJob->Execute(); ExitCode > 0)
			{
				std::cout << std::endl << "Job exited with exit code " << ExitCode << std::endl;
			}
			delete CurrentJob;
		}
	}
	std::cout << "Thread loop stopped" << std::endl;
}

void ThreadWorker::Queue(const ThreadJob* const Job)
{
	if (!m_ShouldStop)
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_FunctionQueue.push(Job);
		}
		m_MutexLockWait.notify_one();
	}
}

std::queue<void(*)()>::size_type ThreadWorker::QueueLength() const
{
	return m_FunctionQueue.size();
}

void ThreadWorker::Stop()
{
	std::cout << "Exiting worker thread " << m_Thread->get_id() << std::endl;

	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_ShouldStop = true;
		// Empty the queue and discard any remaining work
		std::queue<const ThreadJob*>().swap(m_FunctionQueue);
	}
	m_MutexLockWait.notify_all();

	if (m_Thread != nullptr)
	{
		m_Thread->join();
		delete m_Thread;
	}
}
