#ifndef PJCAN_TOOLS_CALLBACK_H
#define PJCAN_TOOLS_CALLBACK_H

#include "thread.h"

namespace tools {
    class Callback {
    public:
        typedef bool (*event_send_t)(void *, void *);
        typedef void (*event_receive_t)(void *, void *);

        typedef struct item_t {
            bool only_index;
            event_send_t p_item;
            void *p_parameters;
        } item_t;

        typedef struct buffer_item_t {
            int16_t index_item;
            uint8_t index_buffer;
        } buffer_item_t;

        /**
         * Обратный вызов
         * @param pv_parameters
         */
        friend void task_callback(void *pv_parameters);

        /** Поток */
        Thread thread;

        /**
         * Обратный вызов
         * @param num           Количество элементов буфера
         * @param size          Размер элемента буфера
         * @param t_name        Имя задачи
         * @param t_stack_depth Глубина стека
         * @param t_priority    Приоритет
         */
        Callback(uint8_t num, size_t size, const char *t_name, uint32_t t_stack_depth = 1024, UBaseType_t t_priority = 15);

        ~Callback();

        /**
         * Инициализация
         * @param num Количество функций обратного вызова
         * @return Результат выполнения
         */
        bool init(uint8_t num);

        /** Статус инициализации */
        bool is_init();

        /**
         * Записать функцию обратного вызова
         * @param item         Функция обратного вызова
         * @param p_parameters Параметры передаваемые в функцию обратного вызова
         * @param only_index   Вызывать только по индексу
         * @return Индекс функции обратного вызова
         */
        int16_t set(event_send_t item, void *p_parameters = nullptr, bool only_index = false);

        /** Очистить список функций обратного вызова */
        void clear();

        /**
         * Вызвать функцию обратного вызова
         * @param value Передаваемые значения
         * @param index Индекс функции обратного вызова
         */
        void call(void *value, int16_t index = -1);

        /**
         * Чтение значения из буфера
         * @param value Значение
         * @return Результат выполнения
         */
        bool read(void *value);

        /**
         * Записать функцию обратного вызова родителя
         * @param cb           Функция обратного вызова родителя
         * @param p_parameters Параметры передаваемые в функцию обратного вызова родителя
         */
        void set_callback_receive(event_receive_t cb, void *p_parameters);

    protected:
        QueueHandle_t queue{};

        /** Функция обратного вызова родителя */
        event_receive_t cb_receive = nullptr;
        /** Параметры передаваемые в функцию обратного вызова родителя */
        void *p_receive_parameters = nullptr;

        /** Количество функций обратного вызова */
        uint8_t num_items = 0;
        /** Список функций обратного вызова */
        item_t *items = nullptr;

        /**
         * Вызвать функции обратного вызова
         * @param b_item Индекс функции обратного вызова и индекс данных в буфере
         */
        void call_items(buffer_item_t &b_item);

    private:
        /** Количество элементов буфера */
        uint8_t num_buffer = 0;
        /** Размер элемента буфера */
        size_t size_buffer = 0;
        /** Позиция в буфере */
        uint8_t index_buffer = 0;
        /** Буфер данных */
        uint8_t *buffer = nullptr;

        SemaphoreHandle_t mutex = nullptr;
    };
}

#endif //PJCAN_TOOLS_CALLBACK_H
