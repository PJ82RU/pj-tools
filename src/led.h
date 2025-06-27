#ifndef PJ_TOOLS_LED_H
#define PJ_TOOLS_LED_H

#include <Arduino.h>

namespace tools
{
    enum class LedMode
    {
        Off, // Светодиод выключен
        On, // Светодиод включен
        Blink, // Одиночное мигание
        DoubleBlink, // Двойное мигание
        TripleBlink // Тройное мигание
    };

    class Led
    {
    public:
        /**
         * @brief Конструктор LED
         * @param pin Пин светодиода (GPIO_NUM_NC если не используется)
         */
        explicit Led(gpio_num_t pin = GPIO_NUM_NC) noexcept;

        /**
         * @brief Инициализировать/изменить пин светодиода
         * @param pin Номер GPIO пина
         */
        void init(gpio_num_t pin) noexcept;

        /**
         * @brief Установить режим работы
         * @param mode Режим работы светодиода
         */
        void setMode(LedMode mode) noexcept;

        /**
         * @brief Обновить состояние светодиода
         * @param current_time Текущее время (мс). Если 0 - используется millis()
         */
        void update(uint32_t current_time = 0) noexcept;

        // Конфигурация
        uint16_t blink_interval = 500; // Интервал мигания (мс)

        // Состояние
        bool is_on = false; // Текущее состояние (вкл/выкл)

    private:
        gpio_num_t pin_ = GPIO_NUM_NC;
        LedMode mode_ = LedMode::Off;
        uint8_t step_ = 0;
        uint32_t next_update_ = 0;

        void updateOutput() const noexcept;
    };
}

#endif // PJ_TOOLS_LED_H
