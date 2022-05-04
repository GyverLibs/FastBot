This is an automatic translation, may be incorrect in some places. See sources and examples!

# FastBot
Very simple and fast library for telegram bot on esp8266/esp32
- Works on standard libraries
- Works without SSL
- Optimized for heavy load
- Optional setting of the chat ID to communicate with the bot
- Check for updates manually or by timer
- Sending/Editing/Replying to messages
- Show menu instead of keyboard
- Display inline menu in message
- Unicode support (other languages ​​+ emoji) for incoming messages
- Built-in urlencode for outgoing messages
- Built-in real time clock with synchronization from Telegram

### Compatibility
ESP8266,ESP32

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
FastBot bot(token);
FastBot bot(token, limit);
FastBot bot(token, limit, threshold);
FastBot bot(token, limit, threshold, period);
// token - unique bot code, taken from BotFather
// limit - the number of messages received from one request (default 10)
// threshold - the number of characters at which the API request will be considered too large and will be skipped (default 10000)
// period -bot automatic polling period in ms (default 3500)
```

<a id="docs"></a>
## Documentation
```cpp
// ============== SETTINGS ==============
void setToken(String token); // change/set bot token
void setChatID(String chatID); // set the chat ID. Multiple separated by commas ("id1,id2,id3")
void setPeriod(int period); // polling period in ms (default 3500)
void setLimit(int limit); // max number of messages per request
void setOvf(int ovf); // max number of characters per request (flood protection)
void setBufferSizes(uint16_t rx, uint16_t tx); // set buffer sizes for receiving and sending, by default. 512 and 512 bytes (only for esp8266)
   

// =============== PARSING ===============
void attach(void (*FB_msg)(msg&)); // connection of the message handler
void attach(void (*handler)(String&, String&)); // connect message handler (deprecated)
void detach(); // disable ALL MESSAGE HANDLERS


// ================ TICKER =================
uint8_t tickManual(); // manual check for updates
uint8_t tick(); // check for updates by timer


// ============== MESSAGES ===============
// send a message to the chat/chats specified in setChatID OR pass the chat id
uint8_t sendMessage(String msg);
uint8_t sendMessage(String msg, String id);

// reply to a message with id (replyID) in the chat specified in setChatID OR specify a chat
uint8_t replyMessage(String msg, int32_t replyID);
uint8_t replyMessage(String msg, int32_t replyID, String id);


// send the sticker to the chat/chats specified in setChatID OR pass the chat id
uint8_t sendSticker(String stickerID);
uint8_t sendSticker(String stickerID, String id);

// ============== MESSAGES ===============
// respond to callback with text (text) and mode (alert) (true - warning)
uint8_t answer(String text, bool alert = false);

void setTextMode(uint8_t mode); // text processing mode: FB_TEXT, FB_MARKDOWN, FB_HTML
void notify(bool mode); // true/false enable/disable notifications from bot messages (on by default)

int32_t lastBotMsg(); // ID of the last message sent by the bot
int32_t lastUsrMsg(); // ID of the last message sent by the user


// ============== DELETE ===============
// delete a message with an offset from the last (offset) in the chat specified in setChatID OR pass the chat id
uint8_t deleteMessage(int32_t offset);
uint8_t deleteMessage(int32_t offset, String id);

// delete the message with id (msgid) in the chat specified in setChatID OR pass the chat id
uint8_t deleteMessageID(int32_t msgid);
uint8_t deleteMessageID(int32_t msgid, String id);


// ============ EDIT ==============
// edit the message with an offset from the last one (offset) in the chat specified in setChatID
uint8_t editMessage(int32_t offset, String text);

// edit the message (msgid) in the chat specified in setChatID OR pass the chat id
uint8_t editMessageID(int32_t msgid, String text);
uint8_t editMessageID(int32_t msgid, String text, String id);


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

//one-time menu (will close when selected)
uint8_t showMenuText(String msg, String menu, true);
uint8_t showMenuText(String msg, String menu, String id, true);

// message (msg) + hide the menu in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t closeMenuText(String msg);
uint8_t closeMenuText(String msg, String id);


// ============= INLINE MENU =============
// message (msg) with an inline menu (menu) in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t inlineMenu(String msg, String menu);
uint8_t inlineMenu(String msg, String menu, String id);


// ======= INLINE MENU WITH A CALLBACK =======
// message (msg) with inline menu (menu) and callback (cbck) in the chat/chats specified in setChatID OR pass the id of the chat/chats
uint8_t inlineMenuCallback(String msg, String menu, String cbck);
uint8_t inlineMenuCallback(String msg, String menu, String cbck, String id);


// ======= EDIT INLINE MENU WITH CALLBACK =======
// edit the menu (msgid) with text (menu) in the chat specified in setChatID OR pass the chat id
uint8_t editMenuID(int32_t msgid, String menu, String cback);
uint8_t editMenuID(int32_t msgid, String menu, String cback, String id);


// ================ SERVICE ===============
uint8_t sendRequest(String&req); // send request (https://api.telegram.org/bot...)
void autoIncrement(boolean incr); // auto increment messages (enabled by default)
void incrementID(uint8_tval); // manually increment ID by val
String chatIDs; // the string specified in setChatID, for debugging and editing the list

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

