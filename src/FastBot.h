/*
    Очень простая и быстрая библиотека для телеграм бота на esp8266/esp32
    Документация: 
    GitHub: https://github.com/GyverLibs/FastBot
    Возможности:
    - Работает на стандартных библиотеках без SSL
    - Опциональный "белый список" ID чатов
    - Проверка обновлений вручную или по таймеру
    - Отправка/удаление/редактирование/ответ на сообщения
    - Работает (чтение и отправка) в чатах, группах, каналах
    - Изменение названия и описания чата
    - Закрепление/открепление сообщений
    - Отправка стикеров, отправка с форматированием markdown/html
    - Вывод обычного и инлайн меню с поддержкой кнопок-ссылок
    - Поддержка Unicode (другие языки + эмодзи) для входящих сообщений
    - Встроенный urlencode для исходящих сообщений
    - Встроенные часы реального времени с синхронизацией от сервера Telegram
    - Возможность OTA обновления прошивки .bin файлом из чата Telegram
    
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
    v2.12: поправлены примеры, исправлен парсинг isBot, переделан механизм защиты от длинных сообщений, переделана инициализация
    v2.13: Оптимизация памяти. Добавил OTA обновление
    v2.14: Улучшен парсинг строки с ID, добавил отключение OTA, добавил парсинг названия группы/канала в username
    v2.15: Заплатка для кривой библиотеки ESP32
    v2.16: добавлен вывод fileName, пофикшены неотправляемые сообщения в Markdown режиме
    v2.17: вывод текста сообщения, на которое ответил юзер + корректная работа с menu в группах
    v2.17.1: мелкий фикс https://github.com/GyverLibs/FastBot/issues/12
    v2.18: добавлен режим FB_DYNAMIC: библиотека дольше выполняет запрос, но занимает на 10 кб меньше памяти в SRAM
    v2.19: поддержка OTA со сжатием gzip
    v2.20:
        - добавил OTA обновление SPIFFS + пример
        - добавил вывод url файла для скачивания из чата + пример
        - добавил возможность скачать файл из чата
        - добавил возможность отправки файлов (из SPIFFS или буфера) + пример
        - добавил возможность редактирования файлов (из SPIFFS или буфера)
        - добавил пример отправки фото с камеры ESP32-CAM
    v2.21: ускорил отправку файлов ботом в чат
    v2.22: мелкая оптимизация, исправил ошибку компиляции при дефайне FB_NO_OTA
    v2.23: пофиксил источник реального времени на editMessage
    v2.24: фикс отправки больших файлов https://github.com/GyverLibs/FastBot/pull/17
    v2.25: добавил skipUpdates - пропуск непрочитанных сообщений
*/

/*
    Статусы tick:
    0 - ожидание
    1 - ОК
    2 - Переполнен
    3 - Ошибка телеграм
    4 - Ошибка подключения
    5 - не задан chat ID
    6 - множественная отправка, статус неизвестен
    7 - не подключен обработчик
    8 - ошибка файла
*/

#ifndef _FastBot_h
#define _FastBot_h

#define FB_BLOCK_SIZE 1024

// ============================================
#define FB_TEXT 0
#define FB_MARKDOWN 1
#define FB_HTML 2

#define FB_NOTIF 0
#define FB_ALERT 1

#define FB_FIRMWARE 1
#define FB_SPIFFS 2

#include <Arduino.h>
#include <StreamString.h>
#include "utils.h"
#include "datatypes.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#ifndef FB_NO_OTA
#include <ESP8266httpUpdate.h>
#endif
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#else   // ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#ifndef FB_NO_OTA
#include <HTTPUpdate.h>
#endif
#include <WiFiClientSecure.h>
#endif

// ================================
class FastBot {
public:
    // инициализация (токен, макс кол-во сообщений на запрос, макс символов, период)
    FastBot(String token = "", uint16_t limit = 10, uint16_t ovf = 0, uint16_t period = 3600) {
        _http = new HTTPClient;
        _token.reserve(46);
        chatIDs.reserve(10);
        _token = token;
        _limit = limit;
        _ovf = ovf;
        _prd = period;
        #if !defined(FB_DYNAMIC) && defined(ESP8266)
        setBufferSizes(512, 512);
        client.setInsecure();
        #endif
    }
    
    ~FastBot() {
        if (_http) delete _http;
    }
    
