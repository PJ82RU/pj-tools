#include "clock.h"

namespace tools
{
    void get_time(char* buffer, unsigned long time, const bool day, const bool hour,
                  const bool minute, const bool second)
    {
        // Проверка на нулевой указатель буфера
        if (buffer == nullptr) return;

        // Конвертация времени
        time /= 1000;
        const uint8_t countSecond = time % 60;
        time /= 60;
        const uint8_t countMinute = time % 60;
        time /= 60;
        const uint8_t countHour = time % 24;
        const uint8_t countDay = time / 24;

        // Временный буфер для форматирования
        char temp[8];
        int pos = 0;

        // Форматирование дней
        if (day)
        {
            const int written = snprintf(temp, sizeof(temp), "%d", countDay);
            if (written > 0)
            {
                memcpy(buffer + pos, temp, written);
                pos += written;

                if (hour || minute || second)
                {
                    buffer[pos++] = '.';
                }
            }
        }

        // Форматирование часов
        if (hour)
        {
            const int written = snprintf(temp, sizeof(temp), "%02d", countHour);
            if (written > 0)
            {
                memcpy(buffer + pos, temp, written);
                pos += written;

                if (minute || second)
                {
                    buffer[pos++] = ':';
                }
            }
        }

        // Форматирование минут
        if (minute)
        {
            const int written = snprintf(temp, sizeof(temp), "%02d", countMinute);
            if (written > 0)
            {
                memcpy(buffer + pos, temp, written);
                pos += written;

                if (second)
                {
                    buffer[pos++] = ':';
                }
            }
        }

        // Форматирование секунд
        if (second)
        {
            snprintf(buffer + pos, 3, "%02d", countSecond);
        }

        // Гарантированное завершение строки
        buffer[pos] = '\0';
    }
}
