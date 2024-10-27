#include "semaphore.h"

namespace tools {
    Semaphore::Semaphore() {
        handle = xSemaphoreCreateMutex();
    }

    Semaphore::~Semaphore() {
        vSemaphoreDelete(handle);
    }

    bool Semaphore::take(TickType_t block_time) {
        return xSemaphoreTake(handle, block_time) == pdTRUE;
    }

    bool Semaphore::give() {
        return xSemaphoreGive(handle) == pdTRUE;
    }
}
