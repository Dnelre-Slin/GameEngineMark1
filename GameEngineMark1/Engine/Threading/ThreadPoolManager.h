#pragma once
#include <vector>

#include "ThreadJob.h"
#include "ThreadWorker.h"

class ThreadPoolManager 
{
public:
	ThreadPoolManager();
	~ThreadPoolManager();
	void Run(const ThreadJob* Job) const;
	void ShutDown();

private:
	std::vector<std::shared_ptr<ThreadWorker>> m_Workers;
	unsigned int m_MaxThreadCount = 0;
};