#include "clock.h"
#include <cstdio>
#include <cstring>

namespace pj_tools
{
    void formatTime(char* buffer, unsigned long time, const bool showDay,
                    const bool showHour, const bool showMinute, const bool showSecond) noexcept
    {
        if (buffer == nullptr)
        {
            return;
        }

        // Конвертация времени из мс в компоненты
        time /= 1000; // секунды
        const uint8_t seconds = time % 60;
        time /= 60; // минуты
        const uint8_t minutes = time % 60;
        time /= 60; // часы
        const uint8_t hours = time % 24;
        const uint8_t days = time / 24;

        char* ptr = buffer;
        bool needSeparator = false;

        // Форматирование дней
        if (showDay && days > 0)
        {
            const int written = snprintf(ptr, 4, "%u", days);
            if (written > 0)
            {
                ptr += written;
                needSeparator = showHour || showMinute || showSecond;
                if (needSeparator)
                {
                    *ptr++ = '.';
                }
            }
        }

        // Форматирование часов
        if (showHour)
        {
            const int written = snprintf(ptr, 3, "%02u", hours);
            if (written > 0)
            {
                ptr += written;
                needSeparator = showMinute || showSecond;
                if (needSeparator)
                {
                    *ptr++ = ':';
                }
            }
        }

        // Форматирование минут
        if (showMinute)
        {
            const int written = snprintf(ptr, 3, "%02u", minutes);
            if (written > 0)
            {
                ptr += written;
                if (showSecond)
                {
                    *ptr++ = ':';
                }
            }
        }

        // Форматирование секунд
        if (showSecond)
        {
            snprintf(ptr, 3, "%02u", seconds);
            ptr += 2;
        }

        // Гарантированное завершение строки
        *ptr = '\0';
    }
} // namespace pj_tools
