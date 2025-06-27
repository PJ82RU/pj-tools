#include "bytes.h"

namespace tools
{
    String bytes2hex(const uint8_t* bytes, const size_t size, const bool upper_case)
    {
        String res;
        if (size == 0 || bytes == nullptr)
        {
            return res;
        }

        res.reserve(size * 2); // Предварительное выделение памяти

        const char* hex_chars = upper_case ? "0123456789ABCDEF" : "0123456789abcdef";

        for (size_t i = 0; i < size; ++i)
        {
            res += hex_chars[(bytes[i] >> 4) & 0x0F];
            res += hex_chars[bytes[i] & 0x0F];
        }

        return res;
    }

    bool hex2bytes(const String& hex, uint8_t* bytes, const size_t size)
    {
        if (hex.length() == 0 || bytes == nullptr || size == 0 || hex.length() % 2 != 0)
        {
            return false;
        }

        const size_t len = min(size, hex.length() / 2);

        for (size_t i = 0, j = 0; j < len; i += 2, j++)
        {
            const uint8_t high = hex[i] <= '9' ? hex[i] - '0' : (hex[i] & 0x0F) + 9;
            const uint8_t low = hex[i + 1] <= '9' ? hex[i + 1] - '0' : (hex[i + 1] & 0x0F) + 9;
            bytes[j] = (high << 4) | low;
        }

        return true;
    }

    bool compare(const uint8_t* buf1, const uint8_t* buf2, const size_t size)
    {
        if (buf1 == nullptr || buf2 == nullptr)
        {
            return false;
        }

        for (size_t i = 0; i < size; ++i)
        {
            if (buf1[i] != buf2[i])
            {
                return false;
            }
        }
        return true;
    }

    uint16_t byte_swap(const uint16_t value)
    {
        return (value << 8) | (value >> 8);
    }
}
