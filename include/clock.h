#ifndef PJ_TOOLS_CLOCK_H
#define PJ_TOOLS_CLOCK_H

#include <Arduino.h>

namespace pj_tools
{
    /**
     * @brief Форматирует время в читаемую строку
     * @param buffer Буфер для результата (минимум 16 байт)
     * @param time Время в миллисекундах
     * @param showDay Флаг отображения дней
     * @param showHour Флаг отображения часов
     * @param showMinute Флаг отображения минут
     * @param showSecond Флаг отображения секунд
     */
    void formatTime(char buffer[16],
                    unsigned long time,
                    bool showDay = false,
                    bool showHour = true,
                    bool showMinute = true,
                    bool showSecond = true) noexcept;
} // namespace pj_tools

#endif // PJ_TOOLS_CLOCK_H
