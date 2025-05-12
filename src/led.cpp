#include "led.h"

namespace tools
{
    Led::Led(const gpio_num_t gpio_led)
    {
        set(gpio_led);
    }

    void Led::set(const gpio_num_t gpio_led)
    {
        if (_pin == GPIO_NUM_NC && gpio_led != GPIO_NUM_NC)
        {
            _pin = gpio_led;
            pinMode(gpio_led, OUTPUT);
            digitalWrite(gpio_led, HIGH);
            log_d("Pin mode OUTPUT for GPIO_LED_WORK = HIGH");
        }
    }

    void Led::set_type(const led_t type)
    {
        _type = type;
        _step = 0;
        _ms = 0;
    }

    void Led::handle(unsigned long ms)
    {
        if (ms == 0) ms = millis();
        if (_ms <= ms)
        {
            uint16_t timeout = blink;
            if (_pin != GPIO_NUM_NC)
            {
                switch (_type)
                {
                case LED_LIGHT:
                    {
                        digitalWrite(_pin, LOW);
                        state = true;
                        log_d("Set pin GPIO_LED_WORK = LOW");
                        break;
                    }

                case LED_BLINK:
                    {
                        const bool result = _step == 0;
                        digitalWrite(_pin, result ? LOW : HIGH);
                        state = result;
                        _step = result ? 1 : 0;
                        log_d("Set pin GPIO_LED_WORK = %s", result ? "LOW" : "HIGH");
                        break;
                    }

                case LED_DOUBLE_BLINK:
                    {
                        const bool result = _step == 0 || _step == 2;
                        digitalWrite(_pin, result ? LOW : HIGH);
                        state = result;
                        _step++;
                        if (_step > 6) _step = 0;
                        timeout = blink / 2;
                        log_d("Set pin GPIO_LED_WORK = %s", result ? "LOW" : "HIGH");
                        break;
                    }

                case LED_TRIPLE_BLINK:
                    {
                        const bool result = _step == 0 || _step == 2 || _step == 4;
                        digitalWrite(_pin, result ? LOW : HIGH);
                        state = result;
                        _step++;
                        if (_step > 8) _step = 0;
                        timeout = blink / 3;
                        log_d("Set pin GPIO_LED_WORK = %s", result ? "LOW" : "HIGH");
                        break;
                    }

                default:
                    {
                        digitalWrite(_pin, HIGH);
                        state = false;
                        log_d("Set pin GPIO_LED_WORK = HIGH");
                        break;
                    }
                }
            }
            _ms = ms + timeout;
        }
    }
}
