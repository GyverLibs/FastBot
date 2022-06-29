This is an automatic translation, may be incorrect in some places. See sources and examples!

# FastBot
Very simple and fast library for telegram bot on esp8266/esp32
- Works on standard libraries without SSL
- Optional "whitelist" chat IDs
- Check for updates manually or by timer
- Send/delete/edit/reply messages
- Works (reading and sending) in chats, groups, channels
- Change the name and description of the chat
- Pin/unpin messages
- Send stickers, send with markdown/html formatting
- Displaying a regular and inline menu with support for link buttons
- Unicode support (other languages ​​+ emoji) for incoming messages
- Built-in urlencode for outgoing messages
- Built-in real time clock with synchronization from the Telegram server
- Possibility of OTA firmware update with .bin file from Telegram chat

### Optional
Use the [CharDisplay] library(https://github.com/GyverLibs/CharDisplay) to display graphs and draw in chat!

### Compatibility
ESP8266 (SDK v2.6+), ESP32

## Documentation and projects
Detailed tutorials on working with the Telegram bot using this library can be found on the [GyverKIT Arduino site] (https://kit.alexgyver.ru/tutorials-category/telegram/)

## Comparison with Universal-Arduino-Telegram-Bot
[Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)

For comparison, a minimal example was used with sending a message to the chat and outputting incoming messages to the series:
- **send** - send a message to the chat
- **update** - check incoming messages
- **free heap** - amount of free RAM while the program is running

| library | Flash, B | SRAM, B | send, ms | update, ms | free heap, B |
|-----------|----------|---------|----------|----- -------|--------------|
| Univ..Bot | 400004 | 29848 | 2000 | 1900 | 38592 |
| fastbot | 393220 | 28036 | 70 | 70 | 37552 |
| diff | 6784 | 1812 | 1930 | 1830 | 1040 |

- FastBot is almost 7kB lighter than Flash and 2kB SRAM, but takes up 1kB more SRAM while the program is running. Total lighter by 2-1 = 1 kB of SRAM.
- FastBot processes chat and sends messages much faster (by 2 seconds) due to manual parsing of the server response and statically allocated HTTP clients
- The test was carried out in the normal mode of operation of FastBot. When `FB_DYNAMIC` is activated, the library will take 10kb less memory, but it will work slower:
  - Free heap: 48000 kB
  - Sending a message: 1 second
  - Update request: 1 second

## Content
- [Install](#install)
- [Initialization](#init)
- [Documentation](#docs)
- [Usage](#usage)
    - [Sending messages](#send)
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
    - [Firmware update from chat](#ota)
    - [Text styling](#textmode)
    - [All sorts of tricks](#tricks)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installedcranberry vka
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
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features
- Through the IDE library manager: find the library as during installation and click "Update"
- Manually: **delete the folder with the old version**, and then put the new one in its place. "Replacement" cannot be done: sometimes new versions delete files that remain after replacement and can lead to errors!


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
void setChatID(int64_t id); // the same, but in int64_t. Pass 0 to disable
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

uint8_t sendRequest(String&req); // send request(https://api.telegram.org/bot...)
void autoIncrement(boolean incr); // auto increment messages (enabled by default)
void incrementID(uint8_tval); // manually increment ID by val


// ============== MESSAGE ===============
// FB_msg structure
String&userID; // user ID
String&username; // nickname (in API this is first_name)
String&chatID; // chat ID
int32_t messageID; // message ID
String&text; // text
String&data; // callback date
bool query; // request
bool& edited; // message edited
bool isBot; // message from bot
bool OTA; // request for OTA update
uint32_t unix; // message time
String&fileName; // File name
String&replyText; // response text, if any


// ================ TIME =================
FB_Time getTime(int16_t gmt); // get the current time, specify the time zone (for example, Moscow 3) in hours or minutes
booltimeSynced(); // check if the time is synchronized
uint32_t getUnix(); // get the current unix time

// FB_Time structure
uint8_t second; // seconds
uint8_t minute; // minutes
uint8_t hour; // watch
uint8_t day; // day of the month
uint8_tmonth; // month
uint8_tdayWeek; // day of the week (Mon..Sun 1..7)
uint16_t year; // year
StringtimeString(); // time string in HH:MM:SS format
String dateString(); // date string in DD.MM.YYYY format


// =============== STATUS ================
// Many functions return status:
// 0 - waiting
// 1 - OK
// 2 - Full
// 3 - Telegram error
// 4 - Connection error
// 5 - chat ID not set
// 6 - multiple send, status unknown
// 7 - handler not connected


// =============== UTILITIES ===============
void FB_unicode(String &s); // translate unicode
void FB_urlencode(String& s, String& dest); // urlencode from s to dest

int64_t FB_str64(const String &s); // transfer from String to int64_t
String FB_64str(int64_t id); // transfer from int64_t to String


// ========== DEFINE SETTINGS ===========
// declare BEFORE linking the library
#define FB_NO_UNICODE // disable Unicode conversion for incoming messages (slightly speed up the program)
#define FB_NO_URLENCODE // disable urlencode conversion for outgoing messages (slightly speeds up the program)
#define FB_NO_OTA // disable support for OTA updates from chat
#define FB_DYNAMIC // enable dynamic mode: the library takes longer to execute the request, but takes up 10 kb less memory in SRAM
```

<a id="usage"></a>
## Usage

<a id="send"></a>
## Sending messages
To send to a chat (messages, stickers, menus, and so on), the ID of the chat to which the message will be sent must be specified. You can specify
several IDs, separated by commas, within one line. There are two ways to specify an ID:
- Directly to the sending function, they all have this option (see documentation above)
```cpp
bot.sendMessage("Hello!", "123456"); // in one chat
bot.sendMessage("Hello!", "123456,7891011"); // two chats
```
- Set ID via `setChatID()` and all sends will go to these chats/chats if no other ID is specified in the sending function
```cpp
bot.setChatID("123456"); // one chat
//bot.setChatID("123456,7891011"); // multiple chats
// ...
bot.sendMessage("Hello!"); // will go to "123456"
bot.sendMessage("Hello!", "112233"); // will go to "112233"
```
> Note: Telegram splits the text into several messages if the text length exceeds ~4000 characters! These messages will have a different messageID in the chat.

### Limits
Telegram sets the following limits on **sending** messages by the bot ([documentation](https://core.telegram.org/bots/faq#my-bot-is-hitting-limits-how-do-i-avoid-this ))
- To chat: no more than once per second. *You can send more often, but the message may not reach*
- To a group: no more than 20 messages per minute
- Total limit: no more than 30 messages per secondКлюква ду

Также бот может читать сообщения, с момента отправки которых прошло меньше 24 часов.

<a id="inbox"></a>
## Парсинг сообщений
Сообщения автоматически запрашиваются и читаются в `tick()`, при поступлении нового сообщения вызывается указанная функция-обработчик:
- Создаём в скетче свою функцию вида `void функция(FB_msg& сообщение)`
- Вызываем `attach(функция)`
- Эта функция будет автоматически вызвана при входящем сообщении, если ID чата совпадает или не настроен
- Если обработчик не подключен - сообщения не будут проверяться
- Внутри этой функции можно пользоваться переданной переменной `сообщение`, которая имеет тип `FB_msg` (структура) и содержит в себе:
    - `String userID` - ID пользователя
    - `String username` - имя пользователя или канала
    - `String chatID` - ID чата
    - `int32_t messageID` - ID сообщения в чате
    - `String text` - текст сообщения или попдпись к файлу
    - `String replyText` - текст ответа, если он есть
    - `String data` - callback данные из меню (если есть)
    - `bool query` - запрос
    - `bool edited` - сообщение отредактировано
    - `bool isBot` - сообщение от бота
    - `bool OTA` - запрос на OTA обновление (получен .bin файл)
    - `uint32_t unix` - время сообщения
    - `String fileName` - имя файла

А также `String toString()` - вся информация из сообщения, удобно для отладки (с версии 2.11)

**Примечания:**
- Телеграм разделяет текст на несколько сообщений, если длина текста превышает ~4000 символов! Эти сообщения будут иметь разный messageID в чате.
- При ответе на сообщение библиотека парсит текст исходного сообщения, а не ответа

### Белый список
В библиотеке реализован механизм белого списка: можно указать в `setChatID()` ID чата (или нескольких через запятую), сообщения из которого будут приниматься. 
Сообщения из остальных чатов будут игнорироваться.

<a id="tick"></a>
## Тикер
Для опроса входящих сообщений нужно подключить обработчик сообщений и вызывать `tick()` в главном цикле программы `loop()`, опрос происходит по встроенному таймеру. 
По умолчанию период опроса установлен 3600 миллисекунд.

Можно опрашивать чаще (сменить период через `setPeriod()`), но лично у меня с ~2021 года сервер Телеграм стал отвечать не 
раньше, чем через ~3 секунды. Если запрашивать обновления чаще этого периода, программа зависает внутри `tick()` (внутри GET запроса) 
в ожидании ответа сервера на остаток от 3 секунд. При периоде ~3600 мс этого не происходит, поэтому я сделал его по умолчанию. 
Возможно это зависит от провайдера или страны.

<a id="example"></a>
## Минимальный пример
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
## Обращение к сообщениям
Для редактирования и удаления сообщений и меню, а также закрепления сообщений, нужно знать ID сообщения (его номер в чате):
- ID входящего сообщения приходит в обработчик входящих сообщений
- ID последнего принятого сообщения можно получить из `lastUsrMsg()`
- ID последнего отправленного ботом сообщения можно получить из `lastBotMsg()`

Будьте внимательны с ID чата, у всех чатов своя нумерация сообщений!

<a id="sticker"></a>
## Отправка стикеров
Для отправки стикера нужно знать ID стикера. Отправь нужный стикер боту *@idstickerbot*, он пришлёт ID стикера. 
Этот ID нужно передать в функцию `sendSticker()`.

<a id="menu"></a>
## Меню
> Примечание: для всех вариантов меню *не производится* url encode. Избегайте символов `#` и `&` или используйте уже закодированный url!

Для отправки меню используется строка с именами кнопок и специальным форматированием:
- `\t` - горизонтальное разделение кнопок
- `\n` - вертикальное разделение кнопок
- Лишние пробелы вырезаются автоматически

Пример меню 3x1: `"Menu1 \t Menu2 \t Menu3 \n Menu4"`

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
## Regular menu
Large menu at the bottom of the chat.
```cpp
showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Pressing the button sends the text from the button (the `text` message field).

<a id="inline"></a>
## Inline menu
Menu in the message. Requires a menu name.
```cpp
inlineMenu("MyMenu", "Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Pressing the button sends the menu name (the `text` message field) and the text from the button (the `data` message field).

<a id="callb"></a>
## Inline menu with callback
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
## Response to callback
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
## Time module
The library has a data type `FB_Time`, which is a structure with fields:
```cpp
uint8_t second; // seconds
uint8_t minute; // minutes
uint8_t hour; // watch
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
## Message received time
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
## Real time clock
In response to any message from the bot, the server reports the time of sending in unix format. Since version 2.6 this time is parsed
library and **count continues on** using standard time functions. Thus, it is enough to send once
message after the board is turned on, so that the library synchronizes the clock. The time will also be synchronized on further sendings.
and to be specified, because the time calculated by means of esp will go away (~ 2 seconds per day). Tools:

- `uint32_t getUnix()` - returns the current time in unix format or `0` if the time is out of sync.
- `booltimeSynced()` - will return `true` if clocks are synchronized.
- `FB_Time getTime(gmt)` - you need to pass your time zone, it will return `FB_Time`.

Thus, there are two ways to get the time (see the timeTest example):
```cpp
FB_Time t = bot.getTime(3);
// or
FB_Time t(bot.getUnix(), 3);
```

<a id="ota"></a>
## Firmware update from chat
Since version 2.13 of the library, a firmware update has appeared "over the air" (OTA) via chat. To update you need:
- Compile the program into a file: *Arduino IDE/Sketch/Export Binary File* (the **.bin** file will appear in the sketch folder)
- Send a file to a chat with a bot, you can add a signature
- The file will be processed as a normal incoming message from the user
    - File signature can be obtained from the `text` field
    - `OTA` flag will be raised
- To start the update process, you need to call `update` inside the message handler
- In the same chat the update status will be sent (*OK* or *error*)
- After a successful update, the esp will reboot

```cpp
// update if just sent a bin file
if (msg.OTA) bot.update();

// update if the file has the desired signature
if (msg.OTA && msg.text == "update") bot.update();

// update if sent by a famous person (admin)
if (msg.OTA && msg.chatID == "123456") bot.update();
```

<a id="textmode"></a>
## Text decoration
The library supports the design of text in messages. The layout markup is selected using `setTextMode(mode)`, where `mode` is:
- `FB_TEXT` - default (decoration disabled)
- `FB_MARKDOWN` - Markdown v2 markup
- `FB_HTML` - HTML markup

Available tags are described in [Telegram API](https://core.telegram.org/bots/api#formatting-options). For example for Markdown:
```cpp
bot.setTextMode(FB_MARKDOWN);
bot.sendMessage(F("*Bold*, ~Strike~, `code`, [alexgyver.ru](https://alexgyver.ru/)"));
```

Output to chat: **Bold**, ~~Strike~~, `code`, [alexgyver.ru](https://alexgyver.ru/)

> **Attention!** In FB_MARKDOWN mode, you cannot use `! + #`, the message will not be sent. It may be possible to fix it in the future (the problem of urlencode and escaping reserved characters).


<a id="tricks"></a>
## Tricks
### Reboot
Messages are marked as read at the next (relative to the current message handler) update in tick(), i.e. after at least the configured timeout.
If you want to reload esp on command, then here's the design
```cpp
void message(FB_msg &msg) {
  if (msg.text == "restart") ESP.restart();
}
```
Will lead to a bootloop (infinite reboot), because the message will not be marked as read. You can raise the flag to go into reboot by calling tickManual first:
```cpp
bool res = 0;
void message(FB_msg &msg) {
  if (msg.text == "restart") res = 1;
}
void loop() {
  if (res) {
    bot.tickManual();
    ESP.restart();
  }
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - optimization
- v1.2 - you can set several chatIDs and send to the specified chat
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
    -Added a reply to a post

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
- v2.13: Memory optimization. Added OTA update
- v2.14: Improved ID string parsing, added OTA disable, added group/channel name parsing to username
- v2.15: ESP32 library curve patch
- v2.16: added fileName output, fixed unsent messages in Markdown mode
- v2.17: displaying the text of the message to which the user replied + correct work with menu in groups
- v2.17.1: minor fix https://github.com/GyverLibs/FastBot/issues/12
- v2.18: added FB_DYNAMIC mode: the library takes longer to execute a request, but takes up 10 kb less memory in SRAM

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!

When reporting bugs or incorrect work of the library, it is necessary to indicate:
- Library version
- What MK is used
- SDK version (for ESP)
- Arduino IDE version
- Whether built-in examples work correctly, which use functions and constructs that lead to a bug in your code
- What code was loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimal code