#ifndef PJCAN_TOOLS_SEMAPHORE_H
#define PJCAN_TOOLS_SEMAPHORE_H

#include <Arduino.h>

#define SemaphoreTake() semaphore.take(pdMS_TO_TICKS(3000), __PRETTY_FUNCTION__, __LINE__)
#define SemaphoreGive() semaphore.give(pdMS_TO_TICKS(3000), __PRETTY_FUNCTION__, __LINE__)

namespace tools
{
    class Semaphore
    {
    public:
        /**
         * Обвертка Семафор (по мере необходимости будут добавляться др. методы)
         * @param recursive Использовать рекурсивные методы
         */
        explicit Semaphore(bool recursive);
        ~Semaphore();

        /**
         * Захват семафора
         * @param block_time Время в тиках
         * @return Результат
         */
        bool take(TickType_t block_time = portMAX_DELAY);
        /**
         * Захват семафора (для отладки используем SemaphoreTake())
         * @param block_time Время в тиках
         * @param _func Имя функции, которая вызывает take (для отладки)
         * @param _line Строка в коде (для отладки)
         * @return Результат
         */
        bool take(TickType_t block_time, const char* _func, int _line);

        /**
         * Освободить семафор
         * @return Результат
         */
        bool give();
        /**
         * Освободить семафор (для отладки используем SemaphoreGive())
         * @param _func Имя функции, которая вызывает take (для отладки)
         * @param _line Строка в коде (для отладки)
         * @return Результат
         */
        bool give(const char* _func, int _line);

        /** Возвращает счетчик семафора */
        int get_count() const;

    protected:
        SemaphoreHandle_t handle = nullptr;
        bool recursive_mutex = false;
    };
}

#endif //PJCAN_TOOLS_SEMAPHORE_H