// =============== STATUS ================
// Many functions return status:
// 0 - waiting
// 1 - OK
// 2 - ovf full
// 3 - Telegram error
// 4 - Connection error
// 5 - chat ID not set
// 6 - multiple send, status unknown


// ========== DEFINE SETTINGS ==========
// declare BEFORE linking the library
#define FB_NO_UNICODE // disable Unicode conversion for incoming messages (slightly speed up the program)
#define FB_NO_URLENCODE // disable urlencode conversion for outgoing messages (slightly speeds up the program)
```

<a id="usage"></a>
## Usage

### Ticker
To poll incoming messages, you need to call `tick()` in the main loop of the program `loop()`, polling occurs according to the built-in timer.
By default, the polling period is set to 3500 milliseconds. You can poll more often (change via `setPeriod()`), but Telegram is sometimes dumb and
with frequent polling, the query can run ~3 seconds instead of 60 milliseconds! During this time, the program "hangs" inside `tick()`.
With a period of 3500 ms this does not happen, so I made it the default.

### ID of the chat(s)
Set via `setChatID()`, where you can pass a single ID or several at once, separated by commas: `setChatID("id1,id2,id3")`

### Parsing messages
Messages are automatically read in `tick()`, when a new message arrives, the specified handler function is called. But there are options here:
- If a chat/chats ID is set (via `setChatID()`) - messages NOT from the specified chats are automatically filtered out
- If the chat/chats ID is not set (via `setChatID()`) - the message will be processed, i.e. handler called

The handler is attached using `attach(FB_msg&)` or `attach(String&, String&)` (deprecated)
- Create our own function in the sketch like `void function(FB_msg& message)`
- Call `attach(funshare)`
- This function will be automatically called on an incoming message if the chat IDs match or are not configured
- Inside this function, you can use the passed variable `message`, which has the type `FB_msg` (structure) and contains:
    - `int32_t ID` - message ID
    - `String usrID` - user ID
    - `String first_name` - username
    - `String username` - user's nickname
    - `String chatID` - chat ID
    - `String text` - message text
    - `bool query` - query flag
    - `bool edited` - message has been edited
    - `bool isBot` - message from bot

### Minimal example
```cpp
void setup() {
  // connect to WiFi
  bot.attach(newMsg); // connect the message handler
}

void newMsg(FB_msg& msg) {
  // display the username and message text
  Serial.print(msg.username);
  Serial.print(", ");
  Serial.println(msg.text);
}

void loop() {
  bot.tick();
}
```

### Send stickers
To send a sticker, you need to know the sticker ID. Send the desired sticker to the *@idstickerbot* bot, it will send the sticker ID.
This ID will need to be passed to the `sendSticker()` function.

### Menu formatting
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

### Regular menu
Large menu at the bottom of the chat.
```cpp
showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Pressing the button **sends the text from the button**.
> Note: for all menu options *not produced* url encode. Avoid `" % & + \` characters!

### Inline menu
Menu in the message. Requires a menu name:
```cpp
inlineMenu("MyMenu", "Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Pressing the button **sends the text from the button**.

### Inline menu with callback
Menu in the message. The callback allows you to set a unique text for each button, which will be sent to the bot instead of the text from the button.
The list of callbacks is listed comma-separated in order:
```cpp
String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
String cback1 = F("action1,action2,action3,back");
bot.inlineMenuCallback("Menu 1", menu1, cback1);
```
Pressing the button **sends the given text**.

### Response to callback
When you click on the inline menu button, a callback is sent to the bot, the `query` flag will be raised in the message handler.
You can respond to the callback with:
- `answer(text)` - popup text
- `answer(text, true)` - window with warning and OK button

Example:
```cpp
void newMsg(FB_msg& msg) {
  if (msg.query) bot.answer("Hello!", true);
}
```

### Real time clock
In response to any message from the bot, the server reports the time of sending in unix format. Since version 2.6 this time is parsed
library and **count continues on** using standard time functions.
Thus, it is enough to send a message once after turning on the board for the library to synchronize the clock. At
further shipments, the time will also be synchronized and updated, because. the time calculated by means of esp will go away.

You can get the current time in unix format using the `getUnix()` function. Returns 0 if the time is out of sync. There is also a function
`timeSynced()` will return `true` if the clock is synchronized.

To get the time in a more convenient format, there is a `getTime(gmt)` function, you need to pass your time zone to it:
- In hours, for example 3 hours for Moscow (UTC+3:00)
- In minutes, e.g. 330 minutes for India (UTC+5:30)

Function
returns the data type `FB_Time`, which is a structure with fields:

```cpp
uint8_t second; // seconds
uint8_t minute; // minutes
uint8_t hour; // clock
uint8_t day; // day of the month
uint8_tmonth; // month
uint8_tdayWeek; // day of the week (Mon..Sun 1..7)
uint16_t year; // year
```

You can use it like this (see the timeTest example):
```cpp
FB_Time t = bot.getTime(3);
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

<a id="example"></a>
## Example
```cpp

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

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!