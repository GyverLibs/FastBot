/*
    Очень простая и быстрая библиотека для телеграм бота на esp8266/esp32
    Документация: 
    GitHub: https://github.com/GyverLibs/FastBot
    Возможности:
    - Работает на стандартных библиотеках
    - Работает без SSL
    - Оптимизирована для большой нагрузки
    - Опциональная установка ID чата для общения с ботом
    - Проверка обновлений вручную или по таймеру
    - Отправка/редактирование/ответ на сообщения
    - Вывод меню вместо клавиатуры
    - Вывод инлайн меню в сообщении
    - Поддержка Unicode (другие языки + эмодзи)
    
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
    
    v2.1: 
        - Ещё оптимизация
        - Добавил форматирование текста (markdown, html)
        - Добавил ответ на сообщение
    
    v2.2: 
        - Большая оптимизация памяти
        - Добавил notify() - уведомления от сообщений бота
        - Добавил единоразовый показ клавиатуры
        
    v2.3: Небольшая оптимизация
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

#define FB_TEXT 0
#define FB_MARKDOWN 1
#define FB_HTML 2

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
    String& usrID;      // ID юзера
    String& first_name; // имя
    String& username;   // ник
    String& chatID;     // ID чата
    String& text;       // текст
    bool query;         // запрос
};

// ================================
class FastBot {
public:
    // инициализация (токен, макс кол-во сообщений на запрос, макс символов, период)
    FastBot(String token = "", uint16_t limit = 10, uint16_t ovf = 10000, uint16_t period = 1000) {
        _token.reserve(50);
        chatIDs.reserve(10);
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
    
    // режим обработки текста: FB_TEXT, FB_MARKDOWN, FB_HTML
    void setTextMode(uint8_t mode) {
        parseMode = mode;
    }
    
    // true/false вкл/выкл уведомления от сообщений бота (по умолч. вкл)
    void notify(bool mode) {
        notif = mode;
    }
    
    // ===================== ТИКЕР =====================
    // ручная проверка обновлений
    uint8_t tickManual() {
        uint8_t status = 1;
        String req;
        _addToken(req);
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
    
    // ===================== ОТПРАВКА =====================
    // отправить сообщение
    uint8_t sendMessage(const String& msg) {
        return sendMessage(msg, chatIDs);
    }

    uint8_t sendMessage(const String& msg, const String& id) {
        return replyMessage(msg, 0, id);
    }
    
    // ответить на сообщение
    uint8_t replyMessage(const String& msg, int32_t replyID) {
        return replyMessage(msg, replyID, chatIDs);
    }

    uint8_t replyMessage(const String& msg, int32_t replyID, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        _addMessage(req, msg);
        if (replyID) {
            req += F("&reply_to_message_id=");
            req += replyID;
        }
        return sendRequest(req, id);
    }
    
    // ==================== УДАЛЕНИЕ =====================
    // удалить сообщение со смещением offset
    uint8_t deleteMessage(int32_t offset) {
        return deleteMessageID(_lastUsrMsg - offset, chatIDs);
    }
    
    uint8_t deleteMessage(int32_t offset, const String& id) {
        return deleteMessageID(_lastUsrMsg - offset, id);
    }
    
    // удалить сообщение id
    uint8_t deleteMessageID(int32_t msgid) {
        return deleteMessageID(msgid, chatIDs);
    }
    
    uint8_t deleteMessageID(int32_t msgid, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/deleteMessage?");
        _addMsgID(req, msgid);
        return sendRequest(req, id);
    }
    
    // ==================== РЕДАКТИРОВАНИЕ =====================
    // редактировать сообщение со смещением offset
    uint8_t editMessage(int32_t offset, const String& text) {
        return editMessageID(_lastUsrMsg - offset, text, chatIDs);
    }
    
    uint8_t editMessage(int32_t offset, const String& text, const String& id) {
        return editMessageID(_lastUsrMsg - offset, text, id);
    }
    
    // редактировать сообщение id
    uint8_t editMessageID(int32_t msgid, const String& text) {
        return editMessageID(msgid, text, chatIDs);
    }

    uint8_t editMessageID(int32_t msgid, const String& text, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/editMessageText?");
        _addMsgID(req, msgid);
        _addText(req, text);
        return sendRequest(req, id);
    }
    
    // ================ РЕДАКТИРОВАНИЕ ИНЛАЙН =================
    // редактировать меню id
    uint8_t editMenuID(int32_t msgid, const String& str, const String& cbck) {
        return editMenuID(msgid, str, cbck, chatIDs);
    }
    
    uint8_t editMenuID(int32_t msgid, const String& str, const String& cbck, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/editMessageReplyMarkup?");
        _addMsgID(req, msgid);
        _addInlineMenu(req, str, cbck);
        return sendRequest(req, id);
    }
    
    // ответить на callback текстом и true - предупреждением
    uint8_t answer(const String& text, bool alert = false) {
        if (!_query) return 5;
        String req;
        _addToken(req);
        req += F("/answerCallbackQuery?callback_query_id=");
        req += *_query;
        _addText(req, text);
        if (alert) req += F("&show_alert=true");
        return sendRequest(req);
    }
    
    // ===================== МЕНЮ =====================
    // показать меню
    uint8_t showMenu(const String& str, bool once = false) {
        return showMenuText(F("Open Menu"), str, chatIDs, once);
    }
    
    uint8_t showMenu(const String& str, const String& id, bool once = false) {
        return showMenuText(F("Open Menu"), str, id, once);
    }
    
    // показать меню с текстом
    uint8_t showMenuText(const String& msg, const String& str, bool once = false) {
        return showMenuText(msg, str, chatIDs, once);
    }
    
    uint8_t showMenuText(const String& msg, const String& str, const String& id, bool once = false) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        _addMessage(req, msg);
        req += F("&reply_markup={\"keyboard\":[[\"");
        FB_Parser t;
        while (t.parseNT(str)) {
            req += t.str;
            if (t.div == '\t') req += F("\",\"");
            else if (t.div == '\n') req += F("\"],[\"");
        }
        req += F("\"]],\"resize_keyboard\":true");
        if (once) req += F(",\"one_time_keyboard\":true");
        req += '}';
        return sendRequest(req, id);
    }
    
    // закрыть меню
    uint8_t closeMenu() {
        return closeMenuText(F("Close Menu"), chatIDs);
    }
    
    uint8_t closeMenu(const String& id) {
        return closeMenuText(F("Close Menu"), id);
    }
    
    // закрыть меню с текстом
    uint8_t closeMenuText(const String& msg) {
        return closeMenuText(msg, chatIDs);
    }
    
    uint8_t closeMenuText(const String& msg, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        _addMessage(req, msg);
        req += F("&reply_markup={\"remove_keyboard\":true}");
        return sendRequest(req, id);
    }
    
    // =================== ИНЛАЙН МЕНЮ ===================
    // показать инлайн меню
    uint8_t inlineMenu(const String& msg, const String& str) {
        return inlineMenuCallback(msg, str, "", chatIDs);
    }
    
    uint8_t inlineMenu(const String& msg, const String& str, const String& id) {
        return inlineMenuCallback(msg, str, "", id);
    }
    
    // показать инлайн меню с коллбэками
    uint8_t inlineMenuCallback(const String& msg, const String& str, const String& cbck) {
        return inlineMenuCallback(msg, str, cbck, chatIDs);
    }
    
    uint8_t inlineMenuCallback(const String& msg, const String& str, const String& cbck, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        _addMessage(req, msg);
        _addInlineMenu(req, str, cbck);
        return sendRequest(req, id);
    }
    
    // ===================== ВСЯКОЕ =====================
    // отправить запрос
    uint8_t sendRequest(String& req, const String& id) {
        if (id.indexOf(',') < 0) {
            _addChatID(req, id);
            return sendRequest(req);
        }
        uint16_t endpos = req.length();
        FB_Parser ids;
        uint8_t status = 5;
        while (ids.parse(id)) {
            req.remove(endpos);
            _addChatID(req, ids.str);
            status = sendRequest(req);
        }
        return status;
    }
    
    uint8_t sendRequest(String& req) {
        uint8_t status = 1;
        if (_FB_httpReq.begin(_FB_client, req)) {
            if (_FB_httpReq.GET() != HTTP_CODE_OK) status = 3;
            String msgID;
            const String& answ = _FB_httpReq.getString();
            int16_t st = 0, end;
            find(answ, msgID, st, end, F("\"message_id\":"), F(",\""), 13, answ.length());
            _lastBotMsg = msgID.toInt();
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
    // ================ BUILDER ===============
    void _addToken(String& s) {
        s.reserve(150);
        s += F("https://api.telegram.org/bot");
        s += _token;
    }
    void _addMsgID(String& s, const int32_t& id) {
        s += F("&message_id=");
        s += id;
    }
    void _addChatID(String& s, const String& id) {
        s += F("&chat_id=");
        s += id;
    }
    void _addMessage(String& req, const String& text) {
        req += F("/sendMessage?");
        _addText(req, text);
        if (!notif) req += F("&disable_notification=true");
    }
    void _addText(String& s, const String& text) {
        s += F("&text=");
        s += text;
        if (parseMode == FB_MARKDOWN) s += F("&parse_mode=MarkdownV2");
        else if (parseMode == FB_HTML) s += F("&parse_mode=HTML");
    }
    
    void _addInlineMenu(String& req, const String& str, const String& cbck) {
        req += F("&reply_markup={\"inline_keyboard\":[[{");
        FB_Parser t, cb;
        while (t.parseNT(str)) {
            if (cbck.length() > 0) {
                cb.parse(cbck);
                _addInlineButton(req, t.str, cb.str);
            } else _addInlineButton(req, t.str, t.str);
            if (t.div == '\t') req += F("},{");
            else if (t.div == '\n') req += F("}],[{");
        }
        req += F("}]]}");
    }
    
    void _addInlineButton(String& str, const String& text, const String& cbck) {
        str += F("\"text\":\"");
        str += text;
        str += F("\",\"callback_data\":\"");
        str += cbck;
        str += '\"';
    }
    
    // ======================== PARSING =========================
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
                if (chatIDs.length() > 0 && chatIDs.indexOf(chatID) < 0) continue;  // проверка на ID чата
                
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

                FB_msg message = (FB_msg){_lastUsrMsg, usrID, first_name, username, chatID, text, (query.length() > 0)};
                if (*_callback2) _callback2(message);
                if (*_callback) _callback(username, text);
                _query = nullptr;
                yield();
            } else break;   // IDpos > 0
        }
        if (_incr) ID += counter;
        return 1;
    }

    void (*_callback)(String& name, String& text) = nullptr;
    void (*_callback2)(FB_msg& msg) = nullptr;
    String _token;
    String* _query = nullptr;
    uint16_t _ovf = 10000, prd = 1000, _limit = 10;
    int32_t ID = 0;
    uint32_t tmr = 0;
    bool _incr = true;
    int32_t _lastUsrMsg = 0, _lastBotMsg = 0;
    uint8_t parseMode = 0;
    bool notif = true;
};
#endif