#pragma once
#include <Arduino.h>

enum FB_FileType {
    FB_PHOTO,
    FB_AUDIO,
    FB_DOC,
    FB_VIDEO,
    FB_GIF,
    FB_VOICE,
};

struct FB_msg {
    String& userID;     // ID юзера
    String& username;   // ник (в API это first_name)
    String& chatID;     // ID чата
    int32_t messageID;  // ID сообщения
    String& text;       // текст
    String& data;       // callback дата
    bool query;         // запрос
    bool& edited;       // сообщение отредактировано
    bool isBot;         // сообщение от бота
    bool OTA;           // запрос на OTA обновление
    uint32_t unix;      // время сообщения
    String& fileName;   // имя файла
    String& replyText;  // текст ответа, если он есть
    bool isFile;        // это файл
    String& fileUrl;    // адрес файла для загрузки
    
    // legacy
    String& usrID;      // ID юзера
    String& first_name; // имя
    String& last_name;  // фамилия
    int32_t ID;         // ID сообщения
    
    // вся информация одной строкой
    String toString() {
        String s;
        s.reserve(200);
        s += F("userID: ");
        s += userID;
        s += F(", username: ");
        s += username;
        s += F(", isBot: ");
        s += isBot;
        s += '\n';
        
        s += F("chatID: ");
        s += chatID;
        s += F(", messageID: ");
        s += messageID;
        s += F(", edited: ");
        s += edited;
        s += '\n';
        
        s += F("text: ");
        s += text;
        s += F(", replyText: ");
        s += replyText;
        s += '\n';
        
        s += F("query: ");
        s += query;
        s += F(", data: ");
        s += data;
        s += '\n';
        
        s += F("isFile: ");
        s += isFile;
        s += F(", fileName: ");
        s += fileName;
        s += F(", OTA: ");
        s += OTA;
        s += '\n';
        
        s += F("fileUrl: ");
        s += fileUrl;
        s += '\n';
        
        s += F("unix: ");
        s += unix;
        s += '\n';
        return s;
    }
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