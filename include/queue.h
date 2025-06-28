#ifndef PJ_TOOLS_QUEUE_H
#define PJ_TOOLS_QUEUE_H

#include <Arduino.h>

namespace tools
{
    /// @brief Класс-обертка для работы с очередью FreeRTOS
    ///
    /// Предоставляет безопасный интерфейс для работы с очередями FreeRTOS
    /// с поддержкой RAII и семантики перемещения
    class Queue
    {
    public:
        /**
         * @brief Конструктор очереди FreeRTOS
         * @param queue_length Максимальное количество элементов в очереди
         * @param item_size Размер каждого элемента в байтах
         * @throws std::runtime_error Если создание очереди не удалось
         */
        Queue(UBaseType_t queue_length, UBaseType_t item_size);

        /// @brief Деструктор - автоматически удаляет очередь FreeRTOS
        ~Queue() noexcept;

        // Запрет копирования
        Queue(const Queue&) = delete; ///< Запрет конструктора копирования
        Queue& operator=(const Queue&) = delete; ///< Запрет оператора присваивания

        // Поддержка перемещения
        Queue(Queue&& other) noexcept; ///< Конструктор перемещения
        Queue& operator=(Queue&& other) noexcept; ///< Оператор перемещающего присваивания

        /**
         * @brief Получить количество элементов в очереди
         * @return Количество элементов, ожидающих обработки
         */
        [[nodiscard]] UBaseType_t messages_waiting() const noexcept;

        /**
         * @brief Получить количество свободных мест в очереди
         * @return Количество доступных слотов для новых элементов
         */
        [[nodiscard]] UBaseType_t spaces_available() const noexcept;

        /**
         * @brief Отправить элемент в очередь
         * @param item Указатель на отправляемые данные
         * @param ticks_to_wait Время ожидания в тиках (0 - не ждать)
         * @return true если отправка успешна, false при ошибке или таймауте
         */
        [[nodiscard]] bool send(const void* item, TickType_t ticks_to_wait = 0) const noexcept;

        /**
         * @brief Перезаписать элемент в очереди (для очередей длиной 1)
         * @param item Указатель на новые данные
         * @return true если операция успешна
         * @note Используется только для очередей размером 1
         */
        [[nodiscard]] bool overwrite(const void* item) const noexcept;

        /**
         * @brief Получить элемент из очереди
         * @param buffer Буфер для принятых данных
         * @param ticks_to_wait Время ожидания в тиках (portMAX_DELAY - ждать вечно)
         * @return true если получение успешно, false при ошибке или таймауте
         */
        [[nodiscard]] bool receive(void* buffer, TickType_t ticks_to_wait = portMAX_DELAY) const noexcept;

        /**
         * @brief Очистить очередь
         * @note Удаляет все элементы из очереди
         */
        void reset() const noexcept;

    private:
        QueueHandle_t handle_ = nullptr; ///< Хэндл очереди FreeRTOS

        /// @brief Внутренняя функция для освобождения ресурсов
        void cleanup() noexcept;
    };
}

#endif // PJ_TOOLS_QUEUE_H
