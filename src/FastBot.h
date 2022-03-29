/*
    Очень простая и быстрая библиотека для телеграм бота
    Документация: 
    GitHub: https://github.com/GyverLibs/FastBot
    Возможности:
    - Оптимизирована для большой нагрузки (спокойно принимает 50 сообщ в секунду)
    - Опциональная установка ID чата для общения с ботом
    - Проверка обновлений вручную или по таймеру
    - Сообщения приходят в функцию-обработчик
    - Отправка сообщений в чат
    - Вывод меню вместо клавиатуры
    - Вывод инлайн меню в сообщении
    - Возможность включить ручной инкремент новых сообщений
    - Работает без SSL
    - Поддержка Unicode (другие языки + эмодзи)
    - Работает на стандартных библиотеках
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - оптимизация
    v1.2 - можно задать несколько chatID и отправлять в указанный чат
    v1.3 - добавлена возможность задать текст при открытии и закрытии меню
    v1.3.1 - исправлены ошибки с 1.3
    v1.4 - добавлена возможность удалять сообщения
    v1.5 - оптимизация, возможность смены токена, новый парсинг сообщений (id, имя, текст)
    v1.5.1 - получаем также ID сообщения
    v1.6 - добавлен режим FB_DYNAMIC_HTTP, чтение имени пользователя
    v1.7:
        - Убрал динамический режим, работает слишком медленно
        - Исправил warningи
        - Починил работу бота в "группах" (отрицательный ID чата)
        - Оптимизация памяти
        - Ускорил работу
        - Пофиксил работу через раз в сценарии "эхо"
    v2.0:
        - Убрал минимум в 3200 мс
        - Добавил обработку Юникода (русский язык, эмодзи). Спасибо Глебу Жукову!
        - Из меню удаляются лишние пробелы, работать стало проще
        - Поддержка esp32
        - Большая оптимизация
        - Добавил коллбэки в inlineMenu
        - Добавил ID юзера
        - Добавил query для ответа на коллбэк
        - Добавил редактирование сообщений и кучу всего
*/

/*
Статусы tick:
    0 - ожидание
    1 - ОК
    2 - Переполнен по ovf
    3 - Ошибка телеграм
    4 - Ошибка подключения
    5 - не задан chat ID
    6 - множественная отправка, статус неизвестен
*/

#ifndef _FastBot_h
#define _FastBot_h

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
static BearSSL::WiFiClientSecure _FB_client;
static HTTPClient _FB_httpGet, _FB_httpReq;
#else
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
WiFiClientSecure _FB_client;
static HTTPClient _FB_httpGet;
#define _FB_httpReq _FB_httpGet
#endif

#include "utils.h"

struct FB_msg {
    int32_t ID;         // ID сообщения
    int32_t usrID;      // ID юзера
    String& first_name; // имя
    String& username;   // ник
    int32_t chatID;     // ID чата
    String& text;       // текст
    bool query;         // запрос
};

// ================================
class FastBot {
public:
    // инициализация (токен, макс кол-во сообщений на запрос, макс символов, период)
    FastBot(String token = "", uint16_t limit = 10, uint16_t ovf = 10000, uint16_t period = 1000) {
        _token = token;
        _ovf = ovf;
        _limit = limit;
        prd = period;
        setBufferSizes(512, 512);
        _FB_client.setInsecure();
    }
    
    // ===================== НАСТРОЙКИ =====================
    // установить размеры буфера на приём и отправку
    void setBufferSizes(uint16_t rx, uint16_t tx) {
        #ifdef ESP8266
        _FB_client.setBufferSizes(rx, tx);
        #endif
    }
    
    // макс кол-во сообщений на запрос
    void setLimit(int limit) {
        _limit = limit;
    }
    
    // макс символов
    void setOvf(int ovf) {
        _ovf = ovf;
    }
    
    // период опроса
    void setPeriod(int period) {
        prd = period;
    }
    
    // установка ID чата для парсинга сообщений. Можно ввести через запятую сколько угодно
    void setChatID(String chatID) {
        chatIDs = chatID;        
    }
    
    void setToken(String token) {
        _token = token;
    }

    // подключение обработчика сообщений
    void attach(void (*handler)(String&, String&)) {
        _callback = handler;
    }
    
    void attach(void (*handler)(FB_msg& msg)) {
        _callback2 = handler;
    }
    
    // отключение обработчика сообщений
    void detach() {
        _callback = nullptr;
        _callback2 = nullptr;
    }
    
