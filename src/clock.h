#ifndef PJCAN_TOOLS_CLOCK_H
#define PJCAN_TOOLS_CLOCK_H

#include <Arduino.h>

namespace tools
{
    /**
     * Читать форматированное время
     * @param buffer Буфер (16 символов)
     * @param time   Время, мс
     * @param day	 Показать дни
     * @param hour	 Показать часы
     * @param minute Показать минуты
     * @param second Показать секунды
     */
    void get_time(char buffer[16], unsigned long time, bool day = false, bool hour = true, bool minute = true,
                  bool second = true);
}

#endif //PJCAN_TOOLS_CLOCK_H
