#include "utils.h"

// упрощённый urlencode (до 38 ASCII + space)
// по хорошему нужно пропускать только a-Z, 0-9 и -_.!~*'()
void FB_urlencode(const String& s, String& dest) {
    dest.reserve(s.length() + 10);
    char c;
    for (uint16_t i = 0; i < s.length(); i++) {
        c = s[i];
        if (c == ' ') dest += '+';
        else if (c <= 38 || c == '+') {
            dest += '%';
            dest += (char)((c >> 4) + (((c >> 4) > 9) ? 87 : 48));
            dest += (char)((c & 0xF) + (((c & 0xF) > 9) ? 87 : 48));
        }
        else dest += c;
    }
}

// разработано Глебом Жуковым, допилено AlexGyver
// https://vk.com/wall-97877471_783011
void FB_unicode(String &uStr) {
    if (!uStr.length()) return;
    String out;
    out.reserve(uStr.length() / 3);
    int32_t uBytes, buf = 0;
    char x0[] = "\0x";
    for (uint16_t i = 0; i < uStr.length(); i++) {
        if (uStr[i] != '\\') out += uStr[i];
        else {
            switch (uStr[++i]) {
            case '0': break;
            case 'n': out += '\n'; break;
            case 'r': out += '\r'; break;
            case 't': out += '\t'; break;
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
                default: out += uStr[i]; break;
            }
        }
    }
    uStr = out;
}