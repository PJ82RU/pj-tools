#ifndef PJCAN_TOOLS_BYTES_H
#define PJCAN_TOOLS_BYTES_H

#include <Arduino.h>

namespace tools {
    class Bytes {
    public:
        /**
         * Конвертировать байты в строку HEX
         * @param bytes Массив байт
         * @param size Размер массива
         * @param upper_case Строка HEX в верхнем регистре
         * @return Строка HEX
         */
        static String bytes2hex(uint8_t bytes[], size_t size, bool upper_case = true);

        /**
         * Конвертировать строку HEX в байты
         * @param hex Строка HEX
         * @param bytes Массив байт
         * @param size Размер массива
         * @return Результат выполнения
         */
        static bool hex2bytes(String &hex, uint8_t bytes[], size_t size);

        /**
         * Конвертировать байты в число HEX
         * @param byte Байт
         * @return Число
         */
        static long byte2hex(uint8_t byte);

        /**
         * Сравнить два массива данных
         * @param buf1 Буфер данных 1
         * @param buf2 Буфер данных 2
         * @param size Размер буфера
         * @return Результат сравнения
         */
        static bool compare(const uint8_t *buf1, const uint8_t *buf2, size_t size);

        /**
         * Поменять байты местами в двух байтовом числе uint16_t
         * @param value Значение
         * @return Новое значение
         */
        static uint16_t byte_swap(uint16_t value);
    };
}

#endif //PJCAN_TOOLS_BYTES_H
