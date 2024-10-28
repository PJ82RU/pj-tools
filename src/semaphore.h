#ifndef PJCAN_TOOLS_SEMAPHORE_H
#define PJCAN_TOOLS_SEMAPHORE_H

#include <Arduino.h>

#define SemaphoreTake() semaphore(pdMS_TO_TICKS(3000), __FUNCTION__, __LINE__)

namespace tools {
    class Semaphore {
    public:
        /**
         * Обвертка Семафор (по мере необходимости будут добавляться др. методы)
         * @param recursive Использовать рекурсивные методы
         */
        Semaphore(bool recursive);
        ~Semaphore();

        /**
         * Захват семафора
         * @param block_time Время в тиках
         * @return
         */
        bool take(TickType_t block_time = portMAX_DELAY, const char *function = nullptr, int line = 0);

        /** Освободить семафор */
        bool give();

        /** Возвращает счетчик семафора */
        int get_count();

        /**
         * Устанавливаем время ожидания
         * @param value Значение
         */
        void set_wait_time(unsigned long value);

        /** Ждем когда наступит время */
        void wait_time() const;

    protected:
        SemaphoreHandle_t handle = nullptr;
        bool recursive_mutex = false;
        unsigned long ms_wait = 0;
    };
}

#endif //PJCAN_TOOLS_SEMAPHORE_H
