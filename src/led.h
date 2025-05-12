#ifndef PJ_TOOLS_LED_H
#define PJ_TOOLS_LED_H

#include <Arduino.h>

namespace tools {
    /** Тип работы светодиода */
    typedef enum led_t
    {
        LED_OFF = 0, // Светодиод не горит
        LED_LIGHT = 1, // Светодиод горит
        LED_BLINK = 2, // Светодиод мигает с указанным интервалом
        LED_DOUBLE_BLINK = 3, // Двойное мигание светодиодом, с указанным интервалом
        LED_TRIPLE_BLINK = 4 // Тройное мигание светодиодом, с указанным интервалом
    } led_t;

    class Led {
        public:
        /** Интервал мигания LED, мс */
        uint16_t blink = 500;
        /** Состояние светодиода: true - включен, false - выключен */
        bool state = false;

        /**
         * Управление светодиодом устройства
         * @param gpio_led Пин светодиода
         */
        explicit Led(gpio_num_t gpio_led = GPIO_NUM_NC);

        /**
         * Инициализировать светодиод
         * @param gpio_led Пин светодиода
         */
        void set(gpio_num_t gpio_led);

        /**
         * Изменить тип работы светодиода
         * @param type Тип
         */
        void set_type(led_t type);

        /** Обработчик мигания светодиода */
        void handle(unsigned long ms = 0);

    protected:
        /** Пин светодиода */
        gpio_num_t _pin = GPIO_NUM_NC;
        /** Тип работы светодиода */
        led_t _type = led_t::LED_OFF;
        /** Текущий шаг состояния светодиода */
        uint8_t _step = 0;
        unsigned long _ms = 0;
    };
}

#endif //PJ_TOOLS_LED_H
