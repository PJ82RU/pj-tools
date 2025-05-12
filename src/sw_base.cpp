#include "sw_base.h"

namespace hardware
{
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

    void task_receive(void* pv_parameters)
    {
        if (pv_parameters)
        {
            const auto sw = static_cast<SWBase*>(pv_parameters);
            for (;;)
            {
                sw->handle_receive();
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }

#pragma clang diagnostic pop

    SWBase::SWBase(const uint8_t number) :
        thread("TASK_SW_RECEIVE", 2048, 10),
        semaphore(true)
    {
        set(number);
    }

    SWBase::~SWBase()
    {
        end();
    }

    bool SWBase::begin(const gpio_num_t pin)
    {
        bool result = false;
        if (semaphore.take())
        {
            if (index_number != 0)
            {
                if (pin_sw == GPIO_NUM_NC)
                {
                    pin_sw = pin;
                    result = thread.start(&task_receive, this);
                }
                else
                {
                    log_w("Buttons is already started");
                }
            }
            else
            {
                log_w("Invalid number of buttons");
            }
            semaphore.give();
        }
        return result;
    }

    void SWBase::end()
    {
        if (semaphore.take())
        {
            thread.stop();
            pin_sw = GPIO_NUM_NC;
            semaphore.give();
        }
    }

    bool SWBase::set(const uint8_t number)
    {
        const bool result = number != 0 && number < INT8_MAX;
        if (result)
        {
            if (semaphore.take())
            {
                index_number = number;
                index_last = -1;
                log_d("The number of buttons has been changed");
                semaphore.give();
            }
        }
        else
        {
            log_w("Invalid number of buttons");
        }
        return result;
    }

    void SWBase::handle_receive()
    {
        const uint16_t read = analogRead(pin_sw);
        if (read >= conf_resistance_min)
        {
            if (read < conf_resistance_max)
            {
                // Если кнопка нажата, то и нет смысла искать ее повторно
                if (index_last < 0)
                {
                    // Значение сопротивления относительно максимального значения, т.е. питания
                    resistance_last = read;
                    for (uint8_t idx = 0; idx < index_number; ++idx)
                    {
                        const uint16_t resistance_max = get_resistance_max(idx);
                        if (resistance_max != 0 && resistance_last <= resistance_max)
                        {
                            if (idx == read_index)
                            {
                                if (++read_count == conf_read_number)
                                {
                                    // Кнопка нажата
                                    log_d("Calling a button press");
                                    index_last = idx;
                                    on(idx, true, resistance_last);
                                }
                            }
                            else
                            {
                                read_index = idx;
                                read_count = 1;
                            }
                            return;
                        }
                    }
                }
            }
            else if (index_last >= 0)
            {
                // Кнопка отпущена
                log_d("Calling the button release");
                on(index_last, false, resistance_last);
                index_last = -1;
            }

            // Обнуляем счетчики
            if (read_index >= 0)
            {
                read_index = -1;
                read_count = 0;
            }
        }
    }
}
