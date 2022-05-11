/*
    Очень простая и быстрая библиотека для телеграм бота на esp8266/esp32
    Документация: 
    GitHub: https://github.com/GyverLibs/FastBot
    Возможности:
    - Работает на стандартных библиотеках
    - Работает без SSL
    - Оптимизирована для большой нагрузки
    - Опциональный белый список ID чатов
    - Проверка обновлений вручную или по таймеру
    - Отправка/удаление/редактирование/ответ на сообщения
    - Отправка стикеров
    - Вывод меню и инлайн меню (с поддержкой ссылок)
    - Изменение названия и описания чата
    - Закрепление/открепление сообщений
    - Поддержка Unicode (другие языки + эмодзи) для входящих сообщений
    - Встроенный urlencode для исходящих сообщений
    - Встроенные часы реального времени с синхронизацией от сервера Telegram
    
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
    v2.4: Добавил url encode для текста сообщений
    v2.5: Добавил флаги в FB_msg: сообщение отредактировано и сообщение отправлено ботом. Улучшил парсинг текста
    v2.6: Добавил встроенные часы реального времени
    v2.7: Добавил отправку стикеров
    v2.8: Убрал лишний вывод в сериал, GMT можно в минутах
    v2.9: Исправлена бага в парсинге, парсинг ускорен, добавлен вывод форматированного времени, добавлена фамилия и время сообщения
    v2.10: Добавлены функции для изменения названия и описания чата, закрепления и открепления сообщений. Убраны edit/deleteMessageID, editMenuID
    v2.11: 
        - Оптимизация, исправление багов
        - Callback data теперь парсится отдельно в data
        - Переделана работа с callback
        - Добавлен toString() для FB_msg для отладки
        - В callback добавлена обработка url адресов
        - Убраны first_name и last_name (с сохранением легаси)
        - usrID и ID переименованы в userID и messageID (с сохранением легаси)
        - Окончательно убран старый обработчик входящих сообщений
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
    7 - не подключен обработчик
*/

#ifndef _FastBot_h
#define _FastBot_h

#define FB_TEXT 0
#define FB_MARKDOWN 1
#define FB_HTML 2

#define FB_NOTIF 0
#define FB_ALERT 1

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

// ================================
class FastBot {
public:
    // инициализация (токен, макс кол-во сообщений на запрос, макс символов, период)
    FastBot(String token = "", uint16_t limit = 10, uint16_t ovf = 10000, uint16_t period = 3600) {
        _token.reserve(46);
        chatIDs.reserve(10);
        _token = token;
        _ovf = ovf;
        _limit = limit;
        _prd = period;
        setBufferSizes(512, 512);
        _FB_client.setInsecure();
        //_FB_httpGet.setTimeout(1000);
    }
    
    // ===================== НАСТРОЙКИ =====================
    // установить размеры буфера на приём и отправку
    void setBufferSizes(uint16_t rx, uint16_t tx) {
        #ifdef ESP8266
        _FB_client.setBufferSizes(rx, tx);
        #endif
    }
    
    // макс кол-во сообщений на запрос
    void setLimit(uint16_t limit) {
        _limit = limit;
    }
    
    // макс символов
    void setOvf(uint16_t ovf) {
        _ovf = ovf;
    }
    
    // период опроса
    void setPeriod(uint16_t period) {
        _prd = period;
    }
    
    // установка ID чата для парсинга сообщений. Можно ввести через запятую сколько угодно
    void setChatID(const String& chatID) {
        chatIDs = chatID;        
    }
    
    void setToken(const String& token) {
        _token = token;
    }

    // подключение обработчика сообщений
    void attach(void (*handler)(FB_msg& msg)) {
        _callback = handler;
    }
    
