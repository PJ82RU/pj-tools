#include "semaphore.h"

namespace tools {
    Semaphore::Semaphore() {
        handle = xSemaphoreCreateRecursiveMutex();
    }

    Semaphore::~Semaphore() {
        vSemaphoreDelete(handle);
    }

    bool Semaphore::take(TickType_t block_time) {
        return xSemaphoreTakeRecursive(handle, block_time) == pdTRUE;
    }

    bool Semaphore::give() {
        return xSemaphoreGiveRecursive(handle) == pdTRUE;
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