    // ===================== ТИКЕР =====================
    // ручная проверка обновлений
    uint8_t tickManual() {
        uint8_t status = 1;
        String req;
        req.reserve(100);
        req += F("https://api.telegram.org/bot");
        req += _token;
        req += F("/getUpdates?limit=");
        req += _limit;
        req += F("&offset=");
        req += ID;
        if (_FB_httpGet.begin(_FB_client, req)) {
            if (_FB_httpGet.GET() == HTTP_CODE_OK) status = parse(_FB_httpGet.getString(), _FB_httpGet.getSize());
            else status = 3;
            _FB_httpGet.end();
        } else status = 4;
        return status;
    }
    
    // проверка обновлений по таймеру
    uint8_t tick() {
        if (millis() - tmr >= prd) {
            tmr = millis();
            return tickManual();
        }
        return 0;
    }
    
    // ===================== СООБЩЕНИЯ =====================
    // ID последнего отправленного ботом сообщения
    int32_t lastBotMsg() {
        return _lastBotMsg;
    }
    
    // ID последнего отправленного юзером сообщения
    int32_t lastUsrMsg() {
        return _lastUsrMsg;
    }
    
    // отправить сообщение в указанный в setChatID чат/чаты
    uint8_t sendMessage(String msg) {
        return sendMessage(msg, chatIDs);
    }
    
    // отправить сообщение в указанный здесь чат/чаты
    uint8_t sendMessage(String msg, String id) {
        if (!id.length()) return 5;                                 // не задан ID чата
        if (id.indexOf(',') < 0) return _sendMessage(msg, id);   // один ID
        FB_Parser ids(id);
        while (ids.parse()) _sendMessage(msg, ids.str);
        return 6;
    }
    
    // ==================== УДАЛЕНИЕ =====================
    // удалить сообщение со смещением offset в указанном в setChatID чате
    uint8_t deleteMessage(int32_t offset) {
        return deleteMessage(offset, chatIDs);
    }
    
    // удалить сообщение со смещением offset в указанном чате
    uint8_t deleteMessage(int32_t offset, String id) {
        if (!id.length()) return 5;                              // не задан ID чата
        return _deleteMessage(_lastUsrMsg - offset, id.toInt()); // один ID
    }
    uint8_t deleteMessage(int32_t offset, int32_t id) {
        return _deleteMessage(_lastUsrMsg - offset, id); // один ID
    }
    
    // удалить сообщение id в указанном в setChatID чате
    uint8_t deleteMessageID(int32_t msgid) {
        return deleteMessage(msgid, chatIDs);
    }
    
    // удалить сообщение id в указанном чате
    uint8_t deleteMessageID(int32_t msgid, String id) {
        if (!id.length()) return 5;                              // не задан ID чата
        return _deleteMessage(msgid, id.toInt());
    }
    uint8_t deleteMessageID(int32_t msgid, int32_t id) {
        return _deleteMessage(msgid, id);
    }
    
    // ==================== РЕДАКТИРОВАНИЕ =====================
    // редактировать сообщение со смещением offset в указанном в setChatID чате
    uint8_t editMessage(int32_t offset, String text) {
        return editMessage(offset, text, chatIDs);
    }
    
    // редактировать сообщение со смещением offset в указанном чате
    uint8_t editMessage(int32_t offset, String text, String id) {
        if (!id.length()) return 5;
        return _editMessage(_lastUsrMsg - offset, text, id.toInt());
    }
    uint8_t editMessage(int32_t offset, String text, int32_t id) {
        return _editMessage(_lastUsrMsg - offset, text, id); 
    }
    
    // редактировать сообщение id в указанном в setChatID чате
    uint8_t editMessageID(int32_t msgid, String text) {
        return editMessageID(msgid, text, chatIDs);
    }
    
    // редактировать сообщение id в указанном чате
    uint8_t editMessageID(int32_t msgid, String text, String id) {
        if (!id.length()) return 5;
        return _editMessage(msgid, text, id.toInt());
    }
    uint8_t editMessageID(int32_t msgid, String text, int32_t id) {
        return _editMessage(msgid, text, id);
    }
    
    // ================ РЕДАКТИРОВАНИЕ ИНЛАЙН =================
    // редактировать меню id в указанном в setChatID чате
    uint8_t editMenuID(int32_t msgid, String text, String cback) {
        return editMenuID(msgid, text, cback, chatIDs);
    }
    
    // редактировать меню id в указанном чате
    uint8_t editMenuID(int32_t msgid, String text, String cback, String id) {
        if (!id.length()) return 5;
        return _editMenu(msgid, text, cback, id.toInt());
    }
    uint8_t editMenuID(int32_t msgid, String text, String cback, int32_t id) {
        return _editMenu(msgid, text, cback, id);
    }
    
