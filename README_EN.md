This is an automatic translation, may be incorrect in some places. See sources and examples!

# FastBot
Multifunctional fast library for telegram bot on esp8266/esp32
- Works on standard libraries
- Optional "whitelist" chat IDs
- Check for updates manually or by timer
- Send/delete/edit/reply messages
- Read and send in chats, groups, channels
- Change the name and description of the chat
- Pin/unpin messages
- Send stickers
- Messages with markdown/html formatting
- Show normal menu
- Display inline menu with support for link buttons
- Unicode support (other languages ​​+ emoji) for incoming messages
- Built-in urlencode for outgoing messages
- Built-in real time clock with synchronization from the Telegram server
- Possibility of OTA firmware update with .bin file from Telegram chat (firmware and SPIFFS)
- Sending files from memory to chat (+ editing)

### Compatibility
ESP8266 (SDK v2.6+), ESP32

### Optional
Use the [CharDisplay] library(https://github.com/GyverLibs/CharDisplay) to display graphs and draw in chat!

![](https://github.com/GyverLibs/CharDisplay/blob/main/docs/plots.png)

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
- The test was carried out in the normal mode of operation of FastBot. When `FB_DYNAMIC` is activated, the library will take up 10kb less memory, but it will work slower:
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
    - [Firmware update from chat](#ota)Cranberry
    - [Text styling](#textmode)
    - [Sending files](#files)
    - [Download files](#download)
    - [All sorts of tricks](#tricks)
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

// send the sticker to the chat/chats specified in setChatID OR pass the id of the chatcranberry ta
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

// ================ FILES ================
// download file from chat
bool downloadFile(File &f, const String& url);
    
// send a file from the buf bytebuffer of length length, type type and file name name to the chat specified in setChatID OR pass the chat id
uint8_t sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name);
uint8_t sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, const String& id);

// send file File, type type and file name name in chat specified in setChatID OR pass chat id
uint8_t sendFile(File &file, FB_FileType type, const String& name);
uint8_t sendFile(File &file, FB_FileType type, const String& name, const String& id);

// edit the file from the buf bytebuffer with length length, type type and file name name in the message msgid in the chat specified in setChatID OR pass the chat id
// except for the FB_VOICE type!
uint8_t editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid);
uint8_t editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid, const String& id);

// edit file File, of type type and file name name in msgid specified in message in setChatID chat OR pass chat id
// except for the FB_VOICE type!
uint8_t editFile(File &file, FB_FileType type, const String& name, int32_t msgid);
uint8_t editFile(File &file, FB_FileType type, const String& name, int32_t msgid, const String& id);

// where FB_FileType - file type
FB_PHOTO - picture
FB_AUDIO - audio
FB_DOC - document
FB_VIDEO - video
FB_GIF - animation
FB_VOICE - voice message

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


// ============== MESSAGE ===============
// FB_msg structure
String&userID; // user ID
String&username; // user's nickname (in API this is first_name)
bool isBot; // user - bot

String&chatID; // chat ID
int32_t messageID; // message ID
bool& edited; // message edited

String&text; // Message text
String&replyText; // response text, if any
bool query; // request
String&data; // callback date

bool isFile; // this is a file
String&fileName; // File name
String&fileUrl; // download file address
bool OTA; // file - request for OTA update

uint32_t unix; // message time


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


// ================ UPDATE =================
uint8_t update(); // OTA firmware update, call inside the message handler on the OTA flag
uint8_t updateFS(); // OTA update SPIFFS, call inside the message handler on the OTA flag

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
// 8 - file error


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
Telegram sets the following limits on **sending** messages by a bot ([documentation](https://core.telegram.org/bots/faq#my-bot-is-hitting-limits-how-do-i-avoid-this ))
- To chat: no more than once per second. *You can send more often, but the message may not reach*
- To a group: no more than 20 messages per minute
- Total limit: no more than 30 messages per second

The bot can also read messages that are less than 24 hours old.

<a id="inbox"></a>
## Message parsing
Messages are automatically requested and read in `tick()`, when a new message arrives, the specified handler function is called:
- Create our own function in the sketch like `void function(FB_msg& message)`
- Call `attach(function)`
- This function will be automatically called on an incoming message if the chat ID matches or is not configured
- If the handler is not connected - messages will not be checked
- Inside this function, you can use the passed variable `message`, which has the type `FB_msg` (structure) and contains:
    - `String userID` - user ID
    - `String username` - user or channel name
    - `bool isBot` - message from bot
    - `String chatID` - chat ID
    - `int32_t messageID` - message ID in the chat
    - `bool edited` - message has been edited
    - `String text` - message text or file caption
    - `String replyText` - reply text, if any
    - `String data` - callback data from the menu (if any)
    - `bool query` - query
    - `bool isFile` is a file
    - `String fileName` - file name
    - `String fileUrl` - file address to download
    - `bool OTA` - request for OTA update (received .bin file)
    - `uint32_t unix` - message time

And also `String toString()` - all information from the message in one line, convenient for debugging (since 2.11)

**Notes:**
- Telegram splits the text into several messages if the text length exceeds ~4000 characters! These messages will have a different messageID in the chat.
- When replying to a message, the library parses the text of the original message, not the answer

### White list
The library implements a white list mechanism: you can specify in `setChatID()` ID of the chat (or several, separated by commas), messages from which will be accepted.
Messages from other chats will be ignored.

<a id="tick"></a>
## Ticker
To poll incoming messages, you need to connect a message handler and call `tick()` in the main loop of the program `loop()`, polling occurs according to the built-in timer.
By default, the polling period is set to 3600 milliseconds.

You can poll more often (change the period via `setPeriod()`), but personally, since ~ 2021, the Telegram server has not responded
sooner than ~3 seconds later. If you request updates more often than this period, the program hangs inside a `tick()` (inside a GET request)
waiting for the server response for the remainder of 3 seconds. With a period of ~3600 ms this does not happen, so I made it the default.
Maybe it depends on the provider or the country.

<a id="example"></a>
## Minimal example
```cpp
void setup() {
  // connect to WiFi
  bot.attach(newMsg); // connect the message handler
}

void newMsg(FB_msg& msg) {
  // display the username and message text
  //Serial.print(msg.username);
  //Serial.print(", ");
  //Serial.println(msg.text);
  
  // output all information about the message
  Serial.println(msg.toString());
}

void loop() {
  bot.tick();
}
```

<a id="msgid"></a>
## Accessing messages
To edit and delete messages and menus, as well as pin messages, you need to know the message ID (its number in the chat):
- ID of the incoming message is sent to the incoming message handler
- Last received message ID can be obtained from `lastUsrMsg()`
- The ID of the last message sent by the bot can be obtained from `lastBotMsg()`

Be careful with the chat ID, all chats have their own message numbering!

<a id="sticker"></a>
## Send stickers
To send a sticker, you need to know the sticker ID. Send the desired sticker to the *@idstickerbot* bot, it will send the sticker ID.
This ID must be passed to the `sendSticker()` function.

<a id="menu"></a>
## Menu
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

You need to answer ** inside the handlerCranberries and messages**! Example:
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
  FB_Time t(msg.unix, 3); // passed unix and time zone
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
- `bool timeSynced()` - will return `true` if the clock is synchronized.
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
- Send file to chat with bot
    - You can add a signature to the file
- File can be forwarded from another chat
- The file will be processed as a normal incoming message from user `msg`
    - File signature can be obtained from the `msg.text` field
- The file name can be obtained from the `msg.fileName` field
    - The `msg.OTA` flag will be raised *(if the file has a .bin extension)*
- To start the firmware update process, you need to call `update()` inside the message handler
    - In version 2.20 it was possible to update SPIFFS - you need to call `updateFS()`
- In the same chat the update status will be sent (*OK* or *error*)
- After a successful update, the esp will reboot

### Firmware update script examples
```cpp
// update if just sent a bin file
if (msg.OTA) bot.update();

// update if the file has the desired signature
if (msg.OTA && msg.text == "update") bot.update();

// update if the file has the correct name
if (msg.OTA && msg.fileName == "update.bin") bot.update();

// update if sent by a famous person (admin)
if (msg.OTA && msg.chatID == "123456") bot.update();
```

### Atmeasures SPIFFS update scripts
```cpp
// update SPIFFS if a file arrived with the word spiffs in its name
if (msg.OTA && msg.fileName.indexOf("spiffs") > 0) bot.updateFS();
```

### Binary compression
If the firmware weighs a lot, it can be compressed into gzip:
- Recommended compression level - 9
- The file name must end with *.bin.gz*
- The file is sent to the chat with the bot in the same way or sent to him
- In the firmware, before connecting all libraries, you need to declare `#define ATOMIC_FS_UPDATE`

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

> **Attention!** In FB_MARKDOWN mode, you cannot use `! + #`, the message will not be sent. It may be possible to fix it in the future (the problem of urlencode and escaping of reserved characters).

<a id="files"></a>
## Sending files (v2.20+)
You can send files of the following types (the type is specified when sending), in telegrams these are different types of messages:
- `FB_PHOTO` - picture (jpg, png...)
- `FB_AUDIO` - audio (mp3, wav...)
- `FB_DOC` - document (txt, pdf...)
- `FB_VIDEO` - video (avi, mp4...)
- `FB_GIF` - animation (gif)
- `FB_VOICE` - voice message (ogg)

> You can edit all the above types of messages except **FB_VOICE**!

> When sending, you must specify the name of the file with the same extension as it was created or stored in memory.

The library supports two options for sending files: from the buffer (RAM) and from SPIFFS.

### File from buffer
To send, you need to pass the buffer, its size, file type, its size and the chat ID (without specifying the chat ID, the chat from setChatID will be used).
To edit, you must also specify the message ID:
```cpp
uint8_t sendFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, const String& id);
uint8_t editFile(uint8_t* buf, uint32_t length, FB_FileType type, const String& name, int32_t msgid, const String& id);
```

Let's send the text as a text file, so you can maintain and upload logs:
```cpp
  charbuf[] = "Hello, World!";
  bot.sendFile((byte*)buf, strlen(buf), FB_DOC, "test.txt", CHAT_ID);
```

Let's send a photo from the camera (see example *sendCamPhoto*):
```cpp
  frame = esp_camera_fb_get();
  bot.sendFile((byte*)frame->buf, frame->len, FB_PHOTO, "photo.jpg", CHAT_ID);
```

### Memory file
Instead of a buffer and its size, the send function takes a file, the rest is the same as when sending from a buffer:
```cpp
uint8_t sendFile(File &file, FB_FileType type, const String& name, const String& id);
uint8_t editFile(File &file, FB_FileType type, const String& name, int32_t msgid, const String& id);
```

To work with files in this way, you need to include a library that defines the `File` class, such as SPIFFS.h or LittleFS.h.
> You need to connect the library BEFORE (higher in the code) connecting FastBot! Otherwise, functions with File will not be available.

Let's send a picture from memory:
```cpp
  File file = LittleFS.open("/test.png", "r");
  bot.sendFile(file, FB_PHOTO, "test.png", CHAT_ID);
  file.close();
```

<a id="download"></a>
## Download files (v2.20+)
Since version 2.20, the incoming message object contains a link to a file if the message contains a file. This allows you to download the file to the internal memory.

To download files using FastBot, you need to include a library that defines the `File` class, such as SPIFFS.h or LittleFS.h.
> You need to connect the library BEFORE (higher in the code) connecting FastBot! Otherwise, functions with File will not be available.

To download a fileneeds to open/create a file with writable permissions and pass it to `downloadFile()` along with a link to the file.
```cpp
void newMsg(FB_msg& msg) {
  if (msg.isFile) { // this is a file
    Serial.print("Downloading");
    Serial.println(msg.fileName);

    String path = '/' + msg.fileName; // path like /filename.xxx
    File f = LittleFS.open(path, "w"); // open for writing
    bool status = bot.downloadFile(f, msg.fileUrl); // load
    Serial.println(status ? "OK" : "Error"); // status
  }
}
```

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
- v2.13: Memory optimization. Added OTA update
- v2.14: Improved ID string parsing, added OTA disable, added group/channel name parsing to username
- v2.15: ESP32 library curve patch
- v2.16: addedoutput fileName, fixed unsent messages in Markdown mode
- v2.17: displaying the text of the message to which the user replied + correct work with menu in groups
- v2.17.1: minor fix https://github.com/GyverLibs/FastBot/issues/12
- v2.18: added FB_DYNAMIC mode: the library takes longer to execute the request, but takes up 10 kb less memory in SRAM
- v2.19: OTA support with gzip compression
- v2.20:
    - added OTA update SPIFFS + example
    - added the output of the url of the file for downloading from the chat + example
    - added the ability to download a file from the chat
    - added the ability to send files (from SPIFFS or buffer) + example
    - added the ability to edit files (from SPIFFS or buffer)
    - added an example of sending photos from the ESP32-CAM camera

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!

When reporting bugs or incorrect work of the library, it is necessary to indicate:
- Library version
- What MK is used
- SDK version (for ESP)
- Arduino IDE Version
- Whether built-in examples work correctly, which use functions and constructs that lead to a bug in your code
- What code was loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimal code