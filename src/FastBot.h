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

#ifndef FastBot_h
#define FastBot_h
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
static std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
static HTTPClient https;

// вспомогательный класс
class _StringParser {
public:
    void reset() {
        from = to = -1;    
    }
    bool update(String& str) {
        if (to == str.length()) return 0;
        from = to + 1;
        to = str.indexOf(',', from);
        if (to < 0) to = str.length();
        return 1;    
    }
    int from = 0;
    int to = 0;
private:
};
static _StringParser _pars;

// ================================
class FastBot {
public:
    // инициализация (токен, макс кол-во сообщений на запрос, макс символов, период)
    FastBot(String token, int limit = 10, int ovf = 10000, int period = 1000) {
        _request = (String)"https://api.telegram.org/bot" + token;
        _ovf = ovf;
        _limit = limit;
        _period = period;
        client->setInsecure();
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
        _period = period;
    }
    
    // установка ID чата для парсинга сообщений. Можно ввести через запятую сколько угодно
    void setChatID(String chatID) {
        chatIDs = chatID;        
    }

    // подключение обработчика сообщений
    void attach(void (*handler)(String&, String&)) {
        _callback = handler;
    }
    
    // отключение обработчика сообщений
    void detach() {
        _callback = NULL;
    }
    
    // ручная проверка обновлений
    uint8_t tickManual() {
        uint8_t status = 1;
        if (https.begin(*client, _request + "/getUpdates?limit=" + _limit + "&offset=" + ID)) {
            if (https.GET() == HTTP_CODE_OK) status = parse(https.getString());
            else status = 3;
            https.end();
        } else status = 4;
        return status;
    }
    
    // проверка обновлений по таймеру
    uint8_t tick() {
        if (millis() - tmr >= _period) {
            tmr = millis();
            return tickManual();
        }
        return 0;
    }
    
    // отправить сообщение в указанный в setChatID чат/чаты
    uint8_t sendMessage(String msg) {
        return sendMessage(msg, chatIDs);
    }
    
    // отправить сообщение в указанный здесь чат/чаты
    uint8_t sendMessage(String msg, String id) {
        if (!id.length()) return 5;                             // не задан ID чата
        if (id.indexOf(',') < 0) return _sendMessage(msg, id);  // один ID
        else {                                                  // несколько ID
            _pars.reset();
            while(_pars.update(id)) _sendMessage(msg, id.substring(_pars.from, _pars.to));            
        }
        return 6;
    }
    
    // удалить сообщение со смещением offset в указанный в setChatID чат/чаты
    uint8_t deleteMessage(int offset) {
        return deleteMessage(offset, chatIDs);
    }
    
    // удалить сообщение со смещением offset в указанном здесь ID чата/чатов
    uint8_t deleteMessage(int offset, String id) {
        if (!id.length()) return 5;                                 // не задан ID чата
        if (id.indexOf(',') < 0) return _deleteMessage(offset, id); // один ID
        else {                                                      // несколько ID
            _pars.reset();
            while(_pars.update(id)) _deleteMessage(offset, id.substring(_pars.from, _pars.to));            
        }
        return 6;
    }
    
    // показать меню в указанном в setChatID чате/чатах
    uint8_t showMenu(String str) {
        return showMenu(str, chatIDs);
    }
    
    // показать меню в указанном здесь чате/чатах
    uint8_t showMenu(String str, String id) {
        String msg = "Show Menu";
        if (!id.length()) return 5;                                 // не задан ID чата
        if (id.indexOf(',') < 0) return _showMenu(str, msg, id);    // один ID
        else {                                                       // несколько ID
            _pars.reset();
            while(_pars.update(id)) _showMenu(str, msg, id.substring(_pars.from, _pars.to));            
        }
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
        else {                                                      // несколько ID
            _pars.reset();
            while(_pars.update(id)) _showMenu(str, msg, id.substring(_pars.from, _pars.to));            
        }
        return 6;
    }
    
    // закрыть меню в указанном в setChatID чате/чатах
    uint8_t closeMenu() {
        return closeMenu(chatIDs);
    }
    
    // закрыть меню в указанном здесь чате/чатах
    uint8_t closeMenu(String id) {
        String msg = "Close Menu";
        if (!id.length()) return 5;                             // не задан ID чата
        if (id.indexOf(',') < 0) return _closeMenu(msg, id);    // один ID
        else {                                                  // несколько ID
            _pars.reset();
            while(_pars.update(id)) _closeMenu(msg, id.substring(_pars.from, _pars.to));            
        }
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
        else {                                                  // несколько ID
            _pars.reset();
            while(_pars.update(id)) _closeMenu(msg, id.substring(_pars.from, _pars.to));            
        }
        return 6;
    }
    
    // показать инлайн меню в указанном в setChatID чате/чатах
    uint8_t inlineMenu(String msg, String str) {
        return inlineMenu(msg, str, chatIDs);
    }
    
    // показать инлайн меню в указанном здесь чате/чатах
    uint8_t inlineMenu(String msg, String str, String id) {
        if (!id.length()) return 5;                             // не задан ID чата
        if (id.indexOf(',') < 0) return _inlineMenu(msg, str, id);     // один ID
        else {                                                  // несколько ID
            _pars.reset();
            while(_pars.update(id)) _inlineMenu(msg, str, id.substring(_pars.from, _pars.to));            
        }
        return 6;
    }
    
