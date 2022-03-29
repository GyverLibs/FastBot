#include "utils.h"
// разработано Глебом Жуковым, допилено AlexGyver
// https://vk.com/wall-97877471_783011
void FB_unicode(String &uStr) {
    String out;
    out.reserve(uStr.length() / 3);
    int32_t uBytes, buf = 0;
    char x0[] = "\0x";
    for (uint16_t i = 0; i < uStr.length(); i++) {
        if (uStr[i] != '\\') out += uStr[i];
        else {
            switch (uStr[++i]) {
            case '/': out += uStr[i]; break;
            case 'n': out += '\n'; break;
            case 'u':
                uBytes = strtol(uStr.c_str() + i + 1, NULL, HEX);
                i += 4;
                if ((uBytes >= 0xD800) && (uBytes <= 0xDBFF)) buf = uBytes;
                else if ((uBytes >= 0xDC00) && (uBytes <= 0xDFFF)) {
                    uBytes = (0x10000 + ((buf - 0xD800) * 0x0400) + (uBytes - 0xDC00));
                    out += (char)(0b11110000 | ((uBytes >> 18) & 0b111));
                    out += x0;
                    out += (char)(0b10000000 | ((uBytes >> 12) & 0b111111));
                    out += x0;
                    out += (char)(0b10000000 | ((uBytes >> 6) & 0b111111));
                    out += x0;
                    out += (char)(0b10000000 | (uBytes & 0b111111));
                } else if (uBytes < 0x800) {
                    out += (char)(0b11000000 | ((uBytes >> 6) & 0b11111));
                    out += x0;
                    out += (char)(0b10000000 | (uBytes & 0b111111));
                } else if (uBytes >= 0x800) {
                    out += (char)(0b11100000 | ((uBytes >> 12) & 0b1111));
                    out += x0;
                    out += (char)(0b10000000 | ((uBytes >> 6) & 0b111111));
                    out += x0;
                    out += (char)(0b10000000 | (uBytes & 0b111111));
                }
                break;
            }
        }
    }
    uStr = out;
}