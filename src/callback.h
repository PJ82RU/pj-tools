#ifndef PJCAN_TOOLS_CALLBACK_H
#define PJCAN_TOOLS_CALLBACK_H

#include <Arduino.h>

#define CALLBACK_BUFFER_NUM     16
#define CALLBACK_ITEM_MAX       100

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

        typedef struct call_value_t {
            int8_t index;
        } call_value_t;

        /**
         * Обратный вызов
         * @param pv_parameters
         */
        friend void task_callback(void *pv_parameters);

        /** Функция обратного вызова родителя */
        event_receive_t cb_receive = nullptr;
        /** Параметры передаваемые в функцию обратного вызова родителя */
        void *p_receive_parameters = nullptr;

        /** Буфер очереди */
        QueueHandle_t queue{};

        /**
         * Обратный вызов
         * @param num  Количество элементов буфера
         * @param size Размер элемента буфера
         */
        Callback(uint8_t num = CALLBACK_BUFFER_NUM, size_t size = sizeof(call_value_t));

        ~Callback();

        /**
         * Инициализация
         * @param num Количество функций обратного вызова
         * @return Результат выполнения
         */
        bool init(int8_t num);

        /** Статус инициализации */
        bool is_init();

        /**
         * Записать функцию обратного вызова
         * @param item         Функция обратного вызова
         * @param p_parameters Параметры передаваемые в функцию обратного вызова
         * @param only_index   Вызывать только по индексу
         * @return Индекс функции обратного вызова
         */
        int8_t set(event_send_t item, void *p_parameters = nullptr, bool only_index = false);

        /** Очистить список функций обратного вызова */
        void clear();

        /**
         * Вызвать функцию обратного вызова
         * @param value Передаваемые значения
         */
        void call(call_value_t &value);

        /**
         * Чтение значения из буфера
         * @param value Значение
         * @return Результат выполнения
         */
        bool read(call_value_t &value);

    protected:
        int8_t num_items = 0;
        item_t *items = nullptr;

        /**
         * Вызвать функции обратного вызова
         * @param value Передаваемые значения
         */
        void call_items(call_value_t &value);

    private:
        TaskHandle_t task_callback_call{};
    };
}

#endif //PJCAN_TOOLS_CALLBACK_H
