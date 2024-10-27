#include "semaphore.h"

namespace tools {
    Semaphore::Semaphore(bool recursive) {
        recursive_mutex = recursive;
        handle = recursive_mutex ? xSemaphoreCreateRecursiveMutex() : xSemaphoreCreateMutex();
    }

    Semaphore::~Semaphore() {
        vSemaphoreDelete(handle);
    }

    bool Semaphore::take(TickType_t block_time) {
        return recursive_mutex ? xSemaphoreTakeRecursive(handle, block_time) == pdTRUE :
               xSemaphoreTake(handle, block_time) == pdTRUE;
    }

    bool Semaphore::give() {
        return recursive_mutex ? xSemaphoreGiveRecursive(handle) == pdTRUE : xSemaphoreGive(handle) == pdTRUE;
    }

    int Semaphore::get_count() {
        return uxSemaphoreGetCount(handle);
    }

    void Semaphore::set_wait_time(unsigned long value) {
        ms_wait = millis() + value;
    }

    void Semaphore::wait_time() const {
        unsigned long ms;
        while (ms_wait > (ms = millis())) delay(ms - ms_wait);
    }
}