    // ===================== НАСТРОЙКИ =====================
    // установить размеры буфера на приём и отправку
    void setBufferSizes(uint16_t rx, uint16_t tx) {
        #if !defined(FB_DYNAMIC) && defined(ESP8266)
        client.setBufferSizes(rx, tx);
        #endif
    }
    
    // макс кол-во сообщений на запрос к API
    void setLimit(uint16_t limit) {
        _limit = limit;
    }
    
    // период опроса
    void setPeriod(uint16_t period) {
        _prd = period;
    }
    
    // установка ID чата для парсинга сообщений. Можно ввести через запятую сколько угодно
    void setChatID(const String& chatID) {
        chatIDs = chatID;        
    }
    void setChatID(int64_t id) {
        if (id) chatIDs = FB_64str(id);
        else chatIDs = "";
    }
    
    // установить токен
    void setToken(const String& token) {
        _token = token;
    }
    
    // пропустить непрочитанные сообщения
    void skipUpdates() {
        ID = -1;
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
        String req;
        req.reserve(120);
        _addToken(req);
        req += F("/getUpdates?limit=");
        req += ovfFlag ? 1 : _limit;    // берём по 1 сообщению если переполнен
        req += F("&offset=");
        req += ID;
        //req += F("&allowed_updates=[\"update_id\",\"message\",\"edited_message\",\"channel_post\",\"edited_channel_post\",\"callback_query\"]");
        
        #ifdef ESP8266
        #ifdef FB_DYNAMIC
        BearSSL::WiFiClientSecure client;
        client.setInsecure();
        #endif
        if (!_http->begin(client, req)) return 4;  // ошибка подключения
        #else
        if (!_http->begin(req)) return 4;   // ошибка подключения
        #endif
        int answ = _http->GET();
        if (answ != HTTP_CODE_OK) {
            _http->end();
            if (answ == -1 && _http) {      // заплатка для есп32
                delete _http;
                _http = new HTTPClient;
            }
            return 3;   // ошибка сервера телеграм
        }
        
        #ifndef FB_NO_OTA
        // была попытка OTA обновления. Обновляемся после ответа серверу!
        if (OTAstate >= 0) {
            String ota;
            if (OTAstate == 0) ota = F("Error");
            else if (OTAstate == 1) ota = F("No updates");
            else if (OTAstate == 2) ota = F("OK");
            sendMessage(ota, _otaID);
            if (OTAstate == 2) ESP.restart();
            OTAstate = -1;
        }
        #endif
        
        int size = _http->getSize();
        ovfFlag = size > 25000;         // 1 полное сообщение на русском языке или ~5 на английском
        uint8_t status = 1;             // OK
        if (size) {                     // не пустой ответ?
            StreamString sstring;
            if (!ovfFlag && sstring.reserve(size + 1)) {    // не переполнен и хватает памяти
                _http->writeToStream(&sstring);             // копируем
                _http->end();                               // завершаем
                return parseMessages(sstring);              // парсим
            } else status = 2;                              // переполнение
        } else status = 3;                                  // пустой ответ        
        _http->end();
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
        if (parseMode == FB_MARKDOWN) FB_escMarkdown(umsg);
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
        if (parseMode == FB_MARKDOWN) FB_escMarkdown(utext);
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
        if (!_query_ptr) return 5;
        String req;
        _addToken(req);
        req += F("/answerCallbackQuery?callback_query_id=");
        req += *_query_ptr;
        _query_ptr = nullptr;
        return sendRequest(req);
    }
    
