#ifndef PJ_TOOLS_QUEUE_H
#define PJ_TOOLS_QUEUE_H

#include <Arduino.h>

namespace tools
{
    class Queue
    {
    public:
        /**
         * @brief Создает очередь FreeRTOS
         * @param queue_length Максимальное количество элементов
         * @param item_size Размер каждого элемента в байтах
         * @throws std::runtime_error Если не удалось создать очередь
         */
        Queue(UBaseType_t queue_length, UBaseType_t item_size);

        ~Queue() noexcept;

        // Запрет копирования
        Queue(const Queue&) = delete;
        Queue& operator=(const Queue&) = delete;

        // Поддержка перемещения
        Queue(Queue&& other) noexcept;
        Queue& operator=(Queue&& other) noexcept;

        /**
         * @brief Количество элементов в очереди
         */
        [[nodiscard]] UBaseType_t messages_waiting() const noexcept;

        /**
         * @brief Количество свободных мест в очереди
         */
        [[nodiscard]] UBaseType_t spaces_available() const noexcept;

        /**
         * @brief Отправить элемент в очередь
         * @param item Указатель на данные для отправки
         * @param ticks_to_wait Время ожидания (по умолчанию не ждать)
         * @return true если успешно, false при ошибке или таймауте
         */
        [[nodiscard]] bool send(const void* item, TickType_t ticks_to_wait = 0) const noexcept;

        /**
         * @brief Перезаписать элемент в очереди (для очередей длиной 1)
         * @param item Указатель на данные для отправки
         * @return true если успешно
         */
        [[nodiscard]] bool overwrite(const void* item) const noexcept;

        /**
         * @brief Получить элемент из очереди
         * @param buffer Буфер для принятых данных
         * @param ticks_to_wait Время ожидания (по умолчанию ждать вечно)
         * @return true если успешно, false при ошибке или таймауте
         */
        [[nodiscard]] bool receive(void* buffer, TickType_t ticks_to_wait = portMAX_DELAY) const noexcept;

        /**
         * @brief Очистить очередь
         */
        void reset() const noexcept;

    private:
        QueueHandle_t handle_ = nullptr;

        void cleanup() noexcept;
    };
}

#endif // PJ_TOOLS_QUEUE_H