    // отключение обработчика сообщений
    void detach() {
        _callback = nullptr;
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
        if (!*_callback) return 7;
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
        if (!*_callback) return 7;
        if (millis() - tmr >= _prd) {
            uint8_t stat = tickManual();
            tmr = millis();
            return stat;
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
    // отправить команду API (пример: "/sendSticker?sticker=123456")
    uint8_t sendCommand(const String& cmd, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += cmd;
        return sendRequest(req, id);
    }
    
    uint8_t sendCommand(const String& cmd) {
        return sendCommand(cmd, chatIDs);
    }
    
    // отправить стикер
    uint8_t sendSticker(const String& sid) {
        return sendSticker(sid, chatIDs);
    }

    uint8_t sendSticker(const String& sid, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/sendSticker?sticker=");
        req += sid;
        if (!notif) req += F("&disable_notification=true");
        return sendRequest(req, id);
    }
    
    // отправить сообщение
    uint8_t sendMessage(const String& msg) {
        return replyMessage(msg, 0, chatIDs);
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
        #ifndef FB_NO_URLENCODE
        String umsg;
        FB_urlencode(msg, umsg);
        #endif
        String req;
        _addToken(req);
        #ifndef FB_NO_URLENCODE
        _addMessage(req, umsg);
        #else
        _addMessage(req, msg);
        #endif
        if (replyID) {
            req += F("&reply_to_message_id=");
            req += replyID;
        }
        return sendRequest(req, id);
    }
    
    // ==================== УДАЛЕНИЕ =====================    
    // удалить сообщение id
    uint8_t deleteMessage(int32_t msgid) {
        return deleteMessage(msgid, chatIDs);
    }
    
    uint8_t deleteMessage(int32_t msgid, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/deleteMessage?");
        _addMsgID(req, msgid);
        return sendRequest(req, id);
    }
    
    // ==================== РЕДАКТИРОВАНИЕ =====================
    // редактировать сообщение id
    uint8_t editMessage(int32_t msgid, const String& text) {
        return editMessage(msgid, text, chatIDs);
    }

    uint8_t editMessage(int32_t msgid, const String& text, const String& id) {
        if (!id.length()) return 5;
        #ifndef FB_NO_URLENCODE
        String utext;
        FB_urlencode(text, utext);
        #endif
        String req;
        _addToken(req);
        req += F("/editMessageText?");
        _addMsgID(req, msgid);
        #ifndef FB_NO_URLENCODE
        _addText(req, utext);
        #else
        _addText(req, text);
        #endif
        return sendRequest(req, id);
    }
    
    // ================ РЕДАКТИРОВАНИЕ ИНЛАЙН =================
    // редактировать меню id
    uint8_t editMenuCallback(int32_t msgid, const String& str, const String& cbck) {
        return editMenuCallback(msgid, str, cbck, chatIDs);
    }
    
    uint8_t editMenuCallback(int32_t msgid, const String& str, const String& cbck, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/editMessageReplyMarkup?");
        _addMsgID(req, msgid);
        _addInlineMenu(req, str, cbck);
        return sendRequest(req, id);
    }
    
    // редактировать callback меню id
    uint8_t editMenu(int32_t msgid, const String& str) {
        return editMenu(msgid, str, chatIDs);
    }
    
    uint8_t editMenu(int32_t msgid, const String& str, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/editMessageReplyMarkup?");
        _addMsgID(req, msgid);
        _addInlineMenu(req, str, "");
        return sendRequest(req, id);
    }
    
    // ответить на callback текстом и true - предупреждением
    uint8_t answer() {
        if (!_query) return 5;
        String req;
        _addToken(req);
        req += F("/answerCallbackQuery?callback_query_id=");
        req += *_query;
        _query = nullptr;
        return sendRequest(req);
    }
    
    uint8_t answer(const String& text, bool alert = false) {
        if (!_query) return 5;
        #ifndef FB_NO_URLENCODE
        String utext;
        FB_urlencode(text, utext);
        #endif
        String req;
        _addToken(req);
        req += F("/answerCallbackQuery?callback_query_id=");
        req += *_query;
        #ifndef FB_NO_URLENCODE
        _addText(req, utext);
        #else
        _addText(req, text);
        #endif
        if (alert) req += F("&show_alert=True");
        _query = nullptr;
        return sendRequest(req);
    }
    
    // ============= ГРУППОВЫЕ ДЕЙСТВИЯ =============
    // удалять из чата сервисные сообщения о смене названия и закреплении сообщений
    void clearServiceMessages(bool state) {
        clrSrv = state;
    }
    
    // установить имя группы (бот должен иметь права админа)
    uint8_t setChatTitle(const String& title, const String& id) {
        if (!id.length()) return 5;
        #ifndef FB_NO_URLENCODE
        String utitle;
        FB_urlencode(title, utitle);
        #endif
        String req;
        _addToken(req);
        req += F("/setChatTitle?title=");
        #ifndef FB_NO_URLENCODE
        req += utitle;
        #else
        req += title;
        #endif
        return sendRequest(req, id);
    }
    
    uint8_t setChatTitle(const String& title) {
        return setChatTitle(title, chatIDs);
    }
    
    // установить описание группы (бот должен иметь права админа)
    uint8_t setChatDescription(const String& descr, const String& id) {
        if (!id.length()) return 5;
        #ifndef FB_NO_URLENCODE
        String udescr;
        FB_urlencode(descr, udescr);
        #endif
        String req;
        _addToken(req);
        req += F("/setChatDescription?description=");
        #ifndef FB_NO_URLENCODE
        req += udescr;
        #else
        req += descr;
        #endif
        return sendRequest(req, id);
    }
    
    uint8_t setChatDescription(const String& descr) {
        return setChatDescription(descr, chatIDs);
    }
    
    // закрепить сообщение с ID msgid
    uint8_t pinMessage(int32_t msgid, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/pinChatMessage?disable_notification=True");
        _addMsgID(req, msgid);
        return sendRequest(req, id);
    }
    
    uint8_t pinMessage(int32_t msgid) {
        return pinMessage(msgid, chatIDs);
    }
    
    // открепить сообщение с ID msgid
    uint8_t unpinMessage(int32_t msgid, const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/unpinChatMessage");
        _addMsgID(req, msgid);
        return sendRequest(req, id);
    }
    
    uint8_t unpinMessage(int32_t msgid) {
        return unpinMessage(msgid, chatIDs);
    }
    
    // открепить все сообщения в чате
    uint8_t unpinAll(const String& id) {
        if (!id.length()) return 5;
        String req;
        _addToken(req);
        req += F("/unpinAllChatMessages?");
        return sendRequest(req, id);
    }
    
    uint8_t unpinAll() {
        return unpinAll(chatIDs);
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
        #ifndef FB_NO_URLENCODE
        String umsg;
        FB_urlencode(msg, umsg);
        #endif
        String req;
        _addToken(req);
        #ifndef FB_NO_URLENCODE
        _addMessage(req, umsg);
        #else
        _addMessage(req, msg);
        #endif
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
        #ifndef FB_NO_URLENCODE
        String umsg;
        FB_urlencode(msg, umsg);
        #endif
        String req;
        _addToken(req);
        #ifndef FB_NO_URLENCODE
        _addMessage(req, umsg);
        #else
        _addMessage(req, msg);
        #endif
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
        #ifndef FB_NO_URLENCODE
        String umsg;
        FB_urlencode(msg, umsg);
        #endif
        String req;
        _addToken(req);
        #ifndef FB_NO_URLENCODE
        _addMessage(req, umsg);
        #else
        _addMessage(req, msg);
        #endif
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
            const String& answ = _FB_httpReq.getString();
            int16_t st = 0;
            String buf;
            if (find(answ, buf, st, F("\"message_id\":"), ',', answ.length())) {
                _lastBotMsg = buf.toInt();
            }
            if (find(answ, buf, st, F("\"date\":"), ',', answ.length())) {
                _unix = buf.toInt();
                _lastUpd = millis();
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
    
    // ===================== ВРЕМЯ =====================
    // получить текущее unix время
    uint32_t getUnix() {
        if (_unix) {
            // защита от переполнения разности через 50 суток
            uint32_t diff = millis() - _lastUpd;
            if (_unix && diff > 86400000ul) {
                _unix += diff / 1000ul;
                _lastUpd = millis() - diff % 1000ul;
            }
            return _unix + (millis() - _lastUpd) / 1000ul;
        }
        return 0;
    }
    
    // получить текущее время, указать часовой пояс в часах или минутах
    FB_Time getTime(int16_t gmt) {
        return FB_Time(getUnix(), gmt);
    }
    
    // проверка, синхронизировано ли время
    bool timeSynced() {
        return _unix;
    }

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
        if (cbck.startsWith(F("http"))) str += F("\",\"url\":\"");
        else str += F("\",\"callback_data\":\"");
        str += cbck;
        str += '\"';
    }
    
    // ======================== PARSING =========================
    bool find(const String& str, String& dest, int16_t& start, const String& from, char to, int16_t max) {
        int strPos = str.indexOf(from, start);
        if (strPos < 0 || strPos > max) return 0;
        start = strPos + from.length();
        int16_t end = start;
        while (1) {
            end = str.indexOf(to, end);
            if (str[end - 1] != '\\') break;
            end++;
            if (end >= max) return 0;
        }
        dest = str.substring(start, end);
        start = end;
        return 1;
    }
    
    bool find(const String& str, const String& text, int16_t start, int16_t end) {
        int16_t strPos = str.indexOf(text, start);
        return (strPos > 0) && (strPos < end);
    }

    uint8_t parse(const String& str, uint16_t size) {
        if (!str.startsWith(F("{\"ok\":true"))) return 3;       // ошибка запроса (неправильный токен итд)
        int16_t IDpos = str.indexOf(F("{\"update_id\":"), 0);   // первая позиция ключа update_id
        if (size > _ovf) {                                      // переполнен
            if (IDpos > 0) ID = str.substring(IDpos + 13, str.indexOf(',', IDpos)).toInt();
            ID++;
            return 2;
        }
        int16_t counter = 0;
        while (true) {
            if (IDpos < 0 || IDpos == (int16_t)str.length()) break;
            if (ID == 0) ID = str.substring(IDpos + 13, str.indexOf(',', IDpos)).toInt() + 1;   // холодный запуск, ищем ID
            else counter++;                                                                     // иначе считаем пакеты
            
            int16_t textPos = IDpos;                                // стартовая позиция для поиска
            IDpos = str.indexOf(F("{\"update_id\":"), IDpos + 1);   // позиция id СЛЕДУЮЩЕГО обновления (мы всегда на шаг впереди)
            if (IDpos < 0) IDpos = str.length();                    // если конец пакета - для удобства считаем что позиция ID в конце
            
            String query;
            int16_t queryEnd = 0;
            if (find(str, query, textPos, F("\"callback_query\":{\"id\":\""), ',', IDpos)) {
                _query = &query;
                queryEnd = textPos;
            } else _query = nullptr;
            
            bool edited = find(str, F("\"edited_message\":"), textPos, IDpos);
            
            String messageID;
            find(str, messageID, textPos, F("\"message_id\":"), ',', IDpos);
            _lastUsrMsg = messageID.toInt();
            
            if (queryEnd) textPos = queryEnd;   // возврат на query id, т.к. отправитель указан там
            String userID;
            find(str, userID, textPos, F("\"id\":"), ',', IDpos);
            
            bool is_bot = find(str, F("\"is_bot\":true"), textPos, IDpos);
            
            String first_name;
            find(str, first_name, textPos, F("\"first_name\":\""), '\"', IDpos);
            
            String chatID;
            find(str, chatID, textPos, F("\"chat\":{\"id\":"), ',', IDpos);
            
            if (chatIDs.length() > 0 && chatIDs.indexOf(chatID) < 0) continue;  // проверка на ID чата
            
            String date;
            find(str, date, textPos, F("\"date\":"), ',', IDpos);
            
            // удаление сервисных сообщений
            if (clrSrv) {
                if (find(str, F("\"new_chat_title\""), textPos, IDpos) ||
                    find(str, F("\"pinned_message\""), textPos, IDpos)) {
                        deleteMessage(_lastUsrMsg, chatID);
                        continue;
                }
            }
            
            String text;
            find(str, text, textPos, F("\"text\":\""), '\"', IDpos);
            
            String data;
            find(str, data, textPos, F("\"data\":\""), '\"', IDpos);

            #ifndef FB_NO_UNICODE
            FB_unicode(first_name);
            FB_unicode(text);
            #endif

            FB_msg msg = (FB_msg) {
                userID,
                first_name,
                chatID,
                _lastUsrMsg,
                text,
                data,
                (bool)_query,
                edited,
                is_bot,
                (uint32_t)date.toInt(),
                
                // legacy
                userID,
                first_name,
                first_name,
                _lastUsrMsg,
            };
            _callback(msg);
            if (_query) answer();   // отвечаем на коллбэк, если юзер не ответил
            yield();
        }
        if (_incr) ID += counter;
        return 1;
    }

    void (*_callback)(FB_msg& msg) = nullptr;
    String _token;
    String* _query = nullptr;
    uint16_t _ovf, _prd, _limit;
    int32_t ID = 0;
    uint32_t tmr = 0;
    bool _incr = true;
    int32_t _lastUsrMsg = 0, _lastBotMsg = 0;
    uint8_t parseMode = 0;
    bool notif = true;
    bool clrSrv = false;
    
    uint32_t _unix = 0;
    uint32_t _lastUpd = 0;
};
#endif