    uint8_t answer(const String& text, bool alert = false) {
        if (!_query_ptr) return 5;
        #ifndef FB_NO_URLENCODE
        String utext;
        FB_urlencode(text, utext);
        if (parseMode == FB_MARKDOWN) FB_escMarkdown(utext);
        #endif
        String req;
        _addToken(req);
        req += F("/answerCallbackQuery?callback_query_id=");
        req += *_query_ptr;
        #ifndef FB_NO_URLENCODE
        _addText(req, utext);
        #else
        _addText(req, text);
        #endif
        if (alert) req += F("&show_alert=True");
        _query_ptr = nullptr;
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
        if (parseMode == FB_MARKDOWN) FB_escMarkdown(umsg);
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
        if (parseMode == FB_MARKDOWN) FB_escMarkdown(umsg);
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
        if (parseMode == FB_MARKDOWN) FB_escMarkdown(umsg);
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
        #ifdef ESP8266
        #ifdef FB_DYNAMIC
        BearSSL::WiFiClientSecure client;
        client.setInsecure();
        #endif
        if (!_http->begin(client, req)) return 4;  // ошибка подключения
        #else
        if (!_http->begin(req)) return 4;
        #endif
        int answ = _http->GET();
        if (answ == -1 && _http) {       // заплатка для есп32
            _http->end();
            delete _http;
            _http = new HTTPClient;
            // постучимся ещё раз
            #ifdef ESP8266
            if (!_http->begin(client, req)) return 4;  // ошибка подключения
            #else
            if (!_http->begin(req)) return 4;
            #endif
            answ = _http->GET();
        }
        uint8_t status = 1;
        if (answ == HTTP_CODE_OK && _http->getSize()) {     // есть ответ и он не пустой
            parseRequest(_http->getString());       // парсим
        } else status = 3;                          // некорректный ответ
        _http->end();
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
    
    // ===================== OTA =====================
    // ОТА обновление, вызывать внутри обработчика сообщения по флагу OTA
    uint8_t update(__attribute__((unused)) uint8_t type = FB_FIRMWARE) {
        #ifndef FB_NO_OTA
        if (!_file_ptr) return 8;
        OTAflag = type;
        sendMessage((type == FB_FIRMWARE) ? F("OTA firmware...") : F("OTA spiffs..."), _otaID);

        #ifdef ESP8266
        ESPhttpUpdate.rebootOnUpdate(false);
        #ifdef FB_DYNAMIC
        BearSSL::WiFiClientSecure client;
        client.setInsecure();
        #endif
        if (OTAflag == FB_FIRMWARE) OTAstate = ESPhttpUpdate.update(client, *_file_ptr);
        else if (OTAflag == FB_SPIFFS) OTAstate = ESPhttpUpdate.updateFS(client, *_file_ptr);
        #else
        WiFiClientSecure client;
        client.setInsecure();
        httpUpdate.rebootOnUpdate(false);
        if (OTAflag == FB_FIRMWARE) OTAstate = httpUpdate.update(client, *_file_ptr);
        else if (OTAflag == FB_SPIFFS) OTAstate = httpUpdate.updateSpiffs(client, *_file_ptr);
        #endif
        #endif
        return 1;
    }
    
    // ОТА обновление SPIFFS, вызывать внутри обработчика сообщения по флагу OTA
    uint8_t updateFS() {
        return update(FB_SPIFFS);
    }
    
    // ===================== FILE =====================
    uint8_t sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, const String& id) {
        return _sendFile(buf, length, type, name, id);
    }
    uint8_t sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name) {
        return sendFile(buf, length, type, name, chatIDs);
    }
    
#ifdef FS_H
    uint8_t sendFile(File &file, FB_FileType type, const String& name, const String& id) {
        return _sendFile(file, type, name, id);
    }
    uint8_t sendFile(File &file, FB_FileType type, const String& name) {
        return sendFile(file, type, name, chatIDs);
    }
#endif
    
    uint8_t editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid, const String& id) {
        return _editFile(buf, length, type, name, msgid, id);
    }
    uint8_t editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid) {
        return editFile(buf, length, type, name, msgid, chatIDs);
    }

#ifdef FS_H
    uint8_t editFile(File &file, FB_FileType type, const String& name, int32_t msgid, const String& id) {
        return _editFile(file, type, name, msgid, id);
    }
    uint8_t editFile(File &file, FB_FileType type, const String& name, int32_t msgid) {
        return editFile(file, type, name, msgid, chatIDs);
    }
#endif
    
    // ============================ DOWNLOAD ============================
#ifdef FS_H
    // скачать файл
    bool downloadFile(File &f, const String& url) {
        if (!f) return 0;
#ifdef ESP8266
#ifdef FB_DYNAMIC
        BearSSL::WiFiClientSecure client;
        client.setInsecure();
#endif
        if (!_http->begin(client, url)) return 0;
#else
        if (!_http->begin(url)) return 0;
#endif
        bool stat = 0;
        if (_http->GET() == HTTP_CODE_OK) {
            if (_http->writeToStream(&f)) stat = 1;
        }
        _http->end();
        f.close();
        return stat;
    }
#endif

    // ============================ PRIVATE ============================
private:
    // конечная строка запроса
#define FB_END_REQ "\r\n" "--FAST_BOT--"

    // тип клиента в зависимости от платформы
