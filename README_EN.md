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
- [Example](#example)
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
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
```cpp
fastbot bot
FastBot bot(token); // token - unique bot code, taken from BotFather
```

<a id="docs"></a>
## Documentation
```cpp
// ============== SETTINGS ==============
void setToken(String token); // change/set bot token
void setChatID(String chatID); // mouthChat ID (white list), optional. Multiple separated by commas ("id1,id2,id3")
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
// 2 - ovf full
// 3 - Telegram error
// 4 - Connection error
// 5 - chat ID not set
// 6 - multiple send, status unknown
// 7 - handler not connected


// ========== DEFINE SETTINGS ==========
// declare BEFORE linking the library
#define FB_NO_UNICODE // disable Unicode conversion for incoming messages (slightly speed up the program)
#define FB_NO_URLENCODE // disable urlencode conversion for outgoing messages (slightly speeds up the program)
```

<a id="usage"></a>
## Usage
### ID of the chat(s)
The library implements an optional "white list" of chat IDs in which the bot works. Disabled by default.
- Set via `setChatID()`, where you can pass a single ID or several at onceКлюква о через запятую: `setChatID("id1,id2,id3")`
- Можно редактировать строку `chatIDs` напрямую как член класса
- Все функции отправки будут отправлять данные в заданный чат/чаты, если не указать его вручную в функции

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
    - `String text` - текст сообщения
    - `String data` - callback дата сообщения (если есть)
    - `bool query` - запрос
    - `bool edited` - сообщение отредактировано
    - `bool isBot` - сообщение от бота
    - `uint32_t unix` - время сообщения
    
С версии 2.11 добавлен метод `toString()`, позволяющий вывести строкой всё содержимое структуры
```cpp
  Serial.println(msg.toString())
```

> Примечание: Телеграм разделяет текст на несколько сообщений, если длина текста превышает ~4000 символов! Эти сообщения будут иметь разный ID в чате.

### Тикер
Для опроса входящих сообщений нужно подключить обработчик сообщений и вызывать `tick()` в главном цикле программы `loop()`, опрос происходит по встроенному таймеру. 
По умолчанию период опроса установлен 3600 миллисекунд. Можно опрашивать чаще (сменить через `setPeriod()`), но Телеграм иногда тупит и 
при частом опросе запрос может выполняться ~3 секунды вместо 60 миллисекунд! На это время программа "зависает" внутри `tick()`. 
При периоде ~3600 мс этого не происходит, поэтому я сделал его по умолчанию.

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

### Обращение к сообщениям
Для редактирования и удаления сообщений и меню, а также закрепления сообщений, нужно знать ID сообщения.
- ID входящего сообщения приходит в обработчик входящих сообщений
- ID последнего принятого сообщения можно получить из `lastUsrMsg()`
- ID последнего отправленного ботом сообщения можно получить из `lastBotMsg()`

Будьте внимательны с ID чата, у всех чатов своя нумерация ID сообщений!

### Отправка стикеров
Для отправки стикера нужно знать ID стикера. Отправь нужный стикер боту *@idstickerbot*, он пришлёт ID стикера. 
Этот ID нужно передать в функцию `sendSticker()`.

### Меню
> Примечание: для всех вариантов меню *не производится* url encode. Избегайте символов `#` и `&` или используйте уже закодированный url!

Для отправки меню используется строка с именами кнопок и специальным форматированием:
- `\t` - горизонтальное разделение кнопок
- `\n` - вертикальное разделение кнопок
- Лишние пробелы вырезаются автоматически

Пример меню 3x1: `"Menu1 \t Menu2 \t Menu3 \n Menu4"`

Результат:
```cpp
 _______________________
|       |       |       |
| Menu1 | Menu2 | Menu3 |
|_______|_______|_______|
|                       |
|       M e n u 4       |
|_______________________|
```

### Обычное меню
Большое меню в нижней части чата.
```cpp
showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Pressing the button sends the text from the button (the `text` message field).

### Inline menu
Menu in the message. Requires a menu name.
```cpp
inlineMenu("MyMenu", "Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Pressing the button sends the menu name (the `text` message field) and the text from the button (the `data` message field).

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

### Message received time
In the incoming message handler, the `FB_msg` structure has a `unix` field that stores the time of the message in unix format.
To translate into a more readable format, we act according to the scheme described above:
```cpp
void newMsg(FB_msg& msg) {
  FB_Time t(msg.unix, 3); // passed unix and time zone
  Serial.print(t.timeString());
  Serial print(' ');
  Serial.println(t.dateString());
}
```

### Real time clock
In response to any message from the bot, the server reports the time of sending in unix format. Since version 2.6 this time is parsed
library and **count continues on** using standard time functions. Thus, it is enough to send once
message after the board is turned on, so that the library synchronizes the clock. The time will also be synchronized on further sendings.
and to be specified, because the time calculated by means of esp will go away (~ 2 seconds per day). Instruments:

- `uint32_t getUnix()` - returns the current time in unix format or `0` if the time is out of sync.
- `bool timeSynced()` - will return `true` if the clock is synchronized.
- `FB_Time getTime(gmt)` - you need to pass your time zone, it will return `FB_Time`.

Thus, there are two ways to get the time (see the timeTest example):
```cpp
FB_Time t = bot.getTime(3);
// or
FB_Time t(bot.getUnix(), 3);
```

<a id="example"></a>

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - optimization
- v1.2 - you can set multiple chatID and send to the specified chat
- v1.3 - added the ability to set text when opening and closing the menu
- v1.3.1 - fixed bugs since 1.3
- v1.4 - added the ability to delete messages
- v1.5 - optimization, the ability to change the token, new message parsing (id, name, text)
- v1.5.1 - get message ID as well
- v1.6 - added FB_DYNAMIC_HTTP mode, reading username
- v1.7:
  - Removed FB_DYNAMIC_HTTP dynamic mode, works too slow
  - Fixed warnings
  - Fixed the work of the bot in "groups" (negative chat ID)
  - Memory optimization
  - Speed ​​up work
  - Fixed work every other time in the "echo" script
  
- v2.0:
    - Removed at least 3200 ms
    - Added Unicode processing (Russian language, emoji). Thanks to Gleb Zhukov!
    - Extra spaces are removed from the menu, it became easier to work
    - Support esp32
    - Big optimization
    - Added callbacks to inlineMenu
    - Added user ID
    - Added editing messages and a bunch of everything

- v2.1:
    - More optimization
    - Added text formatting (markdown, html)
    - Added a reply to the message

- v2.2:
    - Big memory and performance optimizations
    - Added notify() - notifications from bot messages
    - Added a one-time display of the keyboard
    
- v2.3: Minor optimization
- v2.4: Added url encode for message text
- v2.5: Added flags to FB_msg: message edited and message sent by bot. Improved text parsing
- v2.6: Added built-in real time clock
- v2.7: Added sending stickers
- v2.8: Removed extra serial output, GMT can be in minutes
- v2.9: Parsing bug fixed, parsing speeded up, formatted time output added, last name and message time added
- v2.10: Added functions for changing the name and description of the chat, pinning and unpinning messages. Removed edit/deleteMessageID, editMenuID
- v2.11:
    - Optimization, bug fixes
    - Callback data is now parsed separately in data
    - Redesigned work with callback
    - Added toString() for FB_msg for debugging
    - Added processing of url addresses in callback
    - Removed first_name and last_name (preserving legacy)
    - usrID and ID renamed to userID and messageID (preserving legacy)
    - Completely removed the old incoming message handler

- v2.12: examples fixed, isBot parsing fixed, long message protection mechanism redone, initialization redone
    
<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!