#ifndef PJ_TOOLS_LED_H
#define PJ_TOOLS_LED_H

#include <Arduino.h>

namespace tools
{
    /// @brief Перечисление режимов работы светодиода
    enum class LedMode
    {
        Off, ///< Светодиод выключен
        On, ///< Светодиод включен постоянно
        Blink, ///< Одиночное мигание (периодическое включение/выключение)
        DoubleBlink, ///< Двойное мигание (две вспышки с паузой)
        TripleBlink ///< Тройное мигание (три вспышки с паузой)
    };

    /// @brief Класс для управления светодиодом
    class Led
    {
    public:
        /**
         * @brief Конструктор объекта светодиода
         * @param pin Номер GPIO пина (по умолчанию GPIO_NUM_NC - не используется)
         */
        explicit Led(gpio_num_t pin = GPIO_NUM_NC) noexcept;

        /**
         * @brief Инициализация или изменение GPIO пина светодиода
         * @param pin Номер GPIO пина для управления светодиодом
         */
        void init(gpio_num_t pin) noexcept;

        /**
         * @brief Установка режима работы светодиода
         * @param mode Режим работы из перечисления LedMode
         */
        void setMode(LedMode mode) noexcept;

        /**
         * @brief Обновление состояния светодиода согласно текущему режиму
         * @param current_time Текущее время в миллисекундах (0 - использовать millis())
         */
        void update(uint32_t current_time = 0) noexcept;

        // Конфигурационные параметры
        uint16_t blink_interval = 500; ///< Интервал мигания в миллисекундах

        // Текущее состояние
        bool is_on = false; ///< Флаг текущего состояния светодиода (включен/выключен)

    private:
        gpio_num_t pin_ = GPIO_NUM_NC; ///< Номер GPIO пина светодиода
        LedMode mode_ = LedMode::Off; ///< Текущий режим работы
        uint8_t step_ = 0; ///< Текущий шаг в последовательности мигания
        uint32_t next_update_ = 0; ///< Время следующего обновления состояния

        /**
         * @brief Внутренний метод для обновления физического состояния пина
         */
        void updateOutput() const noexcept;
    };
}

#endif // PJ_TOOLS_LED_H
