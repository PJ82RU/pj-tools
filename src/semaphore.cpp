#include "semaphore.h"

namespace tools
{
    Semaphore::Semaphore(const bool recursive)
    {
        recursive_mutex = recursive;
        handle = recursive_mutex ? xSemaphoreCreateRecursiveMutex() : xSemaphoreCreateMutex();
    }

    Semaphore::~Semaphore()
    {
        vSemaphoreDelete(handle);
    }

    bool Semaphore::take(const TickType_t block_time)
    {
        return recursive_mutex
                   ? xSemaphoreTakeRecursive(handle, block_time) == pdTRUE
                   : xSemaphoreTake(handle, block_time) == pdTRUE;
    }

    bool Semaphore::take(const TickType_t block_time, const char* _func, int _line)
    {
        const bool result = take(block_time);
        if (!result && _func)
            log_w("[%s:%d]: Semaphore timeout or error", _func, _line);
        return result;
    }

    bool Semaphore::give()
    {
        return recursive_mutex ? xSemaphoreGiveRecursive(handle) == pdTRUE : xSemaphoreGive(handle) == pdTRUE;
    }

    bool Semaphore::give(const char* _func, int _line)
    {
        const bool result = give();
        if (!result && _func)
            log_w("[%s:%d]: Semaphore give error", _func, _line);
        return result;
    }

    int Semaphore::get_count() const
    {
        return uxSemaphoreGetCount(handle);
    }
}