#ifdef ESP8266
#define FB_SECURE_CLIENT BearSSL::WiFiClientSecure
#else
#define FB_SECURE_CLIENT WiFiClientSecure
#endif

    // ============================ MULTIPART SEND ============================
    bool _multipartSend(FB_SECURE_CLIENT& client, uint32_t length, FB_FileType type, const String& name, const String& id) {
        if (!client.connect("api.telegram.org", 443)) return 0;
        String startReq;
        startReq += F("--FAST_BOT" "\r\n");
        startReq += F("content-disposition: form-data; name=\"");
        switch (type) {
        case FB_PHOTO: startReq += F("photo"); break;
        case FB_AUDIO: startReq += F("audio"); break;
        case FB_DOC: startReq += F("document"); break;
        case FB_VIDEO: startReq += F("video"); break;
        case FB_GIF: startReq += F("animation"); break;
        case FB_VOICE: startReq += F("voice"); break;
        }
        startReq += F("\"; filename=\"");
        startReq += name;
        startReq += F("\"\r\n\r\n");
        client.print(F("POST /bot"));
        client.print(_token);
        switch (type) {
        case FB_PHOTO: client.print(F("/sendPhoto")); break;
        case FB_AUDIO: client.print(F("/sendAudio")); break;
        case FB_DOC: client.print(F("/sendDocument")); break;
        case FB_VIDEO: client.print(F("/sendVideo")); break;
        case FB_GIF: client.print(F("/sendAnimation")); break;
        case FB_VOICE: client.print(F("/sendVoice")); break;
        }
        client.print(F("?chat_id="));
        client.print(id);
        client.println(F(" HTTP/1.1"));
        client.println(F("Host: api.telegram.org"));
        client.println(F("User-Agent: esp"));
        client.println(F("Accept: */*"));
        client.print(F("Content-Length: "));
        String endReq = F(FB_END_REQ);
        client.println(String(length + startReq.length() + endReq.length()));
        client.print(F("Content-Type: multipart/form-data; boundary=" "FAST_BOT" "\r\n\r\n"));
        client.print(startReq);
        return 1;
    }
    
    // ============================ MULTIPART EDIT ============================
    bool _multipartEdit(FB_SECURE_CLIENT& client, uint32_t length, FB_FileType type, const String& name, uint32_t msgid, const String& id) {
        if (!client.connect("api.telegram.org", 443)) return 0;
        String startReq;
        uint16_t rndName = random(0xFFFF);
        startReq += F("--FAST_BOT" "\r\n");
        startReq += F("content-disposition: form-data; name=\"");
        startReq += rndName;
        startReq += F("\"; filename=\"");
        startReq += name;
        startReq += F("\"\r\n\r\n");
        client.print(F("POST /bot"));
        client.print(_token);
        client.print(F("/editMessageMedia?chat_id="));
        client.print(id);
        client.print(F("&message_id="));
        client.print(msgid);
        client.print("&media={\"type\":\"");
        switch (type) {
        case FB_PHOTO: client.print(F("photo")); break;
        case FB_AUDIO: client.print(F("audio")); break;
        case FB_DOC: client.print(F("document")); break;
        case FB_VIDEO: client.print(F("video")); break;
        case FB_GIF: client.print(F("animation")); break;
        case FB_VOICE: break;
        }
        client.print(F("\", \"media\":\"attach://"));
        client.print(rndName);
        client.print(F("\"}"));
        client.println(F(" HTTP/1.1"));
        client.println(F("Host: api.telegram.org"));
        client.println(F("User-Agent: esp"));
        client.println(F("Accept: */*"));
        client.print(F("Content-Length: "));
        String endReq = F(FB_END_REQ);
        client.println(String(length + startReq.length() + endReq.length()));
        client.print(F("Content-Type: multipart/form-data; boundary=" "FAST_BOT" "\r\n\r\n"));
        client.print(startReq);
        return 1;
    }

    // ============================ MULTIPART END ============================
    uint8_t _multipartEnd(FB_SECURE_CLIENT& client) {
        client.print(F(FB_END_REQ));     // endReq
        char prev = '\0';
        while (client.connected()) {
            char cur = client.read();
            if (cur == '\r' && prev == '\n') break;
            prev = cur;
        }
        String resp;
        resp.reserve(client.available());
        while (client.available()) resp += (char)client.read();
        client.stop();
        return (parseRequest(resp) ? 1 : 3);  // 1 - ok, 3 - telegram err
    }
    
    // ============================ ROUTINE BUF ============================
    void _sendBufRoutine(FB_SECURE_CLIENT& client, uint8_t* buf, uint32_t length) {
        uint32_t sz = length;
        uint8_t* bufs = buf;
        while (sz) {
            uint32_t len = min((uint32_t)FB_BLOCK_SIZE, sz);
            client.write(bufs, len);
            bufs += len;
            sz -= len;
        }
    }
    
    // ============================ ROUTINE FILE ============================