    // отправить запрос
    uint8_t sendRequest(String& req) {
        uint8_t status = 1;
        if (https.begin(*client, req)) {
            if (https.GET() != HTTP_CODE_OK) status = 3;
            https.end();
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
    String chatIDs = "";

private:
    uint8_t _deleteMessage(int offset, String id) {
        String request = _request + "/deleteMessage?chat_id=" + id + "&message_id=" + (lastMsg-offset);
        return sendRequest(request);
    }
    
    uint8_t _sendMessage(String& msg, String id) {
        String request = _request + "/sendMessage?chat_id=" + id + "&text=" + msg;
        return sendRequest(request);
    }
    
    uint8_t _showMenu(String& str, String& msg, String id) {
        String request = _request + "/sendMessage?chat_id=" + id + "&text=" + msg + "&reply_markup={\"keyboard\":[[\"";
        for (int i = 0; i < str.length(); i++) {
            char c = str[i];
            if (c == '\t') request += "\",\"";
            else if (c == '\n') request += "\"],[\"";
            else request += c;
        }
        request += "\"]],\"resize_keyboard\":true}";
        return sendRequest(request);
    }
    
    uint8_t _closeMenu(String& msg, String id) {
        String request = _request + "/sendMessage?chat_id=" + id + "&text=" + msg + "&reply_markup={\"remove_keyboard\":true}";
        return sendRequest(request);
    }
    
    uint8_t _inlineMenu(String& msg, String& str, String id) {
        String request = _request + "/sendMessage?chat_id=" + id + "&text=" + msg + "&reply_markup={\"inline_keyboard\":[[{";
        String buf = "";
        for (int i = 0; i < str.length(); i++) {
            char c = str[i];
            if (c == '\t') {
                addInlineButton(request, buf);
                request += "},{";
                buf = "";
            }
            else if (c == '\n') {
                addInlineButton(request, buf);
                request += "}],[{";
                buf = "";
            }
            else buf += c;
        }
        addInlineButton(request, buf);
        request += "}]]}";
        return sendRequest(request);
    }
    
    void addInlineButton(String& str, String& msg) {
        str += "\"text\":\"" + msg + "\",\"callback_data\":\"" + msg + '\"';
    }
    
    uint8_t parse(const String& str) {
        if (!str.startsWith("{\"ok\":true")) {    // ошибка запроса (неправильный токен итд)
            https.end();
            return 3;
        }
        if (https.getSize() > _ovf) {             // переполнен
            int IDpos = str.indexOf("{\"update_id\":", IDpos);
            if (IDpos > 0) ID = str.substring(IDpos + 13, str.indexOf(',', IDpos)).toInt();
            ID++;
            https.end();
            return 2;
        }

        int IDpos = str.indexOf("{\"update_id\":", 0);  // первая позиция ключа update_id
        int counter = 0;
        while (true) {
            if (IDpos > 0 && IDpos < str.length()) {      // если есть что разбирать
                if (ID == 0) ID = str.substring(IDpos + 13, str.indexOf(',', IDpos)).toInt() + 1;   // холодный запуск, ищем ID
                else counter++;                                                                     // иначе считаем пакеты
                int textPos = IDpos;                                  // стартовая позиция для поиска
                int endPos;
                IDpos = str.indexOf("{\"update_id\":", IDpos + 1);    // позиция id СЛЕДУЮЩЕГО обновления (мы всегда на шаг впереди)
                if (IDpos == -1) IDpos = str.length();                // если конец пакета - для удобства считаем что позиция ID в конце

                // ищем ID сообщения 
                textPos = str.indexOf("\"message_id\":", textPos);
                if (textPos < 0 || textPos > IDpos) continue;
                endPos = str.indexOf(",\"", textPos);
                lastMsg = str.substring(textPos + 13, endPos).toInt();
                
                // установлена проверка на ID чата - проверяем соответствие
                if (chatIDs.length() > 0) {
                    textPos = str.indexOf("\"chat\":{\"id\":", textPos);
                    if (textPos < 0 || textPos > IDpos) continue;
                    endPos = str.indexOf(",\"", textPos);
                    String chatID = str.substring(textPos + 13, endPos);
                    textPos = endPos;
                    if (chatIDs.indexOf(chatID) < 0) continue;  // не тот чат
                }

                // ищем имя юзера
                textPos = str.indexOf("\"username\":\"", textPos);
                if (textPos < 0 || textPos > IDpos) continue;
                endPos = str.indexOf("\",\"", textPos);
                String name = str.substring(textPos + 12, endPos);

                // ищем сообщение
                String msg;
                int dataPos = str.indexOf("\"data\":", textPos);  // вдруг это callback_data
                if (dataPos > 0 && dataPos < IDpos) {
                    endPos = str.indexOf("\"}}", textPos);
                    msg = str.substring(dataPos + 8, endPos);     // забираем callback_data
                } else {
                    textPos = str.indexOf(",\"text\":\"", textPos);
                    if (textPos < 0 || textPos > IDpos) continue;
                    endPos = str.indexOf("\"}}", textPos);
                    int endPos2 = str.indexOf("\",\"entities", textPos);
                    if (endPos > 0 && endPos2 > 0) endPos = min(endPos, endPos2);
                    else if (endPos < 0) endPos = endPos2;
                    if (str[textPos + 9] == '/') textPos++;
                    msg = str.substring(textPos + 9, endPos);       // забираем обычное сообщение
                }
                if (*_callback) _callback(name, msg);
            } else break;   // IDpos > 0
        }
        if (_incr) ID += counter;
        return 1;
    }

    void (*_callback)(String& name, String& msg) = NULL;
    String _request;
    int _ovf = 5000, _period = 1000, _limit = 10;
    long ID = 0;
    uint32_t tmr = 0;    
    bool _incr = true;
    uint32_t lastMsg = 0;
};
#endif