    // ответить на callback текстом и true - предупреждением
    void answer(String text, bool alert = false) {
        if (_query) {
            String req;
            req.reserve(100 + text.length());
            req += F("https://api.telegram.org/bot");
            req += _token;
            req += F("/answerCallbackQuery?callback_query_id=");
            req += *_query;
            req += F("&text=");
            req += text;
            if (alert) req += F("&show_alert=true");
            sendRequest(req);
        }
    }
    
    // ===================== МЕНЮ =====================
    // показать меню в указанном в setChatID чате/чатах
    uint8_t showMenu(String str) {
        return showMenu(str, chatIDs);
    }
    
    // показать меню в указанном здесь чате/чатах
    uint8_t showMenu(String str, String id) {
        String msg = F("Show Menu");
        if (!id.length()) return 5;                                 // не задан ID чата
        if (id.indexOf(',') < 0) return _showMenu(str, msg, id);    // один ID
        FB_Parser ids(id);
        while (ids.parse()) _showMenu(str, msg, ids.str);
        return 6;
    }
    
    // показать меню в указанном в setChatID чате/чатах с указанным текстом
    uint8_t showMenuText(String msg, String str) {
        return showMenuText(str, msg, chatIDs);
    }
    
    // показать меню в указанном здесь чате/чатах с указанным текстом
    uint8_t showMenuText(String msg, String str, String id) {
        if (!id.length()) return 5;                                 // не задан ID чата
        if (id.indexOf(',') < 0) return _showMenu(str, msg, id);    // один ID
        FB_Parser ids(id);
        while (ids.parse()) _showMenu(str, msg, ids.str);
        return 6;
    }
    
    // закрыть меню в указанном в setChatID чате/чатах
    uint8_t closeMenu() {
        return closeMenu(chatIDs);
    }
    
    // закрыть меню в указанном здесь чате/чатах
    uint8_t closeMenu(String id) {
        String msg = F("Close Menu");
        if (!id.length()) return 5;                             // не задан ID чата
        if (id.indexOf(',') < 0) return _closeMenu(msg, id);    // один ID
        FB_Parser ids(id);
        while (ids.parse()) _closeMenu(msg, ids.str);
        return 6;
    }
    
    // закрыть меню в указанном в setChatID чате/чатах с указанным текстом
    uint8_t closeMenuText(String msg) {
        return closeMenuText(msg, chatIDs);
    }
    
    // закрыть меню в указанном здесь чате/чатах с указанным текстом
    uint8_t closeMenuText(String msg, String id) {
        if (!id.length()) return 5;                             // не задан ID чата
        if (id.indexOf(',') < 0) return _closeMenu(msg, id);    // один ID
        FB_Parser ids(id);
        while (ids.parse()) _closeMenu(msg, ids.str);
        return 6;
    }
    
    // показать инлайн меню в указанном в setChatID чате/чатах
    uint8_t inlineMenu(String msg, String str) {
        return inlineMenu(msg, str, chatIDs);
    }
    
    // показать инлайн меню в указанном здесь чате/чатах
    uint8_t inlineMenu(String msg, String str, String id) {
        if (!id.length()) return 5;                                     // не задан ID чата
        String cb;
        if (id.indexOf(',') < 0) return _inlineMenu(msg, str, id, cb);  // один ID
        FB_Parser ids(id);
        while (ids.parse()) _inlineMenu(msg, str, ids.str, cb);
        return 6;
    }
    
    // показать инлайн меню с коллбэками в указанном в setChatID чате/чатах
    uint8_t inlineMenuCallback(String msg, String str, String cbacks) {
        return inlineMenuCallback(msg, str, cbacks, chatIDs);
    }
    
    // показать инлайн меню в указанном здесь чате/чатах
    uint8_t inlineMenuCallback(String msg, String str, String cbacks, String id) {
        if (!id.length()) return 5;
        if (id.indexOf(',') < 0) return _inlineMenu(msg, str, id, cbacks);  // один ID
        FB_Parser ids(id);
        while (ids.parse()) _inlineMenu(msg, str, ids.str, cbacks);
        return 6;
    }
    
