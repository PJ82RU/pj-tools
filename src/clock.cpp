#include "clock.h"

namespace tools {
    void get_time(char *buffer, unsigned long time, bool day, bool hour, bool minute, bool second) {
        uint8_t countDay, countHour, countMinute, countSecond;

        time /= 1000;
        countSecond = time % 60;
        time /= 60;
        countMinute = time % 60;
        time /= 60;
        countHour = time % 24;
        countDay = time / 24;

        int pos = 0;
        if (day) {
            pos += (hour || minute || second) ? sprintf(&buffer[pos], "%d.", countDay) : sprintf(&buffer[pos], "%d",
                                                                                                 countDay);
        }
        if (hour) {
            pos += (minute || second) ? sprintf(&buffer[pos], "%02d:", countHour) : sprintf(&buffer[pos], "%02d",
                                                                                            countHour);
        }
        if (minute) {
            pos += second ? sprintf(&buffer[pos], "%02d:", countMinute) : sprintf(&buffer[pos], "%02d", countMinute);
        }
        if (second) {
            sprintf(&buffer[pos], "%02d", countSecond);
        }
    }
}