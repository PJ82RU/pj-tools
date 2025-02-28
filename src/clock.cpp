#include "clock.h"

namespace tools
{
    void get_time(char* buffer, unsigned long time, const bool day, const bool hour, const bool minute,
                  const bool second)
    {
        time /= 1000;
        const uint8_t countSecond = time % 60;
        time /= 60;
        const uint8_t countMinute = time % 60;
        time /= 60;
        const uint8_t countHour = time % 24;
        const uint8_t countDay = time / 24;

        int pos = 0;
        if (day)
        {
            pos += (hour || minute || second)
                       ? sprintf(&buffer[pos], "%d.", countDay)
                       : sprintf(&buffer[pos], "%d",
                                 countDay);
        }
        if (hour)
        {
            pos += (minute || second)
                       ? sprintf(&buffer[pos], "%02d:", countHour)
                       : sprintf(&buffer[pos], "%02d",
                                 countHour);
        }
        if (minute)
        {
            pos += second ? sprintf(&buffer[pos], "%02d:", countMinute) : sprintf(&buffer[pos], "%02d", countMinute);
        }
        if (second)
        {
            sprintf(&buffer[pos], "%02d", countSecond);
        }
    }
}
