This is an automatic translation, may be incorrect in some places. See sources and examples!

# FastBot
Very simple and fast library for telegram bot on esp8266/esp32
- Works on standard libraries
- Works without SSL
- Optimized for heavy load
- Optional whitelist of chat IDs
- Check for updates manually or by timer
- Send/delete/edit/reply messages
- Send stickers
- Menu output and inline menu (with link support)
- Change the name and description of the chat
- Pin/unpin messages
- Unicode support (other languages ​​+ emoji) for incoming messages
- Built-in urlencode for outgoing messages
- Built-in real time clock with synchronization from the Telegram server
- Possibility of OTA firmware update by file from Telegram chat

### Compatibility
ESP8266 (SDK v2.6+), ESP32

## Documentation and projects
Detailed tutorials on working with the Telegram bot using this library can be found on the [GyverKIT Arduino website] (https://kit.alexgyver.ru/tutorials-category/telegram/)

## Comparison with Universal-Arduino-Telegram-Bot
[Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)

For comparison, we used a minimal example with sending a message to the chat and outputting incoming messages to the series:
- **send** - send a message to the chat
- **update** - check incoming messages
- **free heap** - amount of free RAM while the program is running

| library | Flash, B | SRAM, B | send, ms | update, ms | free heap, B |
|-----------|----------|---------|----------|----- -------|--------------|
| Univ..Bot | 400004 | 29848 | 2000 | 1900 | 38592 |
| fastbot | 393220 | 28036 | 70 | 70 | 37552 |
| diff | 6784 | 1812 | 1930 | 1830 | 1040 |

- FastBot is lighter by almost 7 kB of Flash and 2 kB of SRAM, but takes up 1 kB of SRAM more while the program is running. Total lighter by 2-1 = 1 kB of SRAM.
- FastBot processes chat and sends messages much faster (by 2 seconds) due to manual parsing of the server response and statically allocated HTTP clients

## Content
- [Install](#install)
- [Initialization](#init)
- [Documentation](#docs)
- [Usage](#usage)
    - [ID chat/chats](#chatid)
    - [Message Parsing](#inbox)
    - [Ticker](#tick)
    - [Minimal Example](#example)
    - [Referring to messages](#msgid)
    - [Send stickers](#sticker)
    - [Menu](#menu)
    - [Normal menu](#basic)
    - [Inline menu](#inline)
    - [Inline menu with callback](#callb)
    - [Response to callback](#answer)
    - [Time module](#unix)
    - [Time to receive message](#time)
    - [Real Time Clock](#rtc)
    - [OTA firmware update](#ota)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **FastBot** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/FastBot/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more andinstructions for installing libraries [here] %D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
- [Instructions on how to create and configure a Telegram bot](https://kit.alexgyver.ru/tutorials/telegram-basic/)

```cpp
fastbot bot
FastBot bot(token); // indicating the token
```

<a id="docs"></a>
## Documentation
```cpp
// ============== SETTINGS ==============
void setToken(String token); // change/set bot token
void setChatID(String chatID); // setting the chat ID (white list), optional. Multiple separated by commas ("id1,id2,id3")
void setPeriod(int period); // polling period in ms (default 3500)
void setLimit(int limit); // number of messages processed per request, 1..100. (default 10)
void setBufferSizes(uint16_t rx, uint16_t tx); // set buffer sizes for receiving and sending, by default. 512 and 512 bytes (only for esp8266)

void setTextMode(uint8_t mode); // text mode "to send": FB_TEXT, FB_MARKDOWN, FB_HTML (see textMode example)
void notify(bool mode); // true/false enable/disable notifications from bot messages (on by default)
void clearServiceMessages(bool state); // remove service messages about changing the name and pinning messages from the chat (default false)


// =============== PARSING ===============
void attach(callback); // connection of the message handler function
void detach(); // disable message handler


// ================ TICKER ================
uint8_t tick(); // check for updates by timer
uint8_t tickManual(); // manual check for updates


// ============== MESSAGES ==============
// send a message to the chat/chats specified in setChatID OR pass the chat id
uint8_t sendMessage(String msg);
uint8_t sendMessage(String msg, String id);

// edit the message (msgid) in the chat specified in setChatID OR pass the chat id
uint8_t editMessage(int32_t msgid, String text);
uint8_t editMessage(int32_t msgid, String text, String id);

// reply to a message with id (replyID) in the chat specified in setChatID OR specify a chat
uint8_t replyMessage(String msg, int32_t replyID);
uint8_t replyMessage(String msg, int32_t replyID, String id);

// send the sticker to the chat/chats specified in setChatID OR pass the chat id
uint8_t sendSticker(String stickerID);
uint8_t sendSticker(String stickerID, String id);

// reply to callback with text (text) and mode (alert): FB_NOTIF - chat notification, FB_ALERT - window with OK button
uint8_t answer(String text, bool alert);


// =============== DELETE ===============
// delete the message with id (msgid) in the chat specified in setChatID OR pass the chat id
// deletes any type of messages (text, sticker, inline menu)
uint8_t deleteMessage(int32_t msgid);
uint8_t deleteMessage(int32_t msgid, String id);


// ============= REGULAR MENU =============
// show the menu (menu) in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t showMenu(String menu);
uint8_t showMenu(String menu, String id);

// one-time menu (will close when selected) in the current chat OR pass the chat id
uint8_t showMenu(String menu, true);
uint8_t showMenu(String menu, String id, true);

// hide the menu in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t closeMenu();
uint8_t closeMenu(String id);


// ======== REGULAR MENU WITH TEXT =========
// message (msg) + show menu (menu) in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t showMenuText(String msg, String menu);
uint8_t showMenuText(String msg, String menu, String id);

// one-time menu (will close when selected)
uint8_t showMenuText(String msg, String menu, true);
uint8_t showMenuText(String msg, String menu, String id, true);

// message (msg) + hide the menu in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t closeMenuText(String msg);
uint8_t closeMenuText(String msg, String id);


// ============= INLINE MENU =============
// message (msg) with an inline menu (menu) in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t inlineMenu(String msg, String menu);
uint8_t inlineMenu(String msg, String menu, String id);

// edit the menu (msgid) with text (menu) in the chat specified in setChatID OR pass the chat id
uint8_t editMenu(int32_t msgid, String menu);
uint8_t editMenu(int32_t msgid, String menu, String id);


// ======= INLINE MENU WITH A CALLBACK =======
// message (msg) with inline menu (menu) and callback (cbck) in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t inlineMenuCallback(String msg, String menu, String cbck);
uint8_t inlineMenuCallback(String msg, String menu, String cbck, String id);

// edit the menu (msgid) with text (menu) and callback (cback) in the chat specified in setChatID OR pass the chat id
uint8_t editMenuCallback(int32_t msgid, String menu, String cback);
uint8_t editMenuCallback(int32_t msgid, String menu, String cback, String id);


// ============== GROUP ==============
// for all group commands, the bot must be an admin in the chat!

// set the group name in the chat specified in setChatID OR pass the chat id
uint8_t setChatTitle(String& title);
uint8_t setChatTitle(String& title, String& id);

// set the description of the group in the chat specified in setChatID OR pass the chat id
uint8_t setChatDescription(String& description);
uint8_t setChatDescription(String& description, String& id);

// pin message with ID msgid in chat specified in setChatID OR pass chat id
uint8_t pinMessage(int32_t msgid);
uint8_t pinMessage(int32_t msgid, String& id);

// unpin the message with ID msgid in the chat specified in setChatID OR pass the chat id
uint8_t unpinMessage(int32_t msgid);
uint8_t unpinMessage(int32_t msgid, String& id);

// unpin all messages in the chat specified in setChatID OR pass the chat id
uint8_t unpinAll();
uint8_t unpinAll(String& id);


// ============= API COMMAND ==============
// send an API command in the chat specified in setChatID OR pass the chat id (the id will be added to the command itself)
// (command example: "/sendSticker?sticker=123456")
uint8_t sendCommand(String&cmd);
uint8_t sendCommand(String& cmd, String& id);


// ================ SERVICE ===============
int32_t lastBotMsg(); // ID of the last message sent by the bot
int32_t lastUsrMsg(); // ID of the last message sent by the user
String chatIDs; // the string specified in setChatID, for debugging and editing the list

uint8_t sendRequest(String&req); // send request (https://api.telegram.org/bot...)
void autoIncrement(boolean incr); // auto increment messages (enabled by default)
void incrementID(uint8_tval); // manually increment ID by val


// ================ TIME =================
FB_Time getTime(int16_t gmt); // get the current time, specify the time zone (for example, Moscow 3) in hours or minutes
booltimeSynced(); // check if the time is synchronized
uint32_t getUnix(); // get the current unix time

// FB_Time structure
uint8_t second; // seconds
uint8_t minute; // minutes
uint8_t hour; // clock
uint8_t day; // day of the month
uint8_tmonth; // month
uint8_tdayWeek; // day of the week (Mon..Sun 1..7)
uint16_t year; // year
StringtimeString(); // get the time string in HH:MM:SS format
String dateString(); // get date string in DD.MM.YYYY format


// =============== STATUS ================
// Many functions return status:
// 0 - waiting
// 1 - OK
// 2 - Full
// 3 - Telegram error
// 4 - Connection error
// 5 - chat ID not set
// 6 - multiple send, hundredКлюква тус неизвестен
// 7 - не подключен обработчик


// ========== ДЕФАЙНЫ НАСТРОЕК ==========
// объявлять ПЕРЕД подключением библиотеки
#define FB_NO_UNICODE   // отключить конвертацию Unicode для входящих сообщений (чуть ускорит программу)
#define FB_NO_URLENCODE // отключить конвертацию urlencode для исходящих сообщений (чуть ускорит программу)
```

<a id="usage"></a>
## Использование

<a id="chatid"></a>
### ID чата/чатов
В библиотеке реализован необязательный "белый список" ID чатов, в которых работает бот. По умолчанию отключен.
- Устанавливается через `setChatID()`, куда можно передать одиночный ID или сразу несколько через запятую: `setChatID("id1,id2,id3")`
- Можно редактировать строку `chatIDs` напрямую как член класса
- Все функции отправки будут отправлять данные в заданный чат/чаты, если не указать его вручную в функции

<a id="inbox"></a>
### Парсинг сообщений
Сообщения автоматически читаются в `tick()`, при поступлении нового сообщения вызывается указанная функция-обработчик. Но тут есть варианты:
- Если задан ID чата/чатов (через `setChatID()`) - происходит автоматическое отсеивание сообщений НЕ из указанных чатов
- Если ID чата/чатов не задан (через `setChatID()`) - сообщение будет обработано, т.е. вызван обработчик

Обработчик подключается при помощи `attach(FB_msg&)`
- Создаём в скетче свою функцию вида `void функция(FB_msg& сообщение)`
- Вызываем `attach(функция)`
- Эта функция будет автоматически вызвана при входящем сообщении, если ID чата совпадают или не настроены
- Если обработчик не подключен - сообщения не будут опрашиваться!
- Внутри этой функции можно пользоваться переданной переменной `сообщение`, которая имеет тип `FB_msg` (структура) и содержит в себе:
    - `String userID` - ID пользователя
    - `String username` - имя пользователя (в API Telegram это first_name)
    - `String chatID` - ID чата
    - `int32_t messageID` - ID сообщения в чате
    - `String text` - текст сообщения или попдпись к файлу
    - `String data` - callback дата сообщения (если есть)
    - `bool query` - запрос
    - `bool edited` - сообщение отредактировано
    - `bool isBot` - сообщение от бота
    - `bool OTA` - запрос на OTA обновление (получен .bin файл)
    - `uint32_t unix` - время сообщения
    
С версии 2.11 добавлен метод `toString()`, позволяющий вывести строкой всё содержимое структуры
```cpp
  Serial.println(msg.toString())
```

> Примечание: Телеграм разделяет текст на несколько сообщений, если длина текста превышает ~4000 символов! Эти сообщения будут иметь разный ID в чате.

<a id="tick"></a>
### Тикер
Для опроса входящих сообщений нужно подключить обработчик сообщений и вызывать `tick()` в главном цикле программы `loop()`, опрос происходит по встроенному таймеру. 
По умолчанию период опроса установлен 3600 миллисекунд. Можно опрашивать чаще (сменить через `setPeriod()`), но Телеграм иногда тупит и 
при частом опросе запрос может выполняться ~3 секунды вместо 60 миллисекунд! На это время программа "зависает" внутри `tick()`. 
При периоде ~3600 мс этого не происходит, поэтому я сделал его по умолчанию.

<a id="example"></a>
### Минимальный пример
```cpp
void setup() {
  // подключаемся к WiFi
  bot.attach(newMsg);   // подключаем обработчик сообщений
}

void newMsg(FB_msg& msg) {
  // выводим имя юзера и текст сообщения
  //Serial.print(msg.username);
  //Serial.print(", ");
  //Serial.println(msg.text);
  
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());
}

void loop() {
  bot.tick();
}
```

<a id="msgid"></a>
### Обращение к сообщениям
Для редактирования и удаления сообщений и меню, а также закрепления сообщений, нужно знать ID сообщения.
- ID входящего сообщения приходит в обработчик входящих сообщений
- ID последнего принятого сообщения можно получить из `lastUsrMsg()`
- ID последнего отправленного ботом сообщения можно получить из `lastBotMsg()`

Будьте внимательны с ID чата, у всех чатов своя нумерация ID сообщений!

<a id="sticker"></a>
### Отправка стикеров
Для отправки стикераCranberries need to know the ID of the sticker. Send the desired sticker to the *@idstickerbot* bot, it will send the sticker ID.
This ID must be passed to the `sendSticker()` function.

<a id="menu"></a>
### Menu
> Note: for all menu options *not produced* url encode. Avoid the `#` and `&` characters, or use an already encoded url!

To send the menu, a string with button names and special formatting is used:
- `\t` - horizontal separation of buttons
- `\n` - vertical separation of buttons
- Extra spaces are cut automatically

3x1 menu example: `"Menu1 \t Menu2 \t Menu3 \n Menu4"`

Result:
```cpp
 _______________________
| | | |
| Menu1 | menu2 | Menu3 |
|_______|_______|_______|
| |
| M e n u 4 |
|_______________________|
```

<a id="basic"></a>
### Regular menu
Large menu at the bottom of the chat.
```cpp
showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Pressing the button sends the text from the button (the `text` message field).

<a id="inline"></a>
### Inline menu
Menu in the message. Requires a menu name.
```cpp
inlineMenu("MyMenu", "Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Pressing the button sends the menu name (the `text` message field) and the text from the button (the `data` message field).

<a id="callb"></a>
### Inline menu with callback
Menu in the message. Allows you to set a unique text for each button, which will be sent by the bot along with the menu name.
The list of callbacks is listed separated by commas in the order of the menu buttons:
```cpp
String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
String cback1 = F("action1,action2,action3,back");
bot.inlineMenuCallback("Menu 1", menu1, cback1);
```
Clicking the button sends the menu name (the `text` message field) and the specified data (the `data` message field).
- (Since 2.11) if callback is set as http/https address, button will automatically become **link button**

<a id="answer"></a>
### Response to callback
When you click on the inline menu button, a callback is sent to the bot, the `query` flag will be raised in the message handler. The Telegram server will wait for a response.
You can respond to the callback with:
- `answer(text, FB_NOTIF)` - popup notification text
- `answer(text, FB_ALERT)` - warning window and OK button

You need to answer **inside the message handler**! Example:
```cpp
void newMsg(FB_msg& msg) {
  if (msg.query) bot.answer("Hello!", true);
}
```

> If nothing is answered, the library itself will send an empty response and the "timer" on the button will disappear.

<a id="unix"></a>
### Time module
The library has a data type `FB_Time`, which is a structure with fields:
```cpp
uint8_t second; // seconds
uint8_t minute; // minutes
uint8_t hour; // clock
uint8_t day; // day of the month
uint8_tmonth; // month
uint8_tdayWeek; // day of the week (Mon..Sun 1..7)
uint16_t year; // year
```

When creating a structure, you can specify unix time and time zone in hours or minutes (for example, 3 hours OR 180 minutes for Moscow (UTC+3:00),
330 minutes for India (UTC+5:30)). After that, you can take the desired time values:

```cpp
FB_Time t(1651694501, 3);
Serial.print(t.hour);
serial print(':');
Serial.print(t.minute);
serial print(':');
Serial.print(t.second);
Serial print(' ');
Serial.print(t.day);
serial print(':');
Serial.print(t.month);
serial print(':');
serial.println(t.year);
```

Since version 2.9, the library can display formatted time (String):
```cpp
Serial.print(t.timeString()); // HH:MM:SS
Serial print(' ');
Serial.println(t.dateString()); // DD.MM.YYYY
```

<a id="time"></a>
### Message received time
In the incoming message handler, the `FB_msg` structure has a `unix` field that stores the time of the message in unix format.
To translate into a more readable format, we act according to the scheme described above:
```cpp
void newMsg(FB_msg& msg) {
  FB_Time t(msg.unix, 3); // passed unix and timezone
  Serial.print(t.timeString());
  Serial print(' ');
  Serial.println(t.dateString());
}
```

<a id="rtc"></a>
### Real time clock
In response to any message from the bot, the server withКлюква ообщает время отправки в формате unix. С версии 2.6 это время парсится 
библиотекой и **счёт продолжается дальше** при помощи стандартных функций времени. Таким образом достаточно один раз отправить 
сообщение после включения платы, чтобы библиотека синхронизировала часы. При дальнейших отправках время также будет синхронизироваться 
и уточняться, т.к. вычисляемое средствами esp время будет уходить (~2 секунды в сутки). Инструменты:

- `uint32_t getUnix()` - вернёт текущее время в unix формате или `0`, если время не синхронизировано.
- `bool timeSynced()` - вернёт `true` если часы синхронизированы. 
- `FB_Time getTime(gmt)` - нужно передать свой часовой пояс, она вернёт `FB_Time`.

Таким образом получить время можно двумя способами (см. пример timeTest):
```cpp
FB_Time t = bot.getTime(3);
// или
FB_Time t(bot.getUnix(), 3);
```

<a id="ota"></a>
### OTA обновление прошивки
С версии библиотеки 2.13 появилось обновление прошивки "по воздуху" (OTA) через чат. Для обновления нужно:
- Скомпилировать программу в файл: *Arduino IDE/Скетч/Экспорт бинарного файла* (файл **.bin** появится в папке со скетчем)
- Отправить файл в чат с ботом, можно добавить подпись
- Файл будет обработан как обычное входящее сообщение от пользователя
    - Подпись к файлу можно получить из поля `text`
    - Будет поднят флаг `OTA`
- Для запуска процесса обновления нужно вызвать `update` внутри обработчика сообщений
- В тот же чат чат будет отправлен статус обновления (*OK* или *error*)
- После успешного обновления esp перезагрузится

```cpp
// обновить, если просто прислали bin файл
if (msg.OTA) bot.update();

// обновить, если файл имеет нужную подпись
if (msg.OTA && msg.text == "update") bot.update();

// обновить, если прислал известный человек (админ)
if (msg.OTA && msg.chatID == "123456") bot.update();
```


<a id="versions"></a>
## Версии
- v1.0
- v1.1 - оптимизация
- v1.2 - можно задать несколько chatID и отправлять в указанный чат
- v1.3 - добавлена возможность задать текст при открытии и закрытии меню
- v1.3.1 - исправлены ошибки с 1.3
- v1.4 - добавлена возможность удалять сообщения
- v1.5 - оптимизация, возможность смены токена, новый парсинг сообщений (id, имя, текст)
- v1.5.1 - получаем также ID сообщения
- v1.6 - добавлен режим FB_DYNAMIC_HTTP, чтение имени пользователя
- v1.7:
  - Убрал динамический режим FB_DYNAMIC_HTTP, работает слишком медленно
  - Исправил warningи
  - Починил работу бота в "группах" (отрицательный ID чата)
  - Оптимизация памяти
  - Ускорил работу
  - Пофиксил работу через раз в сценарии "эхо"
  
- v2.0:
    - Убрал минимум в 3200 мс
    - Добавил обработку Юникода (русский язык, эмодзи). Спасибо Глебу Жукову!
    - Из меню удаляются лишние пробелы, работать стало проще
    - Поддержка esp32
    - Большая оптимизация
    - Добавил коллбэки в inlineMenu
    - Добавил ID юзера
    - Добавил редактирование сообщений и кучу всего

- v2.1: 
    - Ещё оптимизация
    - Добавил форматирование текста (markdown, html)
    - Добавил ответ на сообщение

- v2.2:
    - Большая оптимизация памяти и производительности
    - Добавил notify() - уведомления от сообщений бота
    - Добавил единоразовый показ клавиатуры
    
- v2.3: Небольшая оптимизация
- v2.4: Добавил url encode для текста сообщений
- v2.5: Добавил флаги в FB_msg: сообщение отредактировано и сообщение отправлено ботом. Улучшил парсинг текста
- v2.6: Добавил встроенные часы реального времени
- v2.7: Добавил отправку стикеров
- v2.8: Убрал лишний вывод в сериал, GMT можно в минутах
- v2.9: Исправлена бага в парсинге, парсинг ускорен, добавлен вывод форматированного времени, добавлена фамилия и время сообщения
- v2.10: Добавлены функции для изменения названия и описания чата, закрепления и открепления сообщений. Убраны edit/deleteMessageID, editMenuID
- v2.11: 
    - Оптимизация, исправление багов
    - Callback data теперь парсится отдельно в data
    - Переделана работа с callback
    - Добавлен toString() для FB_msg для отладки
    - В callback добавлена обработка url адcranberry res
    - Removed first_name and last_name (preserving legacy)
    - usrID and ID renamed to userID and messageID (preserving legacy)
    - Completely removed the old incoming message handler

- v2.12: examples fixed, isBot parsing fixed, long message protection mechanism redone, initialization redone
- v2.13: Memory optimization. Added OTA update

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!