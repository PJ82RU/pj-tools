#include "bytes.h"

namespace tools
{
    String bytes2hex(const uint8_t* bytes, const size_t size, const bool upper_case)
    {
        String res = "";
        if (bytes && size != 0)
        {
            for (int i = 0; i < size; ++i)
            {
                auto buf = String(bytes[i], HEX);
                res += (buf.length() != 2 ? "0" : "") + buf;
            }
            if (upper_case) res.toUpperCase();
        }
        return res;
    }

    bool hex2bytes(String& hex, uint8_t* bytes, const size_t size)
    {
        const auto hexlen = hex.length();
        const bool result = hexlen != 0 && hexlen % 2 == 0 && bytes && size != 0;
        if (result)
        {
            const size_t len = hexlen / 2;
            for (int i = 0, j = 0; j < size; i += 2, j++)
            {
                bytes[j] = j < len ? (hex[i] % 32 + 9) % 25 * 16 + (hex[i + 1] % 32 + 9) % 25 : 0;
            }
        }
        return result;
    }

    long byte2hex(const uint8_t byte)
    {
        uint8_t buf[1] = {byte};
        String res = bytes2hex(buf, 1);
        const char* it = res.begin();
        while (it != res.end() && std::isdigit(*it)) ++it;
        return it == res.end() ? res.toInt() : 0;
    }

    bool compare(const uint8_t* buf1, const uint8_t* buf2, const size_t size)
    {
        for (int i = 0; i < size; ++i)
        {
            if (buf1[i] != buf2[i]) return false;
        }
        return true;
    }

    uint16_t byte_swap(const uint16_t value)
    {
        const uint16_t b_high = (value >> 8) & 0xff;
        const uint16_t b_low = value & 0xff;
        return (b_low << 8) | b_high;
    }
}