#ifdef FS_H
    void _sendFileRoutine(FB_SECURE_CLIENT& client, File &file) {
        uint8_t buf[FB_BLOCK_SIZE];
        uint32_t sz = file.size();
        while (sz) {
            uint32_t len = min((uint32_t)FB_BLOCK_SIZE, sz);
            file.read(buf, len);
            client.write(buf, len);
            sz -= len;
        }
    }
#endif

    // ============================ CLIENT MACRO ============================
    // макрос создания клиента в зависимости от платформы и настроек
#ifdef ESP8266
#ifdef FB_DYNAMIC
#define FB_DECLARE_CLIENT() \
        BearSSL::WiFiClientSecure client; \
        client.setInsecure();
#else
#define FB_DECLARE_CLIENT()
#endif
#else
#define FB_DECLARE_CLIENT() \
        WiFiClientSecure client; \
        client.setInsecure();
#endif
    
    // ============================ SEND BUF ============================
    uint8_t _sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, const String& id) {
        FB_DECLARE_CLIENT();
        if (!_multipartSend(client, length, type, name, id)) return 4;
        _sendBufRoutine(client, buf, length);
        return _multipartEnd(client);
    }
    
    // ============================ SEND FILE ============================
#ifdef FS_H
    uint8_t _sendFile(File &file, FB_FileType type, const String& name, const String& id) {
        FB_DECLARE_CLIENT();
        if (!_multipartSend(client, file.size(), type, name, id)) return 4;
        _sendFileRoutine(client, file);
        return _multipartEnd(client);
    }
#endif

    // ============================ EDIT BUF ============================
    uint8_t _editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid, const String& id) {
        FB_DECLARE_CLIENT();
        if (!_multipartEdit(client, length, type, name, msgid, id)) return 4;
        _sendBufRoutine(client, buf, length);
        return _multipartEnd(client);
    }
    

    // ============================ EDIT FILE ============================
#ifdef FS_H
    uint8_t _editFile(File& file, FB_FileType type, const String& name, int32_t msgid, const String& id) {
        FB_DECLARE_CLIENT();
        if (!_multipartEdit(client, file.size(), type, name, msgid, id)) return 4;
        _sendFileRoutine(client, file);
        return _multipartEnd(client);
    }
