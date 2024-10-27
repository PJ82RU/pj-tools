#include "semaphore.h"

namespace tools {
    Semaphore::Semaphore() {
        handle = xSemaphoreCreateMutex();
        log_d("The mutex semaphore has been created");
    }

    bool Semaphore::take(TickType_t block_time) {
        return xSemaphoreTake(handle, block_time) == pdTRUE;
    }

    bool Semaphore::give() {
        return xSemaphoreGive(handle) == pdTRUE;
    }
}