    // ===================== ВСЯКОЕ =====================
    // отправить запрос
    uint8_t sendRequest(String& req) {
        uint8_t status = 1;
        if (_FB_httpReq.begin(_FB_client, req)) {
            if (_FB_httpReq.GET() != HTTP_CODE_OK) status = 3;
            const String* str = &_FB_httpReq.getString();
            int idx = str->indexOf("message_id");
            if (idx > 0 && idx < (int)str->length()) {
                int end = str->indexOf(",\"", idx);
                _lastBotMsg = str->substring(idx + 12, end).toInt();
            }
            _FB_httpReq.end();
        } else status = 4;
        return status;
    }
    
    // авто инкремент сообщений (по умолч включен)
    void autoIncrement(boolean incr) {
        _incr = incr;
    }
    
    // вручную инкрементировать ID
    void incrementID(uint8_t val) {
        if (_incr) ID += val;
    }
    
    // для непосредственного редактирования
    String chatIDs;
    
private:
    uint8_t _editMenu(int32_t msgid, String& str, String& cbck, int32_t id) {
        String req;
        req.reserve(100 + str.length());
        req += F("https://api.telegram.org/bot");
        req += _token;
        req += F("/editMessageReplyMarkup?chat_id=");
        req += id;
        req += F("&message_id=");
        req += msgid;
        _addInlineMenu(req, str, cbck);
        return sendRequest(req);
    }
    
    uint8_t _editMessage(int32_t msgid, String& text, int32_t id) {
        String req;
        req.reserve(100 + text.length());
        req += F("https://api.telegram.org/bot");
        req += _token;
        req += F("/editMessageText?chat_id=");
        req += id;
        req += F("&message_id=");
        req += msgid;
        req += F("&text=");
        req += text;
        return sendRequest(req);
    }
    
    uint8_t _deleteMessage(int32_t msgid, int32_t id) {
        String req;
        req.reserve(100);
        req += F("https://api.telegram.org/bot");
        req += _token;
        req += F("/deleteMessage?chat_id=");
        req += id;
        req += F("&message_id=");
        req += msgid;
        return sendRequest(req);
    }
    
    uint8_t _sendMessage(String& msg, String& id) {
        String req;
        req.reserve(100 + msg.length());
        req += F("https://api.telegram.org/bot");
        req += _token;
        req += F("/sendMessage?chat_id=");
        req += id;
        req += F("&text=");
        req += msg;
        return sendRequest(req);
    }
    
    uint8_t _showMenu(String& str, String& msg, String& id) {
        String req;
        req.reserve(100 + str.length());
        req += F("https://api.telegram.org/bot");
        req += _token;
        req += F("/sendMessage?chat_id=");
        req += id;
        req += F("&text=");
        req += msg;
        req += F("&reply_markup={\"keyboard\":[[\"");
        for (uint16_t i = 0; i < str.length(); i++) {
            char c = str[i];
            char c1 = str[i + 1];
            if (c == ' ' && (c1 == '\t' || c1 == '\n')) continue;
            if (c == '\t') {
                req += F("\",\"");
                if (c1 == ' ') i++;
            }
            else if (c == '\n') {
                req += F("\"],[\"");
                if (c1 == ' ') i++;
            }
            else req += c;
        }
        req += F("\"]],\"resize_keyboard\":true}");
        return sendRequest(req);
    }
    
    uint8_t _closeMenu(String& msg, String& id) {
        String req;
        req.reserve(100);
        req += F("https://api.telegram.org/bot");
        req += _token;
        req += F("/sendMessage?chat_id=");
        req += id;
        req += F("&text=");
        req += msg;
        req += F("&reply_markup={\"remove_keyboard\":true}");
        return sendRequest(req);
    }
    
    uint8_t _inlineMenu(String& msg, String& str, String& id, String& cback) {
        String req;
        req.reserve(100 + str.length());
        req += F("https://api.telegram.org/bot");
        req += _token;
        req += F("/sendMessage?chat_id=");
        req += id;
        req += F("&text=");
        req += msg;
        _addInlineMenu(req, str, cback);
        return sendRequest(req);
    }
    
    void _addInlineMenu(String& req, String& str, String& cback) {
        req += F("&reply_markup={\"inline_keyboard\":[[{");
        int start = 0;
        int mode, end;
        FB_Parser cbacks(cback);
        while (1) {
            int t = str.indexOf('\t', start);
            int n = str.indexOf('\n', start);
            if (n < 0 && t < 0) mode = 2, end = str.length();
            else if (n < 0) mode = 0, end = t;
            else if (t < 0) mode = 1, end = n;
            else mode = t > n, end = min(t, n);
            String s = str.substring(start, end);
            s.trim();
            if (cback.length() > 0) {
                cbacks.parse();
                addInlineButton(req, s, cbacks.str);
            } else addInlineButton(req, s, s);
            if (mode == 0) req += F("},{");
            else if (mode == 1) req += F("}],[{");
            if (mode == 2) break;
            start = end + 1;
        }
        req += F("}]]}");
    }
    
