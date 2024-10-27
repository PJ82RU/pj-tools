#ifndef PJCAN_TOOLS_SEMAPHORE_H
#define PJCAN_TOOLS_SEMAPHORE_H

#include <Arduino.h>

namespace tools {
    class Semaphore {
    public:
        Semaphore();

        /**
         * Захват семафора
         * @param block_time Время в тиках
         * @return
         */
        bool take(TickType_t block_time = portMAX_DELAY);

        /** Освободить семафор */
        bool give();

    protected:
        SemaphoreHandle_t handle = nullptr;
    };
}

#endif //PJCAN_TOOLS_SEMAPHORE_H
