#pragma once
#include <Arduino.h>
void FB_unicode(String &uStr);
void FB_urlencode(const String& s, String& dest);

struct FB_msg {
    int32_t ID;         // ID сообщения
    String& usrID;      // ID юзера
    String& first_name; // имя
    String& last_name;  // фамилия
    String& username;   // ник
    String& chatID;     // ID чата
    String& text;       // текст
    bool query;         // запрос
    bool& edited;       // сообщение отредактировано
    bool& isBot;        // сообщение от бота
    uint32_t unix;      // время сообщения
};

struct FB_Time {
    FB_Time(uint32_t unix = 0, int16_t gmt = 0) {
        if (!unix) return;
        if (abs(gmt) <= 12) gmt *= 60;
        unix += gmt * 60L;
        second = unix % 60ul;
        unix /= 60ul;
        minute = unix % 60ul;
        unix /= 60ul;
        hour = unix % 24ul;
        unix /= 24ul;
        dayWeek = (unix + 4) % 7;
        if (!dayWeek) dayWeek = 7;
        unix += 719468;
        uint8_t era = unix / 146097ul;
        uint16_t doe = unix - era * 146097ul;
        uint16_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
        year = yoe + era * 400;
        uint16_t doy = doe - (yoe * 365 + yoe / 4 - yoe / 100);
        uint16_t mp = (doy * 5 + 2) / 153;
        day = doy - (mp * 153 + 2) / 5 + 1;
        month = mp + (mp < 10 ? 3 : -9);
        year += (month <= 2);
    }
    
    uint8_t second = 0;
    uint8_t minute = 0;
    uint8_t hour = 0;
    uint8_t day = 0;
    uint8_t month = 0;
    uint8_t dayWeek = 0;
    uint16_t year = 0;
    
    // получить строку времени формата ЧЧ:ММ:СС
    String timeString() {
        String str;
        if (!year) return str = F("Not sync");
        str.reserve(8);
        if (hour < 10) str += '0';
        str += hour;
        str += ':';
        if (minute < 10) str += '0';
        str += minute;
        str += ':';
        if (second < 10) str += '0';
        str += second;
        return str;
    }
    
    // получить строку даты формата ДД.ММ.ГГГГ
    String dateString() {
        String str;
        if (!year) return str = F("Not sync");
        str.reserve(10);
        if (day < 10) str += '0';
        str += day;
        str += '.';
        if (month < 10) str += '0';
        str += month;
        str += '.';
        str += year;    
        return str;
    }
};

struct FB_Parser {
    FB_Parser() {
      str.reserve(20);
    }
    
    bool parseNT(const String& s) {
      while (!end) {
        char c1 = s[++i];
        if (c1 == '\t' || c1 == '\n' || c1 == '\0') {
          int to = i;
          if (s[to - 1] == ' ') to--;
          if (s[from] == ' ') from++;
          str = s.substring(from, to);
          from = i + 1;
          end = (c1 == '\0');
          div = c1;
          return 1;
        }
      }
      return 0;
    }

    bool parse(const String& s) {
        if (i == (int)s.length()) return 0;
        i = s.indexOf(',', from);
        if (i < 0) i = s.length();
        int to = i;
        if (s[to - 1] == ' ') to--;
        if (s[from] == ' ') from++;
        str = s.substring(from, to);
        from = i + 1;
        return 1;    
    }
    
    int i = 0, from = 0;
    char div;
    bool end = false;
    String str;
};