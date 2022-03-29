#pragma once
#include <Arduino.h>
void FB_unicode(String &uStr);

struct FB_Parser {
public:
    FB_Parser(const String& str) : source(&str) {}
    bool parse() {
        if (to == (int)source -> length()) return 0;
        to = source->indexOf(',', from);
        if (to < 0) to = source -> length();
        str = source -> substring(from, to);
        str.trim();
        from = to + 1;
        return 1;    
    }
    int from = 0, to = 0;
    const String* source;
    String str;
};