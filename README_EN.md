This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/FastBot.svg?color=brightgreen)](https://github.com/GyverLibs/FastBot/releases/latest/download/FastBot.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/FastBot.svg)](https://registry.platformio.org/libraries/gyverlibs/FastBot)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/FastBot?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

|в️в️в️<br>** Appeared[FastBot2](https://github.com/GyverLibs/FastBot2)A faster and much more versatile library for Telegram Bot!**<br>стра️стра️стра️|
| --- |

# FastBot
Multifunctional fast library for telegram bot on esp8266/esp32
- Works on standard libraries
- Optional "white list" ID chats
- Checking updates manually or by timer
- Sending/deleting/editing/responding to communications
- Reading and sending in chats, groups, channels
- Changing the name and description of the chat
- Affiliation/rejection of communications
- Sending stickers
- Messages with markdown/html formatting
- Output of a regular menu
- Inline menu with support for link buttons
- Unicode support (other languages + emoji) for incoming messages
- Built-in urlencode for outgoing messages
- Built-in real-time clock with synchronization from Telegram server
- Possibility of OTA firmware update .bin file from Telegram chat (firmware and SPIFFS)
- Sending files from memory to chat (+ editing)

### Compatibility
ESP8266 (SDK v2.6+), ESP32

## Creating and configuring a bot
- [Instructions on how to create and configure a Telegram bot](https://kit.alexgyver.ru/tutorials/telegram-basic/)
- If you already have a bot, make sure it is *not in webhook mode* (defaulted) or esp will not be able to receive messages!
- In order for the bot to read all the messages in the group (not only)`/команды`), you need to disable the *Group Privacy* settings in the *Bot Settings* chat with *@BotFather*. This setting is on by default!
- For full-fledged work in a group (supergroup), the bot must be made an administrator!

## Limitations
### Telegram limits
Telegram sets the following limits on **sending** messages by a bot.[documentation](https://core.telegram.org/bots/faq#my-bot-is-hitting-limits-how-do-i-avoid-this))
- In chat: no more than once per second. * You can send more often, but the message may not reach *
- In group: no more than 20 messages per minute
- Total limit: no more than 30 messages per second
- The bot can read messages that have been sent less than 24 hours.
- Bot can't face another bot
- bot[see](https://core.telegram.org/bots/faq#why-doesn-39t-my-bot-see-messages-from-other-bots)Messages from other bots in the group

### Others.
- Telegram divides the text into several messages if the length of the text exceeds 4,000 characters! These messages will have a different message ID in the chat
- When responding to a message, the library parses the text of the original message, not the answer.

## Graphic output
Use the library.[CharDisplay](https://github.com/GyverLibs/CharDisplay)To display charts and drawing in chat!

![](https://github.com/GyverLibs/CharDisplay/blob/main/docs/plots.png)

## Documentation and projects
Detailed lessons on working with Telegram bot using this library can be found at[Arduino site set GyverKIT](https://kit.alexgyver.ru/tutorials-category/telegram/)

## Comparison with Universal-Arduino-Telegram-Bot
[Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)  

For comparison, a minimal example was used with sending a message to chat and outputting incoming messages to the series:
- **send** - sending a message to chat
- **update** - check the incoming messages
- **free heap** - the amount of free RAM during the program

| Library   | Flash, B | SRAM, B | send, ms | update, ms | free heap, B |
|-----------|----------|---------|----------|------------|--------------|
| Univ..Bot | 400004   | 29848   | 2000     | 1900       | 38592        |
| FastBot   | 393220   | 28036   | 70       | 70         | 37552        |
| diff      | 6784     | 1812    | 1930     | 1830       | 1040         |

- FastBot is lighter by nearly 7kB of Flash and 2kB of SRAM, but takes up 1kB more in SRAM while the program is running. Totally easier by 2-1 = 1 kB of SRAM.
- FastBot processes chat and sends messages much faster (by 2 seconds) due to manual parsing of the server response and statically allocated HTTP clients.
- The test was conducted in the normal mode of FastBot. When activated`FB_DYNAMIC`The library will take up 10kb less memory, but will run slower.
  - Free heap: 48,000 kB
  - Sending a message: 1 second
  - Request for update: 1 second

## Contents
- [Installation](#install)
- [Initialization](#init)
- [Documentation.](#docs)
- [Use of use](#usage)
    - [Sending messages](#send)
    - [Parsing messages](#inbox)
    - [ticker](#tick)
    - [Minimum example](#example)
    - [Appeal to the communication n](#msgid)
    - [Sending stickers](#sticker)
    - [menu](#menu)
    - [Regular menu](#basic)
    - [Online menu](#inline)
    - [Inline menu with callback](#callb)
    - [Callback response](#answer)
    - [Time module](#unix)
    - [Time of receipt of the communication](#time)
    - [Real-time clocks](#rtc)
    - [Update firmware from chat](#ota)
    - [Drafting of the text](#textmode)
    - [Sending files](#files)
    - [Downloading files](#download)
    - [Location](#location)
    - [All kinds of tricks.](#tricks)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found under the name **FastBot** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/FastBot/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="init"></a>

## Initialization
```cpp
FastBot bot;
FastBot bot(токен); // tokenized
```

<a id="docs"></a>

## Documentation.
```cpp
// ========================================
void setToken(String token);                    // change/set the bot token
void setChatID(String chatID);                  // Installation of chat ID (white list), optional. Can be several times a comma ("id1,id2,id3")
void setChatID(int64_t id);                     // Same thing, but int64 t. Pass 0 to turn off.
void setPeriod(int period);                     // Interview period in MS (by omission 3500)
void setLimit(int limit);                       // Number of messages processed per request, 1..100. (silence). 10)
void setBufferSizes(uint16_t rx, uint16_t tx);  // set the size of the buffer to receive and send, silently. 512 and 512 bytes (esp8266 only)
void skipUpdates();                             // miss out on unread messages
    
void setTextMode(uint8_t mode);                 // Text mode "to send": FB TEXT, FB MARKDOWN, FB HTML (see textMode example)
void notify(bool mode);                         // true/false on/off notifications from bot messages (by default on)
void clearServiceMessages(bool state);          // remove service messages from the chat about changing the name and fixing messages (silent. false)


// ===========================================
void attach(callback);                          // plug-in
void detach();                                  // shutdown


// ================================================================================================================================================================================================================================================================
uint8_t tick();                                 // timer check
uint8_t tickManual();                           // manual


// =========================================================================
// send a message to setChatID chat / chat OR transfer id chat
uint8_t sendMessage(String msg);
uint8_t sendMessage(String msg, String id);

// edit the message (msgid) in the chat specified in the setChatID OR transfer id chat
uint8_t editMessage(int32_t msgid, String text);
uint8_t editMessage(int32_t msgid, String text, String id);

// respond to a message with id (replyID) in the chat specified in setChatID OR specify the chat
uint8_t replyMessage(String msg, int32_t replyID);
uint8_t replyMessage(String msg, int32_t replyID, String id);

// send the sticker to the chat / chat specified in setChatID OR transfer id chat
uint8_t sendSticker(String stickerID);
uint8_t sendSticker(String stickerID, String id);

// respond to callback with text (text) and mode (alert): FB NOTIF - notification in chat, FB ALERT - window with OK button
uint8_t answer(String text, bool alert);

// Do not automatically respond to the query of this update
void noAnswer();

//Send a notification that the bot is printing a message
uint8_t sendTyping(const String& id);

// ======================================================
// delete the message from the id (msgid) in the chat specified in the setChatID OR transfer the id chat
// deletes any type of message (text, sticker, online menu)
uint8_t deleteMessage(int32_t msgid);
uint8_t deleteMessage(int32_t msgid, String id);


// ===============================================================
// show the menu (menu) in the chat / chat specified in setChatID OR transfer id chat / chat
uint8_t showMenu(String menu);
uint8_t showMenu(String menu, String id);

// one-time menu (closes when selecting) in the current chat OR transfer id chat
uint8_t showMenu(String menu, true);
uint8_t showMenu(String menu, String id, true);

// hide the menu in the chat / chat specified in setChatID OR transfer id chat / chat
uint8_t closeMenu();
uint8_t closeMenu(String id);


// ======== Normal me with text ============================
// message (msg) + show menu (menu) in setChatID chat / chat OR transfer id chat / chat
uint8_t showMenuText(String msg, String menu);
uint8_t showMenuText(String msg, String menu, String id);

// One-time menu (closes upon selection)
uint8_t showMenuText(String msg, String menu, true);
uint8_t showMenuText(String msg, String menu, String id, true);

// message (msg) + hide the menu in setChatID chat / chat OR transfer id chat / chat
uint8_t closeMenuText(String msg);
uint8_t closeMenuText(String msg, String id);


// ==========================================
// message (msg) from the inline menu (menu) in the chat / chat specified in the setChatID OR transfer id chat / chat
uint8_t inlineMenu(String msg, String menu);
uint8_t inlineMenu(String msg, String menu, String id);

// edit the menu (msgid) with text (menu) in the chat specified in the setChatID OR transfer id chat
uint8_t editMenu(int32_t msgid, String menu);
uint8_t editMenu(int32_t msgid, String menu, String id);


// =====================
// message (msg) with in-line menu (menu) and callback (cbck) in setChatID chat / chat OR transfer id chat / chat
uint8_t inlineMenuCallback(String msg, String menu, String cbck);
uint8_t inlineMenuCallback(String msg, String menu, String cbck, String id);

// edit the menu (msgid) with text (menu) and callback (cback) in the setChatID chat OR transfer id chat
uint8_t editMenuCallback(int32_t msgid, String menu, String cback);
uint8_t editMenuCallback(int32_t msgid, String menu, String cback, String id);


// ========================================================================
// For all group teams, the bot must be an admin in the chat!

// set the group name in the setChatID chat OR transfer id chat
uint8_t setChatTitle(String& title);
uint8_t setChatTitle(String& title, String& id);

// set the group description in the setChatID chat OR transfer id chat
uint8_t setChatDescription(String& description);
uint8_t setChatDescription(String& description, String& id);

// fix the message with ID msgid in the chat specified in setChatID OR transfer id chat
uint8_t pinMessage(int32_t msgid);
uint8_t pinMessage(int32_t msgid, String& id);

// unplug a message with ID msgid in the chat specified in setChatID OR transfer id chat
uint8_t unpinMessage(int32_t msgid);
uint8_t unpinMessage(int32_t msgid, String& id);

// disable all messages in the chat specified in setChatID OR transfer id chat
uint8_t unpinAll();
uint8_t unpinAll(String& id);

// ============================================================================
// download
bool downloadFile(File &f, const String& url);
    
// send a file from the bytebuffer buf length, type and name file name in the chat specified in setChatID OR transfer id chat
uint8_t sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name);
uint8_t sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, const String& id);

// send a File type file and name file in the setChatID chat OR transfer id chat
uint8_t sendFile(File &file, FB_FileType type, const String& name);
uint8_t sendFile(File &file, FB_FileType type, const String& name, const String& id);

// edit the file from the bytebuffer buf length, type type and name file in the msgid message in the chat specified in the setChatID OR transfer the chat id
// Except for the FB VOICE type!
uint8_t editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid);
uint8_t editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid, const String& id);

// edit the File file type and name file name in the msgid message in the setChatID chat OR transfer the id chat
// Except for the FB VOICE type!
uint8_t editFile(File &file, FB_FileType type, const String& name, int32_t msgid);
uint8_t editFile(File &file, FB_FileType type, const String& name, int32_t msgid, const String& id);

// where FB FileType is the file type
FB_PHOTO - картинка
FB_AUDIO - аудио
FB_DOC - документ
FB_VIDEO - видео
FB_GIF - анимация
FB_VOICE - голосовое сообщение

// ===============================================================
// send an API command in the chat specified in the setChatID OR transfer the id chat (id itself will be added to the command)
// (Example of command: "/sendSticker?sticker=123456")
uint8_t sendCommand(String& cmd);
uint8_t sendCommand(String& cmd, String& id);


// ===================================
int32_t lastBotMsg();               // ID of the last message sent by the bot
int32_t lastUsrMsg();               // ID of the last message sent by the user
String chatIDs;                     // the line specified in setChatID, for debugging and editing the list

uint8_t sendRequest(String& req);   // send a requesthttps://api.telegram.org/bot...)
void autoIncrement(boolean incr);   // Auto increment messages (installed included)
void incrementID(uint8_t val);      // manually increment ID to val


// =====================================================================
// FB msg
String& userID;     // User ID
String& username;   // username (in the API it is first name)
bool isBot;         // user-bot

String& chatID;     // chat ID
int32_t messageID;  // ID message
bool& edited;       // message edited

String& text;       // text
String& replyText;  // text of the answer, if any
bool query;         // request
String& data;       // callback

bool isFile;        // it's a file
String& fileName;   // filename
String& fileUrl;    // file address
bool OTA;           // file - request for OTA update

uint32_t unix;      // timing


// =======================================================
FB_Time getTime(int16_t gmt);   // get the current time, specify the time zone (for example, Moscow 3) in hours or minutes
bool timeSynced();              // Check if the time is synchronized
uint32_t getUnix();             // Get the current unix time

// FB Time
uint8_t second;         // seconds
uint8_t minute;         // minutes
uint8_t hour;           // clockwork
uint8_t day;            // month
uint8_t month;          // month
uint8_t dayWeek;        // day of the week (Mn.vc 1.7)
uint16_t year;          // year
String timeString();    // timeline of the format CH:MM:SS
String dateString();    // Date line of DD.MM format. GHG


// ==============UPDATE ===============================================================================================================================================================================================================================================
uint8_t update();       // OTA firmware update, call inside the handler messages on the OTA flag
uint8_t updateFS();     // OTA SPIFFS update, call inside the OTA flag message handler

// ========================================================================================
// Many functions return status:
// 0 - waiting
// 1 - OK
// 2 - Overflowing
// 3 - Telegram error
// 4 - Connection error
// 5 - no chat ID set
// 6 - Multiple sending, status unknown
// 7 - not connected to the handler
// 8 - file error


// ============================================================================
void FB_unicode(String &s);                 // unicode
void FB_urlencode(String& s, String& dest); // urlencode from s to dest

int64_t FB_str64(const String &s);  // Transfer from String to int64 t
String FB_64str(int64_t id);        // Transfer from int64 t to String


// ===========================
// declare before connecting the library
#define FB_NO_UNICODE       // disable Unicode conversion for incoming messages (slightly speed up the program)
#define FB_NO_URLENCODE     // disable urlencode conversion for outgoing messages (slightly speed up the program)
#define FB_NO_OTA           // disable support for OTA updates from chat
#define FB_DYNAMIC          // Enable dynamic mode: the library takes longer to execute a request, but takes up 10 kB less memory in SRAM
#define FB_WITH_LOCATION    // include an additional location field (containing latitude and longitude) in the message (see examples of location and sunPosition)
```

<a id="usage"></a>

## Use of use
<a id="send"></a>

## Sending messages
To send to the chat (messages, stickers, menus, etc.) must be specified ID chat, which will be sent. Can you say
several IDs by comma, within one line. There are two ways to specify the ID:
- Directly to the sending function, they all have this option (see documentation above)
```cpp
bot.sendMessage("Hello!", "123456");            // one-chat
bot.sendMessage("Hello!", "123456,7891011");    // double-chat
```
- Set the ID through`setChatID()`and all shipments will go to these chats/chats unless another ID is specified in the sending function.
```cpp
bot.setChatID("123456");             // chat
//bot.setChatID("123456,7891011"); // multiple chat rooms
// ...
bot.sendMessage("Hello!");           // Go to 123456.
bot.sendMessage("Hello!", "112233"); // Go to 112233.
```
> Note: Telegram divides the text into several messages if the length of the text exceeds ~4000 characters! These messages will have a different message ID in the chat.

<a id="inbox"></a>

## Parsing messages
Messages are automatically requested and read in`tick()`When a new message is received, the specified processing function is called:
- We create in the sketch our function of the species.`void функция(FB_msg& сообщение)`
- Call in.`attach(функция)`
- This feature will be automatically called when an incoming message is sent if the chat ID matches or is not configured.
- If the processor is not connected, the messages will not be checked.
- Within this function, the transmitted variable can be used.`сообщение`which has a type`FB_msg`(Structure) and contains:
    - `String userID`- User ID
    - `String username`- user or channel name
    - `bool isBot`- message from the bot.
    - `String chatID`- Chat ID.
    - `int32_t messageID`- Message ID in chat
    - `bool edited`- message edited
    - `String text`- text of the message or inscription to the file
    - `String replyText`- text of the answer, if any
    - `String data`Callback data from the menu (if any)
    - `bool query`request
    - `bool isFile`- It's a file.
    - `String fileName`- file name.
    - `String fileUrl`- file address for download
    - `bool OTA`Request for an OTA update (received .bin file)
    - `uint32_t unix`- time of communication
    - `uint32_t update_id`- id update.
    - `String query_id` - id query

And also`String toString()`All information from the message in one line, convenient for debugging (from version 2.11)

### Whitelist
The library has a white list mechanism: you can specify the`setChatID()`ID chat (or several comma), messages from which will be received.
Messages from other chat rooms will be ignored.

<a id="tick"></a>
## ticker
To survey incoming messages, you need to connect the message processor and call`tick()`main-cycle`loop()`The survey takes place on a built-in timer.
The default survey period is set at 3600 milliseconds.

You can ask more often (change the period).`setPeriod()`), but personally, from ~2021, the Telegram server began to respond
earlier than ~3 seconds. If you request updates more often than this period, the program hangs inside.`tick()`(Inside GET request)
waiting for the server to respond for the remainder of 3 seconds. At a period of ~3600ms, this does not happen, so I made it by default.
This may depend on the provider or country.

<a id="example"></a>
## Minimum example
```cpp
void setup() {
  // connect to WiFi
  bot.attach(newMsg);   // connect the message handler
}

void newMsg(FB_msg& msg) {
  // We display the user name and text of the message
  //Serial.print(msg.username);
  //Serial.print(", ");
  //Serial.println(msg.text);
  
  // Provide all information about the message
  Serial.println(msg.toString());
}

void loop() {
  bot.tick();
}
```

<a id="msgid"></a>
## Appeal to the communication n
To edit and delete messages and menus, as well as fix messages, you need to know the ID of the message (its number in the chat):
- The ID of the incoming message comes to the incoming message handler
- The ID of the last received message can be obtained from`lastUsrMsg()`
- The ID of the last message sent by the bot can be obtained from`lastBotMsg()`

Be careful with the chat ID, all chat rooms have their own message numbering!

<a id="sticker"></a>
## Sending stickers
To send a sticker, you need to know the sticker ID. Send the right sticker to the bot *@idstickerbot*, it will send the sticker ID.
This ID must be transferred to the function`sendSticker()`.

<a id="menu"></a>
## menu
> Note: No *url encode is produced for all menu options. Avoid symbols`#`and`&`Or use an already encoded URL!

To send the menu, a line with button names and special formatting is used:
- `\t`- horizontal separation of buttons
- `\n`- vertical separation of buttons
- Extra gaps are cut out automatically

Example 3x1 menu:`"Menu1 \t Menu2 \t Menu3 \n Menu4"`

Result:
```cpp
 _______________________
|       |       |       |
| Menu1 | Menu2 | Menu3 |
|_______|_______|_______|
|                       |
|       M e n u 4       |
|_______________________|
```

<a id="basic"></a>
## Regular menu
A large menu at the bottom of the chat.
```cpp
showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");
```Pressing a button sends text from a button (message field)`text`).

<a id="inline"></a>
## Online menu
Menu message. Requires the menu name.
```cpp
inlineMenu("MyMenu", "Menu1 \t Menu2 \t Menu3 \n Menu4");
```Pressing the button sends the menu name (message field)`text`) and text from the button (message field)`data`).

<a id="callb"></a>
## Inline menu with callback
Menu message. Allows you to set each button a unique text that will be sent by the bot along with the menu name.
The list of callbacks is listed through a comma in order of the menu buttons:
```cpp
String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
String cback1 = F("action1,action2,action3,back");
bot.inlineMenuCallback("Menu 1", menu1, cback1);
```
Pressing the button sends the menu name (message field)`text`) and specified data (message field)`data`).
- (From version 2.11) if the callback is set as an http/https address, the button will automatically become a **link button**

<a id="answer"></a>
## Callback response
When you click on the inline menu button, a callback is sent to the bot, a flag will be raised in the message handler.`query`. The Telegram server will be waiting for a response.
You can answer the callback with:
- `answer(текст, FB_NOTIF)`- notification text pop-up
- `answer(текст, FB_ALERT)`- window with warning and OK button

You have to answer inside the message handler! Example:
```cpp
void newMsg(FB_msg& msg) {
  if (msg.query) bot.answer("Hello!", true);
}
```

> If you do not answer anything, the library will send an empty answer and the “timer” on the button disappears.

<a id="unix"></a>
## Time module
There's a data type in the library.`FB_Time`which is a structure with fields:
```cpp
uint8_t second;     // seconds
uint8_t minute;     // minutes
uint8_t hour;       // clockwork
uint8_t day;        // month
uint8_t month;      // month
uint8_t dayWeek;    // day of the week (Mn.vc 1.7)
uint16_t year;      // year
```

When creating a structure, you can specify unix time and time zone in hours or minutes (for example, 3 hours OR 180 minutes for Moscow (UTC+3:00),
330 minutes for India (UTC+5:30) After that, you can take the necessary time values:

```cpp
FB_Time t(1651694501, 3);
Serial.print(t.hour);
Serial.print(':');
Serial.print(t.minute);
Serial.print(':');
Serial.print(t.second);
Serial.print(' ');
Serial.print(t.day);
Serial.print(':');
Serial.print(t.month);
Serial.print(':');
Serial.println(t.year);
```

With version 2.9, the library can output formatted time (String):
```cpp
Serial.print(t.timeString());   // CH:MM:SS
Serial.print(' ');
Serial.println(t.dateString()); // DD.
```

<a id="time"></a>
## Time of receipt of the communication
In the processor of incoming messages at the structure`FB_msg`field`unix`It stores the message time in unix format.
To translate into a more readable format, we act according to the scheme described above:
```cpp
void newMsg(FB_msg& msg) {
  FB_Time t(msg.unix, 3);   // transmitted unix and time zone
  Serial.print(t.timeString());
  Serial.print(' ');
  Serial.println(t.dateString());
}
```

<a id="rtc"></a>
## Real-time clocks
In response to any message from the bot, the server reports the time of sending in unix format. On version 2.6, it's time to parse.
the library and ** count continues ** using standard time functions. Thus, it is enough to send once.
A message after the board is turned on so that the library synchronizes the clock. With further shipments, the time will also be synchronized.
and to be specified, because calculated by means of esp time will go away (~2 seconds per day). Tools:

- `uint32_t getUnix()`return the current time in unix format or`0`If the time is not synchronized.
- `bool timeSynced()`- I will.`true`if the clock is in sync.
- `FB_Time getTime(gmt)`- you need to pass your time zone, she'll get it back.`FB_Time`.

There are two ways to get time (see timetest example):
```cpp
FB_Time t = bot.getTime(3);
// or
FB_Time t(bot.getUnix(), 3);
```

<a id="ota"></a>
## Update firmware from chat
With the library version 2.13, there was an update to the firmware "over the air" (OTA) via chat. Update requires:
- Compile the program into a file: *Arduino IDE/Sketch/Export of a binary file* (the file **.bin** will appear in the sketch folder)
- Send file to chat with bot
    - You can add a signature to the file.
	- The file can be transferred from another chat.
- The file will be treated as a normal incoming message from the user.`msg`
    - The signature to the file can be obtained from the field`msg.text`
	- The file name can be obtained from the field.`msg.fileName`
    - The flag will be raised.`msg.OTA`* (if the file has a .bin extension) *
- To start the firmware update process, you need to call`update()`inside
    - Version 2.20 has the ability to update SPIFFS - you need to call`updateFS()`
- Update status will be sent to the same chat (*OK* or *error*)
- After a successful update, esp restarts

### Examples of firmware update scenarios
```cpp
// Update if you just sent a bin file
if (msg.OTA) bot.update();

// Update if the file has the required signature
if (msg.OTA && msg.text == "update") bot.update();

// Update if the file has the right name
if (msg.OTA && msg.fileName == "update.bin") bot.update();

// Update if sent by a well-known person (admin)
if (msg.OTA && msg.chatID == "123456") bot.update();
```

### Examples of SPIFFS update scenarios
```cpp
// Update SPIFFS if a file has the word spiffs in its name
if (msg.OTA && msg.fileName.indexOf("spiffs") > 0) bot.updateFS();
```

### Binary compression
If the firmware weighs a lot, it can be compressed into gzip:
- Recommended compression level - 9
- The file name should end in *.bin.gz*
- The file is also sent to chat with the bot or sent to it.
- In the firmware before connecting all libraries must be announced`#define ATOMIC_FS_UPDATE`

<a id="textmode"></a>
## Drafting of the text
The library supports text in messages. The design is selected by means of`setTextMode(mode)`where`mode`:
- `FB_TEXT`- by default (registration disabled)
- `FB_MARKDOWN`- Markdown v2 markup
- `FB_HTML`- HTML markup

Available tags are described in[API Telegram](https://core.telegram.org/bots/api#formatting-options). For example, for Markdown:
```cpp
bot.setTextMode(FB_MARKDOWN);
bot.sendMessage(F("*Bold*, ~Strike~, `code`, [alexgyver.ru](https://alexgyver.ru/)"));
```

You can chat: **Bold**, ~~Strike~,`code`, [alexgyver.ru](https://alexgyver.ru/)

> **Warning!** In FB MARKDOWN mode, symbols cannot be used in messages.`! + #`The message won't go. It may be possible to fix in the future (the problem of urlencode and shielding reserved characters).

<a id="files"></a>
## Sending files (v2.20+)
You can send files of the following types (type specified when sending), in telegrams these are different types of messages:
- `FB_PHOTO`- picture (jpg, png...)
- `FB_AUDIO`- audio (mp3, wav...)
- `FB_DOC`- document (txt, pdf...)
- `FB_VIDEO`- video (avi, mp4...)
- `FB_GIF`- animation (gif)
- `FB_VOICE`- voice message (ogg)

> All of the above types of posts can be edited except **FB VOICE**!

> When sending, you need to specify the name of the file with the same extension as it was created or stored in memory.

The library supports two options for sending files: from the buffer (RAM) and from SPIFFS.

### Buffer file.
To send, you need to transfer the buffer, its size, file type, its size and chat ID (without specifying the chat ID, a chat from setChatID will be used).
For editing, you also need to specify the message ID:
```cpp
uint8_t sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, const String& id);
uint8_t editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid, const String& id);
```

Send the text in the form of a text file, so you can lead and unload logs:
```cpp
  char buf[] = "Hello, World!";
  bot.sendFile((byte*)buf, strlen(buf), FB_DOC, "test.txt", CHAT_ID);
```

Send a photo from the camera (see example *sendCamPhoto*):
```cpp
  frame = esp_camera_fb_get();
  bot.sendFile((byte*)frame->buf, frame->len, FB_PHOTO, "photo.jpg", CHAT_ID);
```

### File from memory
Instead of the buffer and its size, the sending function accepts the file, the rest is as when sending from the buffer:
```cpp
uint8_t sendFile(File &file, FB_FileType type, const String& name, const String& id);
uint8_t editFile(File &file, FB_FileType type, const String& name, int32_t msgid, const String& id);
```

To work with files in this way, you need to connect the library to which the class determines.`File`For example, SPIFFS.h or LittleFS.h.
> You need to connect the library before (above the code) connecting FastBot! Otherwise, functions with File will not be available.

Let's send a picture from memory.
```cpp
  File file = LittleFS.open("/test.png", "r");
  bot.sendFile(file, FB_PHOTO, "test.png", CHAT_ID);
  file.close();
```

<a id="download"></a>
## Download files (v2.20+)
Since version 2.20, there is a file reference in the incoming message object if there is a file in the message. This allows you to download the file to internal memory.

To download files using FastBot, you need to connect a library that defines the class.`File`For example, SPIFFS.h or LittleFS.h.
> You need to connect the library before (above the code) connecting FastBot! Otherwise, functions with File will not be available.

To download a file, you need to open / create a file with rights to write and transfer it to`downloadFile()`along with a link to the file.
```cpp
void newMsg(FB_msg& msg) {
  if (msg.isFile) {                     // it's a file
    Serial.print("Downloading ");
    Serial.println(msg.fileName);

    String path = '/' + msg.fileName;   // path of view /filename.xxx
    File f = LittleFS.open(path, "w");  // open up
    bool status = bot.downloadFile(f, msg.fileUrl);  // loadable
    Serial.println(status ? "OK" : "Error");    // status
  }
}
```

<a id="location"></a>
## Location
At the specified setting`#define FB_WITH_LOCATION`bot`location`c processed messages (FB msg):

```cpp
struct FB_Location {
  String &latitude;
  String &longitude;
};
```

If the bot is sent a geographical location, the latitude/longitude fields are sent.
filled with coordinates from the bot location:

```cpp
// message-handler
void newMsg(FB_msg& msg) {
  if (msg.location.latitude.length() > 0 && msg.location.longitude.length() > 0) {
    bot.sendMessage("Lat: " + msg.location.latitude + ", Lon: " + msg.location.longitude, msg.chatID);
  }
}
```

See examples`examples/location`and`examples/sunPosition`.

<a id="tricks"></a>
## Tricks.
### Resetting
Messages are marked when read the next (relative to the current message handler) update in tick(), that is, after at least a configured timeout.
If you want to restart esp on command, then this design
```cpp
void message(FB_msg &msg) {
  if (msg.text == "restart") ESP.restart();
}
```It will lead to a bootloop (infinite reboot), because the message will not be marked as read. You can raise the flag on which to go to the reboot, before calling the tickManual:
```cpp
bool res = 0;
void message(FB_msg &msg) {
  if (msg.text == "restart") res = 1;
}
void loop() {
  bot.tick();
  if (res) {
    bot.tickManual(); // To mark the message read
    ESP.restart();
  }
}
```

### omission of “missing” messages based on time
The library has a skipUpdates feature that allows you to skip all unread messages. But sometimes it is convenient to navigate by time.

If you want to ignore messages sent by the user while the bot was offline (or offline), you can do this:
- Remember the unix time when the bot went online
- Compare the time of the current message with it. If it is smaller, ignore the message.

Example of missing messages sent before the controller starts:
```cpp
uint32_t startUnix;     // keep time

void setup() {
  //connectWiFi();

  bot.attach(newMsg);
  bot.sendMessage("start", "1234"); // Send a message to get time
  startUnix = bot.getUnix();        // remember
}

// message-handler
void newMsg(FB_msg& msg) {
  if (msg.unix < startUnix) return; // ignore
  // ....
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - optimization
- v1.2 - you can set several chatIDs and send to the specified chat
- v1.3 - Added the ability to specify text when opening and closing the menu
- v1.3.1 - corrected errors from 1. 3
- v1.4 - Added the ability to delete messages
- v1.5 - optimization, the ability to change the token, new parsing messages (id, name, text)
- v1.5.1 - We also receive the message ID
- v1.6 - Added FB DYNAMIC HTTP mode, read username
- v1.7:
    - Removed dynamic FB DYNAMIC HTTP, running too slowly
    - Fixed warnings
    - Fixed the bot in "groups" (negative chat ID)
    - Memory optimization
    - Accelerated work.
    - Fixed the work again in the script "echo".
  
- v2.0:
    - Removed a minimum of 3200 ms
    - Added processing Unicode (Russian, emoji). Thank you Gleb Zhukov!
    - Extra spaces are removed from the menu, it becomes easier to work
    - Support for esp32
    - Great optimization
    - Added callbacks to inlineMenu
    - Added user ID
    - Added text editing and a bunch of stuff.

- v2.1: 
    - More optimization.
    - Added text formatting (markdown, html)
    - Added a response to the message

- v2.2:
    - Greater optimization of memory and performance
    - Added notify() - notifications from bot messages
    - Added a one-time keyboard display
    
- v2.3. A little optimization
- v2.4: Added url encode for message text
- v2.5: Added flags to FB msg: message edited and message sent by bot. Improving text parsing
- v2.6: Added a built-in real-time clock
- v2.7: Added the posting of stickers.
- v2.8: Removed unnecessary output in the series, GMT can be in minutes
- v2.9: Parsing bug fixed, parsing accelerated, formatted time output added, surname and message time added
- v2.10: Added features to change the name and description of the chat, fixing and detaching messages. Removed edit/deleteMessageID, editMenuID
- v2.11: 
    - Optimization, Bug Correction
    - Callback data is now parsed separately in data
    - Reworked work with callback
    - Added toString() for FB msg for debugging
    - In callback added processing of URLs
    - Removed first name and last name (with Legasi saved)
    - usrID and ID rebranded as userID and messageID (with Legasi remaining)
    - Finally removed the old incoming message handler

- v2.12: examples corrected, isBot parsing fixed, long message protection redesigned, initialization redesigned
- v2.13: Memory optimization. Added an OTA update
- v2.14: Improved line parsing with ID, added OTA disabling, added group/channel parsing to username
- v2.15: Patch for the ESP32 Library Curve
- v2.16: fileName output added, non-sent messages fixed in Markdown mode
- v2.17: output of the text of the message, which the user answered + correct work with the menu in groups
- v2.17.1: small fixhttps://github.com/GyverLibs/FastBot/issues/12
- v2.18: added FB DYNAMIC mode: the library takes longer to complete the request, but takes up 10kb less memory in SRAM
- v2.19: OTA support with gzip compression
- v2.20:
    - Added SPIFFS update + example
    - added the output of the url file for download from the chat + example
    - Added the ability to download the file from the chat
    - added the ability to send files (from SPIFFS or buffer) + example
    - added the ability to edit files (from SPIFFS or buffer)
    - An example of sending photos from the ESP32-CAM camera
- v2.21: Accelerated file sending by bot to chat
- v2.22: Minor optimization, fixes compilation error in FB NO OTA
- v2.23: Fixed a real-time source on editMessage
- v2.24: Fix sending large fileshttps://github.com/GyverLibs/FastBot/pull/17
- v2.25: added skipUpdates - skipping unread messages
- v2.26: fixing incorrect display of numbers after Russian lettershttps://github.com/GyverLibs/FastBot/pull/37

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
