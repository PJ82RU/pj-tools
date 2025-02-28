#ifndef PJCAN_TOOLS_QUEUE_H
#define PJCAN_TOOLS_QUEUE_H

#include <Arduino.h>

namespace tools {
    class Queue {
    public:
        /**
         * Обвертка Управление очередью (по мере необходимости будут добавляться др. методы)
         * @param queue_length Максимальное количество элементов, которое может находиться в очереди
         * @param item_size Размер (в байтах), необходимый для хранения каждого элемента в очереди
         */
        Queue(UBaseType_t queue_length, UBaseType_t item_size);
        ~Queue();

        /** Возвращает количество сообщений, хранящихся в очереди */
        unsigned int messages_waiting();

        /** Возвращает количество свободных мест в очереди */
        unsigned int spaces_available();

        /**
         * Отправить элемент в очередь
         * @param p_item_to_queue Указатель на элемент, который должен быть помещен в очередь
         * @param ticks_to_wait Максимальное количество времени, на которое задача должна блокироваться в ожидании освобождения места в очереди
         * @return Результат выполнения
         */
        bool send(const void * p_item_to_queue, TickType_t ticks_to_wait = 0);

        /**
         * Отправить элемент в очередь перезаписывая данные, которые уже в очереди
         * @param p_item_to_queue Указатель на элемент, который должен быть помещен в очередь
         * @return Результат выполнения
         */
        bool overwrite(const void * p_item_to_queue);

        /**
         * Получить элемент из очереди
         * @param p_buffer Указатель на буфер, в который будет скопирован полученный элемент
         * @param ticks_to_wait Максимальное время, на которое задача должна быть заблокирована в ожидании получения элемента
         * @return Результат выполнения
         */
        bool receive(void *p_buffer, TickType_t ticks_to_wait = portMAX_DELAY);

        /** Сбрасывает очередь в исходное пустое состояние */
        void reset();

    protected:
        QueueHandle_t handle;
    };
}

#endif //PJCAN_TOOLS_QUEUE_H
