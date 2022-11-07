#pragma once

class ThreadJob
{
public:
    virtual int Execute() const = 0;

    virtual ~ThreadJob() = default;
};
