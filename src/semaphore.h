#ifndef PJ_TOOLS_SEMAPHORE_H
#define PJ_TOOLS_SEMAPHORE_H

#include <Arduino.h>

// Макросы для удобного использования семафора с отладочной информацией
#define SemaphoreTake() semaphore.take(pdMS_TO_TICKS(3000), __PRETTY_FUNCTION__, __LINE__)
#define SemaphoreGive() semaphore.give(__PRETTY_FUNCTION__, __LINE__)

namespace tools
{
    class Semaphore
    {
    public:
        /**
         * @brief Создает обертку для семафора
         * @param recursive Использовать рекурсивный мьютекс
         * @throws std::runtime_error Если не удалось создать семафор
         */
        explicit Semaphore(bool recursive);

        /**
         * @brief Деструктор - освобождает ресурсы семафора
         */
        ~Semaphore() noexcept;

        // Запрещаем копирование
        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

        // Разрешаем перемещение
        Semaphore(Semaphore&& other) noexcept;
        Semaphore& operator=(Semaphore&& other) noexcept;

        /**
         * @brief Захватить семафор
         * @param block_time Время ожидания в тиках
         * @return true если семафор захвачен, false при таймауте
         */
        [[nodiscard]] bool take(TickType_t block_time = portMAX_DELAY) const noexcept;

        /**
         * @brief Захватить семафор с отладочной информацией
         * @param block_time Время ожидания в тиках
         * @param func Имя функции, откуда вызван метод (для логов)
         * @param line Номер строки (для логов)
         * @return true если семафор захвачен, false при таймауте
         */
        [[nodiscard]] bool take(TickType_t block_time, const char* func, int line) const noexcept;

        /**
         * @brief Освободить семафор
         * @return true если семафор освобожден успешно
         */
        [[nodiscard]] bool give() const noexcept;

        /**
         * @brief Освободить семафор с отладочной информацией
         * @param func Имя функции, откуда вызван метод (для логов)
         * @param line Номер строки (для логов)
         * @return true если семафор освобожден успешно
         */
        [[nodiscard]] bool give(const char* func, int line) const noexcept;

        /**
         * @brief Получить текущее значение счетчика семафора
         * @return Текущее значение счетчика
         */
        [[nodiscard]] unsigned int get_count() const noexcept;

    private:
        SemaphoreHandle_t handle = nullptr; // Хэндл семафора FreeRTOS
        bool recursive_mutex = false; // Флаг рекурсивного мьютекса

        /**
         * @brief Внутренний метод для освобождения ресурсов
         */
        void cleanup() const noexcept;
    };
}

#endif // PJ_TOOLS_SEMAPHORE_H
