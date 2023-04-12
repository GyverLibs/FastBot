#include "utils.h"

void FB_escHTML(String& s) {
    String out;
    out.reserve(s.length());
    for (uint16_t i = 0; i < s.length(); i++) {
        switch (s[i]) {
        case '<': out += F("&lt;"); break;
        case '>': out += F("&gt;"); break;
        case '&': out += F("&amp;"); break;
        default: out += s[i]; break;
        }
    }
    s = out;
}

static const char FB_escList[] = ">-={}.!";

void FB_escMarkdown(String& s) {
    String out;
    out.reserve(s.length());
    for (uint16_t i = 0; i < s.length(); i++) {
        if (strchr(FB_escList, s[i]) != 0) out += '\\';
        out += s[i];
    }
    Serial.println(out);
    s = out;
}

int64_t FB_str64(const String &s) {
    return atoll(s.c_str());
}
String FB_64str(int64_t id) {
    String s;
    int32_t s1 = (int64_t)id % 1000000000;
    int32_t s2 = (int64_t)id / 1000000000;
    if (s2) {
        s += s2;
        s += abs(s1);
    } else {
        s += s1;
    }
    return s;
}

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
                uBytes = strtol(uStr.substring(i + 1, i + 5).c_str(), NULL, HEX);
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