#include "led.h"
#include <esp_log.h>

namespace tools
{
    Led::Led(const gpio_num_t pin) noexcept
    {
        init(pin);
    }

    void Led::init(const gpio_num_t pin) noexcept
    {
        if (pin_ == GPIO_NUM_NC && pin != GPIO_NUM_NC)
        {
            pin_ = pin;
            pinMode(pin_, OUTPUT);
            digitalWrite(pin_, HIGH);
            ESP_LOGD("LED", "Initialized pin %d", pin_);
        }
    }

    void Led::setMode(const LedMode mode) noexcept
    {
        mode_ = mode;
        step_ = 0;
        next_update_ = 0;
        updateOutput(); // Немедленное обновление состояния
        ESP_LOGI("LED", "Mode changed to %d", static_cast<int>(mode));
    }

    void Led::update(uint32_t current_time) noexcept
    {
        if (pin_ == GPIO_NUM_NC) return;

        if (current_time == 0)
        {
            current_time = millis();
        }

        if (current_time < next_update_)
        {
            return;
        }

        uint16_t timeout = blink_interval;

        switch (mode_)
        {
        case LedMode::On:
            is_on = true;
            break;

        case LedMode::Blink:
            is_on = !is_on;
            step_ = is_on ? 1 : 0;
            break;

        case LedMode::DoubleBlink:
            is_on = (step_ % 2) == 0;
            step_ = (step_ + 1) % 4;
            timeout = blink_interval / 2;
            break;

        case LedMode::TripleBlink:
            is_on = (step_ % 2) == 0;
            step_ = (step_ + 1) % 6;
            timeout = blink_interval / 3;
            break;

        default: // Off
            is_on = false;
            break;
        }

        updateOutput();
        next_update_ = current_time + timeout;
    }

    void Led::updateOutput() const noexcept
    {
        digitalWrite(pin_, is_on ? LOW : HIGH);
        ESP_LOGD("LED", "Pin %d set to %s", pin_, is_on ? "LOW" : "HIGH");
    }
}
