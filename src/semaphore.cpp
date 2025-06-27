#include "semaphore.h"

namespace tools
{
    Semaphore::Semaphore(const bool recursive) : recursive_mutex(recursive)
    {
        handle = recursive_mutex ? xSemaphoreCreateRecursiveMutex() : xSemaphoreCreateMutex();
    }

    Semaphore::~Semaphore() noexcept
    {
        cleanup();
    }

    Semaphore::Semaphore(Semaphore&& other) noexcept :
        handle(other.handle),
        recursive_mutex(other.recursive_mutex)
    {
        other.handle = nullptr;
    }

    Semaphore& Semaphore::operator=(Semaphore&& other) noexcept
    {
        if (this != &other)
        {
            cleanup();
            handle = other.handle;
            recursive_mutex = other.recursive_mutex;
            other.handle = nullptr;
        }
        return *this;
    }

    void Semaphore::cleanup() const noexcept
    {
        if (handle)
        {
            vSemaphoreDelete(handle);
        }
    }

    bool Semaphore::take(const TickType_t block_time) const noexcept
    {
        if (!handle) return false;

        return recursive_mutex
                   ? xSemaphoreTakeRecursive(handle, block_time) == pdTRUE
                   : xSemaphoreTake(handle, block_time) == pdTRUE;
    }

    bool Semaphore::take(const TickType_t block_time, const char* func, int line) const noexcept
    {
        const bool result = take(block_time);
        if (!result && func)
        {
            log_w("[%s:%d]: Semaphore take failed", func, line);
        }
        return result;
    }

    bool Semaphore::give() const noexcept
    {
        if (!handle) return false;

        return recursive_mutex ? xSemaphoreGiveRecursive(handle) == pdTRUE : xSemaphoreGive(handle) == pdTRUE;
    }

    bool Semaphore::give(const char* func, int line) const noexcept
    {
        const bool result = give();
        if (!result && func)
        {
            log_w("[%s:%d]: Semaphore give failed", func, line);
        }
        return result;
    }

    unsigned int Semaphore::get_count() const noexcept
    {
        return handle ? uxSemaphoreGetCount(handle) : 0;
    }
}
