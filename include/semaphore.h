#ifndef PJ_TOOLS_SEMAPHORE_H
#define PJ_TOOLS_SEMAPHORE_H

#include <Arduino.h>

// Макросы для удобного использования семафора с отладочной информацией
/// @brief Макрос для захвата семафора с таймаутом 3 секунды и отладочной информацией
#define SemaphoreTake() semaphore.take(pdMS_TO_TICKS(3000), __PRETTY_FUNCTION__, __LINE__)

/// @brief Макрос для освобождения семафора с отладочной информацией
#define SemaphoreGive() semaphore.give(__PRETTY_FUNCTION__, __LINE__)

namespace tools
{
    /// @brief Класс-обертка для работы с семафорами FreeRTOS
    ///
    /// Предоставляет потокобезопасный интерфейс для работы с семафорами,
    /// включая поддержку рекурсивных мьютексов и отладочной информации
    class Semaphore
    {
    public:
        /**
         * @brief Конструктор семафора/мьютекса
         * @param recursive Флаг создания рекурсивного мьютекса
         * @throws std::runtime_error При ошибке создания семафора
         */
        explicit Semaphore(bool recursive);

        /**
         * @brief Деструктор - автоматически освобождает ресурсы семафора
         */
        ~Semaphore() noexcept;

        // Запрещаем копирование
        Semaphore(const Semaphore&) = delete; ///< Запрет копирования
        Semaphore& operator=(const Semaphore&) = delete; ///< Запрет присваивания

        // Разрешаем перемещение
        Semaphore(Semaphore&& other) noexcept; ///< Конструктор перемещения
        Semaphore& operator=(Semaphore&& other) noexcept; ///< Оператор перемещающего присваивания

        /**
         * @brief Захват семафора (блокирующий)
         * @param block_time Время ожидания в тиках (portMAX_DELAY - бесконечно)
         * @return true - успешный захват, false - таймаут или ошибка
         */
        [[nodiscard]] bool take(TickType_t block_time = portMAX_DELAY) const noexcept;

        /**
         * @brief Захват семафора с отладочной информацией
         * @param block_time Время ожидания в тиках
         * @param func Имя вызывающей функции (для отладки)
         * @param line Номер строки вызова (для отладки)
         * @return true - успешный захват, false - таймаут или ошибка
         */
        [[nodiscard]] bool take(TickType_t block_time, const char* func, int line) const noexcept;

        /**
         * @brief Освобождение семафора
         * @return true - успешное освобождение, false - ошибка
         */
        [[nodiscard]] bool give() const noexcept;

        /**
         * @brief Освобождение семафора с отладочной информацией
         * @param func Имя вызывающей функции (для отладки)
         * @param line Номер строки вызова (для отладки)
         * @return true - успешное освобождение, false - ошибка
         */
        [[nodiscard]] bool give(const char* func, int line) const noexcept;

        /**
         * @brief Получение текущего значения счетчика семафора
         * @return Текущее значение счетчика
         */
        [[nodiscard]] unsigned int get_count() const noexcept;

    private:
        SemaphoreHandle_t handle = nullptr; ///< Хэндл семафора FreeRTOS
        bool recursive_mutex = false; ///< Флаг рекурсивного мьютекса

        /**
         * @brief Внутренняя функция освобождения ресурсов
         */
        void cleanup() const noexcept;
    };
}

#endif // PJ_TOOLS_SEMAPHORE_H
