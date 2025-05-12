#ifndef PJ_TOOLS_CALLBACK_H
#define PJ_TOOLS_CALLBACK_H

#include "thread.h"
#include "semaphore.h"
#include "queue.h"
#include "simple_callback.h"

namespace tools
{
    class Callback
    {
    public:
        typedef bool (*event_send_t)(void*, void*);

        typedef struct item_t
        {
            bool only_index;
            event_send_t p_item;
            void* p_parameters;
        } item_t;

        typedef struct buffer_item_t
        {
            int16_t index_item;
            uint8_t index_buffer;
        } buffer_item_t;

        /**
         * Обратный вызов
         * @param pv_parameters
         */
        friend void task_callback(void* pv_parameters);

        /** Поток */
        Thread thread;
        /** Очередь */
        Queue queue;
        /** Обратный вызов родителя */
        SimpleCallback parent_callback;

        /**
         * Обратный вызов
         * @param num Количество элементов буфера
         * @param size Размер элемента буфера
         * @param t_name Имя задачи
         * @param t_stack_depth Глубина стека
         * @param t_priority Приоритет
         */
        Callback(uint8_t num, size_t size, const char* t_name, uint32_t t_stack_depth = 3072,
                 UBaseType_t t_priority = 18);
        ~Callback();

        /**
         * Инициализация
         * @param num Количество функций обратного вызова
         * @return Результат выполнения
         */
        bool init(uint8_t num);

        /** Статус инициализации */
        bool is_init() const;

        /**
         * Записать функцию обратного вызова
         * @param item Функция обратного вызова
         * @param p_parameters Параметры передаваемые в функцию обратного вызова
         * @param only_index Вызывать только по индексу
         * @return Индекс функции обратного вызова
         */
        int16_t set(event_send_t item, void* p_parameters = nullptr, bool only_index = false);

        /** Очистить список функций обратного вызова */
        void clear();

        /**
         * Вызвать функцию обратного вызова
         * @param value Передаваемые значения
         * @param index Индекс функции обратного вызова
         */
        void call(const void* value, int16_t index = -1);

        /**
         * Чтение значения из буфера
         * @param value Значение
         * @return Результат выполнения
         */
        bool read(void* value);

    protected:
        /** Количество функций обратного вызова */
        uint8_t num_items = 0;
        /** Список функций обратного вызова */
        item_t* items = nullptr;

        /**
         * Вызвать функции обратного вызова
         * @param b_item Индекс функции обратного вызова и индекс данных в буфере
         */
        void call_items(const buffer_item_t& b_item);

        /** Выполнение потока */
        void handle();

    private:
        /** Количество элементов буфера */
        uint8_t num_buffer = 0;
        /** Размер элемента буфера */
        size_t size_buffer = 0;
        /** Позиция в буфере */
        uint8_t index_buffer = 0;
        /** Буфер данных */
        uint8_t* buffer = nullptr;

        Semaphore semaphore;
    };
}

#endif //PJ_TOOLS_CALLBACK_H
