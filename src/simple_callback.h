#ifndef PJCAN_TOOLS_SIMPLE_CALLBACK_H
#define PJCAN_TOOLS_SIMPLE_CALLBACK_H

#include <Arduino.h>

namespace tools {
    class SimpleCallback {
    public:
        typedef void (*event_receive_t)(void *, void *);

        SimpleCallback(event_receive_t cb, void *p_parameters);

        /**
         * Записать параметры обратного вызова
         * @param cb Функция
         * @param p_parameters Параметры передаваемые в функцию
         */
        void set(event_receive_t cb, void *p_parameters);

        /** Очистить параметры */
        void free();

        /**
         * Вызвать функцию обратного вызова
         * @param p_value Передаваемые значения
         */
        void call(void *p_value);

    protected:
        /** Функция обратного вызова */
        event_receive_t cb_receive = nullptr;
        /** Параметры передаваемые в функцию обратного вызова */
        void *p_receive_parameters = nullptr;
    };
}

#endif //PJCAN_TOOLS_SIMPLE_CALLBACK_H