#endif

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

    uint8_t parseMessages(const String& str) {
        if (!str.startsWith(F("{\"ok\":true"))) return 3;       // ошибка запроса (неправильный токен итд)
        int16_t IDpos = str.indexOf(F("{\"update_id\":"), 0);   // первая позиция ключа update_id
        if (IDpos < 0) {
            if (ID < 0) ID = 0;
            return 1;
        }
        int16_t counter = 0;
        while (true) {
            if (IDpos < 0 || IDpos == (int16_t)str.length()) break;
            bool skip = (ID == -1);
            if (!counter) ID = str.substring(IDpos + 13, str.indexOf(',', IDpos)).toInt() + 1;
            else counter++;                                                           // иначе считаем пакеты
            
            int16_t textPos = IDpos;                                // стартовая позиция для поиска
            IDpos = str.indexOf(F("{\"update_id\":"), IDpos + 1);   // позиция id СЛЕДУЮЩЕГО обновления (мы всегда на шаг впереди)
            if (IDpos < 0) IDpos = str.length();                    // если конец пакета - для удобства считаем что позиция ID в конце
            if (skip) continue;
            
            String query;
            int16_t queryEnd = 0;
            if (_query_ptr) _query_ptr = nullptr;
            if (find(str, query, textPos, F("\"callback_query\":{\"id\":\""), ',', IDpos)) {
                _query_ptr = &query;
                queryEnd = textPos;
            }
            
            bool edited = find(str, F("\"edited_message\":"), textPos, IDpos);
            
            String messageID;
            find(str, messageID, textPos, F("\"message_id\":"), ',', IDpos);
            _lastUsrMsg = messageID.toInt();
            
            if (queryEnd) textPos = queryEnd;   // возврат на query id, т.к. отправитель указан там
            String userID;
            find(str, userID, textPos, F("\"id\":"), ',', IDpos);
            
            String is_bot;
            find(str, is_bot, textPos, F("\"is_bot\":"), ',', IDpos);
            
            String first_name;
            find(str, first_name, textPos, F("\"first_name\":\""), '\"', IDpos);
            
            String chatID;
            find(str, chatID, textPos, F("\"chat\":{\"id\":"), ',', IDpos);
            
            if (!first_name.length()) {
                int typePos = str.indexOf("\"type\"", textPos);
                find(str, first_name, textPos, F("\"title\":\""), '\"', typePos);
            }
            
            if (chatIDs.length() > 0 && chatIDs.indexOf(chatID) < 0) continue;  // проверка на ID чата
            
            String date;
            find(str, date, textPos, F("\"date\":"), ',', IDpos);
            bool reply = find(str, F("\"reply_to_message\""), textPos, IDpos);
            
            bool isOTA = false;
            String fileName;
            String fileUrl;
            if (find(str, fileName, textPos, F("\"file_name\":\""), '\"', IDpos)) {
                _file_ptr = &fileUrl;
                if (fileName.endsWith(F(".bin")) || fileName.endsWith(F(".bin.gz"))) {
                    isOTA = 1;
                    _otaID = chatID;
                }
                fileUrl.reserve(120);
                String fileID;
                find(str, fileID, textPos, F("\"file_id\":\""), '\"', IDpos);
                String req;
                _addToken(req);
                req += F("/getFile?file_id=");
                req += fileID;
                sendRequest(req);
            }
            
            // удаление сервисных сообщений
            if (clrSrv) {
                if (find(str, F("\"new_chat_title\""), textPos, IDpos) ||
                        find(str, F("\"pinned_message\""), textPos, IDpos)) {
                    deleteMessage(_lastUsrMsg, chatID);
                    continue;
                }
            }
            
            String text;
            String replyText;
            if (_file_ptr) find(str, text, textPos, F("\"caption\":\""), '\"', IDpos);
            else find(str, text, textPos, F("\"text\":\""), '\"', IDpos);
            
            if (reply) {
                replyText = text;
                find(str, text, textPos, F("\"text\":\""), '\"', IDpos);
            }
            
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
                (bool)_query_ptr,
                edited,
                is_bot[0] == 't',
                isOTA,
                (uint32_t)date.toInt(),
                fileName,
                replyText,
                (bool)_file_ptr,
                fileUrl,
                
                // legacy
                userID,
                first_name,
                first_name,
                _lastUsrMsg,
            };
            _callback(msg);
            if (_query_ptr) answer();   // отвечаем на коллбэк, если юзер не ответил
            _file_ptr = nullptr;
            if (OTAstate >= 0) break;   // обновление! выходим
            yield();
        }
        if (_incr) ID += counter;
        return 1;
    }
    
    bool parseRequest(const String& answ) {
        int16_t st = 0;
        String buf;
        bool OK = 0;
        if (find(answ, buf, st, F("\"message_id\":"), ',', answ.length())) {
            _lastBotMsg = buf.toInt();
            OK = 1;
        }
        if (find(answ, buf, st, F("\"edit_date\":"), ',', answ.length()) ||
                find(answ, buf, st, F("\"date\":"), ',', answ.length())) {
            _unix = buf.toInt();
            _lastUpd = millis();
        }
        if (_file_ptr && find(answ, buf, st, F("\"file_path\":\""), '\"', answ.length())) {
            *_file_ptr = F("https://api.telegram.org/file/bot");
            *_file_ptr += _token;
            *_file_ptr += '/';
            *_file_ptr += buf;
        }
        return OK;
    }
    
    HTTPClient *_http = nullptr;
    #if !defined(FB_DYNAMIC) && defined(ESP8266)
    BearSSL::WiFiClientSecure client;
    #endif
    
    void (*_callback)(FB_msg& msg) = nullptr;
    String _token;
    String _otaID;
    String* _file_ptr = nullptr;
    String* _query_ptr = nullptr;
    uint16_t _ovf, _prd, _limit;
    int32_t ID = 0;
    uint32_t tmr = 0;
    bool _incr = true;
    int32_t _lastUsrMsg = 0, _lastBotMsg = 0;
    uint8_t parseMode = 0;
    bool notif = true;
    bool clrSrv = false;
    bool ovfFlag = 0;
    int8_t OTAstate = -1;
    uint8_t OTAflag = 0;
    
    uint32_t _unix = 0;
    uint32_t _lastUpd = 0;
};
#endif