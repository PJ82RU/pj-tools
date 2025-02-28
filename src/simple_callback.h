#ifndef PJCAN_TOOLS_SIMPLE_CALLBACK_H
#define PJCAN_TOOLS_SIMPLE_CALLBACK_H

#include <Arduino.h>

namespace tools
{
    class SimpleCallback
    {
    public:
        typedef void (*event_receive_t)(void*, void*);

        /**
         * Простая реализация обратного вызова
         * @param cb Функция обратного вызова
         * @param p_parameters Значение, которое передается в качестве параметра функции обратного вызова
         */
        SimpleCallback(event_receive_t cb, void* p_parameters);
        SimpleCallback();

        /**
         * Записать параметры обратного вызова
         * @param cb Функция
         * @param p_parameters Параметры передаваемые в функцию
         */
        void set(event_receive_t cb, void* p_parameters);

        /** Очистить параметры */
        void free();

        /**
         * Вызвать функцию обратного вызова
         * @param p_value Передаваемые значения
         */
        void call(void* p_value) const;

    protected:
        /** Функция обратного вызова */
        event_receive_t cb_receive = nullptr;
        /** Параметры передаваемые в функцию обратного вызова */
        void* p_receive_parameters = nullptr;
    };
}

#endif //PJCAN_TOOLS_SIMPLE_CALLBACK_H
