#ifndef PJCAN_FIRMWARE_SW_BASE_H
#define PJCAN_FIRMWARE_SW_BASE_H

#include "callback.h"
#include "thread.h"

namespace hardware
{
    /** Тип действий */
    typedef enum sw_press_t
    {
        PRESS_PRESSED,
        PRESS_DUAL,
        PRESS_TRIPLE,
        PRESS_HOLD
    } sw_press_t;

    class SWBase
    {
    public:
        /**
         * Следим за нажатиями кнопок
         * @param pv_parameters
         */
        friend void task_receive(void* pv_parameters);

        /** Поток */
        Thread thread;

        /** Количество чтений кнопки */
        uint8_t conf_read_number = 3;
        /** Минимальное доступное значение */
        uint16_t conf_resistance_min = 17;
        /** Значение отпущенной кнопки */
        uint16_t conf_resistance_max = 3800;

        /**
         * Базовый класс кнопок
         * @param number Количество кнопок
         */
        explicit SWBase(const uint8_t number);

        virtual ~SWBase();

        /** Запуск сервиса
         * @param pin Контакт кнопок
         */
        virtual bool begin(gpio_num_t pin);

        /** Остановка сервиса */
        void end();

        /**
         * Записать количество кнопок
         * @param number Значение
         * @return Результат
         */
        bool set(const uint8_t number);

    protected:
        uint8_t index_number = 0; // Количество кнопок
        int8_t index_last = -1; // Текущая кнопка
        uint16_t resistance_last = 0; // Последнее определенное сопротивление

        tools::Semaphore semaphore;

        /**
         * Чтение максимального сопротивления кнопки
         * @param index Индекс кнопки
         * @return Значение
         */
        virtual uint16_t get_resistance_max(uint8_t index)
        {
            return 0;
        }

        /**
         * Событие нажатия
         * @param index Индекс кнопки
         * @param pressed Статус
         * @param resistance Сопротивление
         */
        virtual void on(uint8_t index, bool pressed, uint16_t resistance)
        {
        }

        /** Выполнение потока */
        virtual void handle_receive();

    private:
        gpio_num_t pin_sw = GPIO_NUM_NC;

        int8_t read_index = -1; // Индекс прочитанной кнопки
        uint8_t read_count = 0; // Счетчик чтения сопротивления кнопки
    };
}

#endif //PJCAN_FIRMWARE_SW_BASE_H
