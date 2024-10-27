#ifndef PJCAN_TOOLS_SEMAPHORE_H
#define PJCAN_TOOLS_SEMAPHORE_H

#include <Arduino.h>

namespace tools {
    class Semaphore {
    public:
        /** Обвертка Семафор (по мере необходимости будут добавляться др. методы) */
        Semaphore();
        ~Semaphore();

        /**
         * Захват семафора
         * @param block_time Время в тиках
         * @return
         */
        bool take(TickType_t block_time = portMAX_DELAY);

        /** Освободить семафор */
        bool give();

        /**
         * Устанавливаем время ожидания
         * @param value Значение
         */
        void set_wait_time(unsigned long value);

        /** Ждем когда наступит время */
        void wait_time() const;

    protected:
        SemaphoreHandle_t handle = nullptr;
        unsigned long ms_wait = 0;
    };
}

#endif //PJCAN_TOOLS_SEMAPHORE_H