    void addInlineButton(String& str, String& msg, String& data) {
        str += F("\"text\":\"");
        str += msg;
        str += F("\",\"callback_data\":\"");
        str += data;
        str += '\"';
    }
    
    bool find(const String& str, String& dest, int16_t& start, int16_t& end, String from, String to, int16_t len, int16_t pos) {
        int strPos = str.indexOf(from, start);
        if (strPos < 0 || strPos > pos) return 0;
        start = strPos + len;
        end = str.indexOf(to, start);
        dest = str.substring(start, end);
        start = end;
        return 1;
    }

    uint8_t parse(const String& str, uint16_t size) {
        if (!str.startsWith(F("{\"ok\":true"))) return 3;       // ошибка запроса (неправильный токен итд)
        if (size > _ovf) {                                      // переполнен
            int IDpos = str.indexOf(F("{\"update_id\":"), 0);
            if (IDpos > 0) ID = str.substring(IDpos + 13, str.indexOf(',', IDpos)).toInt();
            ID++;
            return 2;
        }

        int16_t IDpos = str.indexOf(F("{\"update_id\":"), 0);   // первая позиция ключа update_id
        int16_t counter = 0;
        while (true) {
            if (IDpos > 0 && IDpos < (int16_t)str.length()) {           // если есть что разбирать
                if (ID == 0) ID = str.substring(IDpos + 13, str.indexOf(',', IDpos)).toInt() + 1;   // холодный запуск, ищем ID
                else counter++;                                                                     // иначе считаем пакеты
                int16_t textPos = IDpos;                                // стартовая позиция для поиска
                int16_t endPos;
                IDpos = str.indexOf(F("{\"update_id\":"), IDpos + 1);   // позиция id СЛЕДУЮЩЕГО обновления (мы всегда на шаг впереди)
                if (IDpos == -1) IDpos = str.length();                  // если конец пакета - для удобства считаем что позиция ID в конце
                
                String query;
                find(str, query, textPos, endPos, F("query\":{\"id\":\""), F(",\""), 14, IDpos);
                _query = &query;
                
                String message_id;
                if (!find(str, message_id, textPos, endPos, F("\"message_id\":"), F(",\""), 13, IDpos)) continue;
                _lastUsrMsg = message_id.toInt();
                
                String usrID;
                if (!find(str, usrID, textPos, endPos, F("\"id\":"), F(",\""), 5, IDpos)) continue;
                
                String first_name;
                find(str, first_name, textPos, endPos, F("\"first_name\":\""), F("\""), 14, IDpos);

                String username;
                find(str, username, textPos, endPos, F("\"username\":\""), F("\""), 12, IDpos);
                
                String chatID;
                if (!find(str, chatID, textPos, endPos, F("\"chat\":{\"id\":"), F(",\""), 13, IDpos)) continue;
                // установлена проверка на ID чата - проверяем соответствие, если что - выходим
                if (chatIDs.length() > 0 && chatIDs.indexOf(chatID) < 0) continue;
                
                String text;
                int dataPos = str.indexOf(F("\"data\":\""), textPos);
                if (dataPos < 0 || dataPos > IDpos) {
                    if (!find(str, text, textPos, endPos, F("\"text\":\""), F("\""), 8, IDpos)) continue;
                } else {
                    find(str, text, textPos, endPos, F("\"data\":\""), F("\""), 8, IDpos);
                }

                #ifndef FB_NO_UNICODE
                FB_unicode(first_name);
                FB_unicode(username);
                FB_unicode(text);
                #endif

                FB_msg message = (FB_msg){message_id.toInt(), usrID.toInt(), first_name, username, chatID.toInt(), text, (query.length() > 0)};
                if (*_callback2) _callback2(message);
                if (*_callback) _callback(username, text);
                _query = nullptr;
            } else break;   // IDpos > 0
        }
        if (_incr) ID += counter;
        return 1;
    }

    void (*_callback)(String& name, String& text) = nullptr;
    void (*_callback2)(FB_msg& msg) = nullptr;
    String _token = "";
    String* _query = nullptr;
    uint16_t _ovf = 10000, prd = 1000, _limit = 10;
    long ID = 0;
    uint32_t tmr = 0;    
    bool _incr = true;
    int32_t _lastUsrMsg = 0, _lastBotMsg = 0;
};
#endif