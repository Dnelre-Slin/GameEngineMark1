#pragma once
#include <vector>
#include <thread>
#include <mutex>

#include "ThreadWorker.h"

class ThreadPoolManager 
{
public:
	ThreadPoolManager();
	~ThreadPoolManager();
	int Run(void (*function_pointer)());

private:
	ThreadWorker* m_Workers = nullptr;
	int m_MaxThreadCount = 0;
};