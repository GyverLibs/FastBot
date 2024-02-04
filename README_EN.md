This is an automatic translation, may be incorrect in some places. See sources and examples!

# Fastbot
Multifunctional fast library for telegram bot on ESP8266/ESP32
- works in standard libraries
- Optional "White List" ID Chats
- checking updates manually or by timer
- Sending/Removing/editing/response to messages
- Reading and sending in chats, groups, channels
- Changing the name and description of the chat
- consolidation/reinforcement of messages
- Sending stickers
- Markdown/html formatting messages
- Conclusion of a conventional menu
- Conclusion Inline menu with support for buttons-lits
- Support Unicode (other languages + emoji) for incoming messages
- Built -in Urlencode for outgoing messages
- built -in hours of real time with synchronization from the Telegram server
- The possibility of OTA updating the firmware .bin with a Telegram chat file (Firmware and Spiffs)
- sending files from memory to chat (+ editing)

## compatibility
ESP8266 (SDK V2.6+), ESP32

## Creating and Setting a bot
- [Instruction How to create and configure Telegram bot] (https://kit.alexgyver.ru/tutorials/telegram-basic/)
- If you already have a bot, make sure that it * is not in Webhook mode * (disabled by default), otherwise ESP will not be able to accept messages!
- In order for the bot to read all the messages in the group (and not just `/commands`), you need to turn off the *group privacy *parameter in the Bota *Bot Settings *settings in chat C *@byfather *.This parameter is turned on by default!
- For full work in the group (supergroup), the bot must be done by the administrator!

## Restrictions
### TEELGRAM limits
Telegram sets the following limits on ** sending ** Bot Messages ([documentation] (https://core.telegram.org/faq#my-bot-is-limits-hem-do-i- void-this))
- In chat: no more than once per second.*You can send more often, but the message may not reach*
- to group: no more than 20 messages per minute
- total limit: no more than 30 messages per second
- The bot can read messages, from the moment of which less than 24 hours have passed
- The bot cannot write in a personal bottle
-Bot [does not see] (https://core.telegram.org/bots/faq#why-doesn-39t-by-see-mee-messages-from-bots) messages from other bots in the group

### Other
- Telegram divides the text into several messages if the length of the text exceeds ~ 4000 characters!These messages will have different Messageid in the chat
- When responding to the message, the library parses the text of the source message, not an answer

## Graphics output
Use the library [Chardisplay] (https://github.com/gyverlibs/chardisplay) to display graphs and drawing in the chat!

! [] (https://github.com/gyverlibs/chardisplay/blob/main/docs/plots.png)

## Documentation and Projects
Detailed lessons for working with telegram boot using this library can be found on [Arduino set of gyverkit] (https://kit.alexgyver.ru/tutorials-category/telegram/)

## Comparison with Universal-Arduino-Telegram-Bot
[Universal-arduino- Telegram-bot] (https://github.com/witnessMenow/universal-rduino- Telegram-bot)

For comparison, a minimum example was used with sending a message to the chat and withdrawing incoming messages to the series:
- ** SEND ** - Sending the message to the chat
- ** update ** - checking incoming messages
- ** free heap ** - the volume of free RAM during the program

|Library |Flash, b |Sram, b|SEND, MS |update, ms |free heap, b |
| ----------- | ---------- | --------- | ----------- | ---------------- | ------------- |
|Univ..bot |400004 |29848 |2000 |1900 |38592 |
|Fastbot |393220 |28036 |70 |70 |37552 |
|Diff |6784 |1812 |1930 |1830 |1040 |

- Fastbot is almost 7 KB Flash and 2 KB SRAM, but occupies 1 KB in SRAM more during the program.In total, 2-1 = 1 KB SRAM is more easily.
- Fastbot processes the chat much faster and sends messages (for 2 seconds) due to manual parsing of the server response and statically allocated http customers
- The test was carried out in the usual operating mode Fastbot.When activating `fb_dynamic`, the library will occupy 10 kb less memory, but it will work more slowly:
  - Free Heap: 48000 Kb
  - Sending message: 1 second
  - Refresh Request: 1 second

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
    - [Location](#location)
    - [All sorts of tricks](#tricks)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>

## Installation
- The library can be found by the name ** farbot ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/fastbot/archive/refs/heads/main.zip). Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id="init"> </a>
## initialization
`` `CPP
Fastbot Bot;
FASTBOT BOT (token);// TOKEN
`` `

<a id="docs"> </a>
## Documentation
`` `CPP
// ============== MAYS =======================
VOID SetToken (String Token);// Change/Set the token bot
VOID SetChatid (String Chatid);// Installation of ID chat (white list), optional.Can be somewhat after a comma ("ID1, ID2, ID3")
VOID SetChatid (Int64_T ID);// The same thing, but in int64_t.Send 0 to turn off
VOID setperiod (intration);// Polling period in the MS (in silence 3500)
VOID SetLimit (int limit);// Summer messages that are processed for one request, 1..100.(in silence 10)
VOID SetBuffersizes (Uint16_T RX, UINT16_T TX);// Set the sizes of the buffer for reception and sending, by the silence.512 and 512 bytes (only for ESP8266)
VOID Skipupdates ();// Skip unread messagesCranberry
    
VOID settextmode (Uint8_t Mode);// Text mode "For Sending": fb_text, fb_markdown, fb_html (see the example of Textmode)
VOID Notify (Bool Mode);// True/False ON/Off of notifications from the posts of the bot (by the silence)
VOID CLEARSERVICEMESSAGES (BOOL STATE);// Delete from the chat service messages about changing the name and consolidation of messages (silence. FALSE)


// ================ Parsing =======================================
Void attach (callback);// Connection of the Message Processing Function
VOID Detach ();// Disconnecting the messenger of messages


// ====================================================================
uint8_t tick ();// checking updates by timer
uint8_t Tickmanual ();// manual verification of updates


// ============== Message =======================
// Send a message to the chat/chat specified in setchatid or transmit chat ID
Uint8_T SendMessage (String MSG);
Uint8_T SendMessage (String MSG, String ID);

// Edit a message (msgid) in the SetChatid chat or transmit chat ID
uint8_t editmessage (Int32_t Msgid, String Text);
uint8_t editmessage (Int32_t Msgid, String Text, String ID);

// Reply to the message with ID (replyid) to the chat specified in Setchatid or specify the chat
Uint8_T ReplyMessage (String MSG, Int32_T Replyid);
Uint8_T ReplyMessage (String MSG, Int32_T Replyid, String ID);

// Send a sticker to the chat/chat specified in SetChatid or transfer ID chat
Uint8_T SendSticker (String Stickerid);
Uint8_T SendSticker (String Stickerid, String ID);

// Reply to Callback text (Text) and mode (alert): fb_notif - chat notice, fb_alert - window with button
Uint8_T ANSWER (String Text, Bool Alert);


// ===========ward
// delete a message from ID (msgid) in the chat specified in SetChatid or transmit chat ID
// deleting any types of messages (text, sticker, inline menu)
Uint8_t DeleteMessage (Int32_T MSGID);
Uint8_T DeleteMessage (Int32_T MSGID, String ID);


// ============= Ordinary menu =====================
// Show the menu (menu) in the chat/chats indicated in SetChatid or transfer ID chat/chats
uint8_t showmenu (string menu);
uint8_t showmenu (String Menu, String ID);

// United menu (closed when choosing) in the current chat or transfer ID chat
uint8_t showmenu (string menu, true);
uint8_t showmenu (string menu, string id, true);

// Hide the menu in the chat/chats indicated in setchatid or transmit chat/chat ID
uint8_t closemenu ();
Uint8_t Closemenu (String ID);


// ======== Ordinary menu with text ==========
// Message (MSG) + Show the menu (Menu) in the chat/chats indicated in SetChatid or transfer ID chat/chats
uint8_t showmenutext (String MSG, String Menu);
uint8_t showmenutext (String MSG, String Menu, String ID);

// United Menu (closes when choosing)
uint8_t showmenutext (String MSG, String Menu, True);
uint8_t showmenutext (String MSG, String Menu, String ID, True);

// Message (MSG) + Hide the menu in the chats/chats indicated in SetChatid or transfer ID chat/chats
Uint8_T Closemenutext (String MSG);
Uint8_t Closemenutext (String MSG, String ID);


// ============= Inline menu ==================
// Message (MSG) with Inline Menu (Menu) in SetChatid Chat/Chats or transfer ID chat/chats
Uint8_T Inlinemenu (String MSG, String Menu);
Uint8_T Inlinemenu (String MSG, String Menu, String ID);

// edit the menu (msgid) with text (Menu) in the SetChatid chat or transfer ID chat
uint8_t editMenu (int32_t msgid, string menu);
Uint8_T EditMenu (Int32_T MSGID, String Menu, String ID);


// ======= Inline menu with collbe =======
// Message (MSG) with Inline Menu (Menu) and CBCK (CBCK) in the chats/chats indicated in SetChatid or transfer ID chat/chats
Uint8_T Inlinemenucallback (String MSG, String Menu, String CBCK);
Uint8_T Inlinemenucallback (String MSG, String Menu, String CBCK, String ID);

// edit the menu (msgid) with text (Menu) and CBACK in SetChatid Chat or transfer ID chat
uint8_t edItmenucallback (Int32_T MSGID, String Menu, String CBACK);
uint8_t editMenucallback (Int32_T MSGID, String Menu, String CBACK, String ID);


// ============== Group =======================
// For all group commands, the bot should be an admin in the chat!

// set the name of the group in the chat specified in setchatid or transmit chat ID
uint8_t setChattitle (String & Title);
Uint8_T SetChattitle (String & Title, String & ID);

// Install the description of the group in the chat specified in SetChatid or transmit chat ID
Uint8_T SetChatDescription (String & Description);
Uint8_T SetChatDescription (String & Description, String & ID);

// fix the message from ID MSGID in SetChatid Chat or transmit chat ID
uint8_t Pinmessage (Int32_T MSGID);
Uint8_t Pinmessage (Int32_T MSGID, String & ID);

// Up the message from ID MSGID in the chat specified in SetChatid or transmit chat ID
uint8_t unpinMessage (int32_t msgid);
uint8_t unpinMessage (Int32_t Msgid, String & ID);

// Upstake all messages in the SetChatid chat or transfer chat ID
uint8_t unpinall ();
Uint8_T Unpinall (String & ID);

// ====ward
// Download Chat file
Bool DownloadFile (File & F, Const String & URL);
    
// Send a file from the Buf byt Bufer Length, such as Type and the name of the NAME file in the SetChatid chat or transfer ID chat
Uint8_t Sendfile (Uint8_t* Buf, Uint32_t Length, FB_FileType Type, Constation String & Name);
Uint8_t Sendfile (Uint8_t* Buf, Uint32_t Length, FB_FileType Type, Constation String & Name, Constance String & ID);

// Send a file file, such as Type and the name of the NAME file in the SetChatid chat or transfer ID chat
Uint8_t Sendfile (File & File, FB_FileTYPE TYPE, COST String & NAME);
Uint8_t Sendfile (File & File, FB_FileTYPE TYPE, COST String & NAME, COST String & ID);

// Edit a BUF BuF Buf Bufer file, such as Type and NAME file in the MSGID message in the SetChatid chat or transfer ID chat
// with the exception of the type fb_voice!
uint8_t editfile (uint8_t* buf, uint32_t Length, fb_fileetype type, const String & Name, Int32_T MSGID);
Uint8_t Editfile (Uint8_t* Buf, Uint32_T Length, FB_FileType Type, Constation String & Name, Int32_T MSGID, COST String & ID);

// Edit File file, such as Type and the name of the NAME file in the MSGID in SetChatid Chat or transfer ID chat.
// with the exception of the type fb_voice!
uint8_t editfile (File & File, FB_FileTYPE TYPE, COST String & Name, Int32_T MSGID);
uint8_t editfile (File & File, FB_FileTYPE TYPE, COST String & NAME, Int32_T MSGID, COST String & ID);

// where fb_fileetype - file type
FB_PHOTO - picture
FB_Audio - Audio
Fb_doc - document
Fb_video - video
FB_GIF - animation
Fb_voice - vocal message

// ============= VILLED API ===========
// Send the API command in SetChatid Chat or transfer ID chat (ID itself will be added to the command)
// (example of the team: "/SendSticker? Sticker = 123456")
Uint8_T Sendcommand (String & CMD);
Uint8_T Sendcommand (String & CMD, String & ID);


// ================ Service =========================================
Int32_T Lastbotmsg ();// ID of the last message sent by the bot
int32_t Lastusrmsg ();// ID of the last message sent by user
String Chatids;// Setchatid line for debugging and editing the list

Uint8_t Sendrequest (String & Req);// Send a request (https: //api.telegram.org/bot ...)
VOID autoincrement (Boolean Incr);// Auto Increase of Messages (on the silence turned on)
VOID Incrementid (Uint8_t Val);// manually increase ID on Val


// ============== Message ========================
// Structure FB_MSG
String & userid;// ID Yuzer
String & username;// Nick Yuzer (in the API this is first_name)
Bool ISBOT;// user - bot

String & Chatid;// ID Chat
Int32_T Messageid;// ID messages
Bool & Edited;// Message is edited

String & Text;// Message text
String & ReplyText;// The text of the answer if it is
Bool query;// request
String & Data;// Callback Date

Bool ISFILE;// This is a file
String & Filename;// file name
String & Fileurl;// file address address
Bool OTA;// file - request for OTA update

uint32_t unix;// Message time


// ====================================================================
Fb_time gettime (int16_t GMT);// get the current time, indicate the time zone (for example, Moscow 3) in hours or minutes
Bool TimeSynced ();// Check, whether time is synchronized
uint32_t getunix ();// Get current unix time

// structure fb_time
uint8_t second;// seconds
uint8_t minute;// minutes
Uint8_t Hour;// watch
uint8_t day;// Day of the month
uint8_t months;// month
uint8_t dayweek;// Day of the week (Mon.. SS 1..7)
uint16_t year;// year
String Timestring ();// line of time of the format of hhh: mm: ss
String Datestring ();// line of dates of the format DD.MM.YYYY


// ================ Main ========================ward
uint8_t update ();// Ota update of the firmware, call a message from the OTA flag inside the processor
uint8_t updatefs ();// Ota update SPIFFS, call a message from the OTA flag inside the processor

// =========================================
// Many functions return the status:
// 0 - waiting
// 1 - ok
// 2 - crowded
// 3 - Telegram error
// 4 - connection error
// 5 - not set Chat ID
// 6 - multiple shipping, status is unknown
// 7 - the processor is not connected
// 8 - file error


// =============== Vitus =======================================
VOID FB_UNICODE (String & S);// Translate Unicode
VOID fb_urlencode (String & S, String & Dest);// Urlencode from S to Dest

Int64_T FB_STR64 (COST String & S);// Translating from String to Int64_T
String fb_64str (int64_t ID);// Translation from int64_t to string


// ========== DEFINE SETTINGS ===========
// declare BEFORE linking the library
#define FB_NO_UNICODE // disable Unicode conversion for incoming messages (slightly speed up the program)
#define FB_NO_URLENCODE // disable urlencode conversion for outgoing messages (slightly speeds up the program)
#define FB_NO_OTA // disable support for OTA updates from chat
#define FB_DYNAMIC // enable dynamic mode: the library takes longer to execute the request, but takes up 10 kb less memory in SRAM
#define FB_WITH_LOCATION // enable location fields in message (FB_msg)
```

<a id="usage"> </a>
## Usage
<a id="send"> </a>
## Sending Posts
For sending to the chat (messages, stickers, menu, and so on), the chat ID must be indicated to which the shipment will be carried out.You can specify
A few IDs through a comma, within one line.There are two ways to specify ID:
- directly to the sending function, they all have such an option (see documentation above)
`` `CPP
BOT.SENDMESSAGE ("Hello!", "123456");// in one chat
BOT.SENDMESSAGE ("Hello!", "123456,7891011");// in two chats
`` `
- install the ID through `setChatid ()` and all departments will go to these chat/chats if another ID is not indicated in the sending function
`` `CPP
BOT.SETChatid ("123456");// One chat
//bot.setchatid("123456.7891011 ");// Several chats
// ...
BOT.SENDMESSAGE ("HELLO!");// will go to "123456"
BOT.SENDMESSAGE ("Hello!", "112233");// will go to "112233"
`` `
> Note: Telegram divides the text into several messages if the length of the text exceeds ~ 4000 characters!These messages will have different Messageid in the chat.

<a id="inbox"> </a>
## Parsing Posts
Messages are automatically requested and read in `tick ()`, upon receipt of a new message, the specified processor function is called:
- Create in the sketch our function of the type `void function (fb_msg & message)`
- call `Attach (function)`
- this function will be automatically caused with an incoming message if the chat ID coincides or is not configured
- if the processor is not connected - the messages will not be checked
- Inside this function, you can use the conveyed variable `message`, which has the type` fb_msg` (structure) and contains:
    - `String Userid` - User ID
    - `String Username` - Name of Usatla or channel
    - `bool isbot` - Message from the bot
    - `String Chatid` - ID Chat
    - `int32_t Messageid` - ID messages in the chat
    - `Bool edited` - the message is edited
    - `String Text` - Text of a message or a package to the file
    - `string replytext` - the text of the answer if it is
    - `string data` - callback data from the menu (if any)
    - `bool query` - request
    - `Bool ISFILE` - this is a file
    - `String Filename` - File name
    - `String Fileurn` - file address for download
    - `Bool OTA` - request for OTA update (received .bin file)
    - `uint32_t unix` - Message time

As well as `String Tostring ()` - all the information from the message by one line is convenient for debugging (from version 2.11)

### White list
The library has a white list mechanism: you can specify in `setChatid ()` chat (or several after aim), the messages from which will be accepted.
Messages from other chats will be ignored.

<a id="tick"> </a>
## ticker
To interview incoming messages, you need to connect the messenger of messages and call `tick ()` in the main cycle of the `loop ()` program, the survey takes place according to the built -in timer.
By default, the survey period set 3600 milliseconds.

Can be interviewed more often (change the period through `setperiod ()`), but I personally have a telegram server, I did not answer.
earlier than in ~ 3 seconds.If you request updates more often than this period, the program freezes inside `tick ()` (inside Get request)
In anticipation of the server response to the remainder of 3 seconds.In a period of ~ 3600 ms this does not happen, so I did it by default.
Perhaps it depends on the provider or country.

<a id="EXAMPLE"> </a>
## The minimum example
`` `CPP
VOID setup () {
  // We connect to wifi
  BOT.ATTACH (NewMSG);// Connect the message processor
}

VOID NewMSG (FB_MSG & MSG) {
  // display the name of the user and the text of the message
  //Serial.Print(MSG.USERNAME);
  //Serial.print (",");
  //Serial.println(MSG.TEXT);
  
  // We display all the information about the message
  Serial.println (msg.tostring ());
}

VOID loop () {
  BOT.Tick ();
}
`` `

<a id="msgid"> </a>
##
To edit and delete messages and menu, as well as fixing messages, you need to know the ID messages (its number in the chat):
- ID of the incoming message comes to the processor of incoming messages
- ID of the last adopted message can be obtained from `Lastusrmsg ()`
- ID of the last message sent by the bot can be obtained from `Lastbotmsg ()` `

Be careful with the ID chat, all chats have their own numbering of messages!

<a Id="SSSTICER"> </A>
## Sending stickers
To send a sticker, you need to know the sticker ID.Send the desired sticker to the bot *@idStickerbot *, he will send an ID sticker.
This ID must be conveyed to the function `SENSTICKER ()`.

<a id="menu"> </a>
## menu
> Note: for all menu options * not produced * url encode.Avoid the symbols `#` `&` Or use the already encoded URL!

To send the menu, a string with buttons and special formatting is used:
- `\ t` - horizontal separation of buttons
- `\ n` - vertical separation of buttons
- extra gaps are cut out automatically

Example menu 3x1: `" menu1 \ t menu2 \ t menu3 \ n menu4 "`

Result:
`` `CPP
 _______________________
||||
|Menu1 |Menu2 |Menu3 |
| _______ | _______ | _______ |
||
|M e n u 4 |
| _______________________ |
`` `

<a id="basic"> </a>
## Conventional menu
Large menu in the lower part of the chat.
`` `CPP
ShowMenu ("menu1 \ t menu2 \ t menu3 \ n menu4");
`` `
Pressing the button sends the text from the button (the message field `Text`).

<a id="inline"> </a>
## Inline menu
The menu in the message.Requires an input name menu.
`` `CPP
Inlinemenu ("Mymenu", "Menu1 \ T Menu2 \ T Menu3 \ N Menu4");
`` `
Pressing the button sends the menu name (message `Text`) and text from the button of the message` Data`).

<a id="callb"> </a>
## Inline menu with collker
The menu in the message.Allows you to set each button to a unique text that will be sent by the bot with the name of the menu.
The list of collves is listed through the decimalYu in the order of the menu buttons:
`` `CPP
String menu1 = f ("menu 1 \ t menu 2 \ t menu 3 \ n back");
String CBACK1 = F ("Action1, Action2, Action3, Back");
Bot.inlineMenucallback ("Menu 1", Menu1, CBACK1);
`` `
Pressing the button sends the menu name (message `Text`) and the specified data (message` data`).
- (from version 2.11) If Callback is specified as http/https address, the button automatically becomes ** a clip button **

<a id="answer"> </a>
## response to collback
When you press the Inline Botu button, the collob is sent, the `query` flag will be raised in the message processor.The telegram server will wait for an answer.
You can answer the collob with the help of:
- `ANSWER (text, fb_notif)` - pop -up text -notification
- `ANSWER (text, fb_ALERT)` - Warning window and ok button

You need to answer ** inside the message processor **!Example:
`` `CPP
VOID NewMSG (FB_MSG & MSG) {
  if (msg.query) Bot.answer ("Hello!", True);
}
`` `

> If nothing is answered, the library itself will send an empty answer and the "timer" on the button will disappear.

<a id="unix"> </a>
## time module
The library has a data type `fb_time`, which is a structure with the fields:
`` `CPP
uint8_t second;// seconds
uint8_t minute;// minutes
Uint8_t Hour;// watch
uint8_t day;// Day of the month
uint8_t months;// month
uint8_t dayweek;// Day of the week (Mon.. SS 1..7)
uint16_t year;// year
`` `

When creating the structure, you can specify UNIX time and time zone in hours or minutes (for example, 3 hours or 180 minutes for Moscow (UTC+3: 00),
330 minutes for India (UTC+5: 30)).After that, you can take the necessary values of time:

`` `CPP
Fb_time t (1651694501, 3);
Serial.print (T.Hour);
Serial.print (':');
Serial.print (T.Minute);
Serial.print (':');
Serial.print (T.Second);
Serial.print ('');
Serial.print (t.day);
Serial.print (':');
Serial.print (T.MONTH);
Serial.print (':');
Serial.println (t.year);
`` `

From version 2.9, the library knows how to display a formatted time (string):
`` `CPP
Serial.print (t.timestring ());// HF: MM: SS
Serial.print ('');
Serial.println (T.Datestestring ());// DD.MM.YYYY
`` `

<a id="time"> </a>
##
In the processor of incoming messages, the `fb_msg` structure has a` unix` field, it stores the message time in the Unix format.
For transfer to a more readable format, we act according to the described above scheme:
`` `CPP
VOID NewMSG (FB_MSG & MSG) {
  Fb_time t (msg.unix, 3);// transferred unix and time zone
  Serial.print (t.timestring ());
  Serial.print ('');
  Serial.println (T.Datestestring ());
}
`` `

<a id="rtc"> </a>
## Real Time Watch
In response to any message from the bot, the server reports the time of sending in Unix format.From version 2.6, this time is parsing
The library and ** the account continues further ** using standard functions of time.Thus it is enough to send once once
The message after the inclusion of the board so that the library synchronizes the clock.During further shipments, time will also synchronize
And specify, becauseThe time calculated by ESP means will take (~ 2 seconds per day).Tools:

- `uint32_t getunix ()` - will return the current time in Unix format or `0` if time is not synchronized.
- `Bool Timesynced ()` - will return `true` if the clock is synchronized.
- `fb_time gettime (GMT)` - you need to convey your watch belt, it will return `fb_time`.

Thus, you can get time in two ways (see Timetest example):
`` `CPP
Fb_time t = Bot.gettime (3);
// or
Fb_time t (Bot.getunix (), 3);
`` `

<a id="ota"> </a>
## Chat firmware update
From the version of the library 2.13, an update of the firmware "by air" (OTA) through the chat appeared.For updating you need:
- compile the program to the file:*arduino ide/sketch/export of the binary file*(file **. Bin ** will appear in the dick with a sketch)
- Send a file to a chat with a bot
    - you can add a signature to the file
- the file can be sent from another chat
- the file will be processed as a conventional incoming message from the user `msg`
    - the signature to the file can be obtained from the field `msg.text`
- The name of the file can be obtained from the field `msg.filename`
    - The `msg flag will be raised.ota` *(in the event that the file has .bin extension) *
- To launch the process of updating the firmware, you need to call `update ()` inside the message processor
    - In version 2.20, the possibility of updating Spiffs appeared - you need to call `updatefs ()` `
- In the same chat chat, the update status will be sent (*ok*or*error*)
- after a successful update, ESP will reboot

### Examples of scenarios of firmware update
`` `CPP
// update if you just sent a bin file
if (msg.ota) BOT.UPDATE ();

// update if the file has the desired signature
if (msg.ota && msg.text == "update") BOT.UPDATE ();

// update if the file has the right name
if (msg.ota && msg.filename == "update.bin") Bot.update ();

// update if a famous person has sent (admin)
if (msg.ota && msg.chatid == "123456") BOT.UPDATE ();
`` `

### Examples of SPIFFS update scripts
`` `CPP
// update spiffs if you come a file in which there is a word spiffs
IF (msg.ota && msg.filename.indexof ("spiffs")> 0) BOT.UPDATEFS ();
`` `

### compression of the binary
If the firmware weighs a lot, you can squeeze it to GZIP:
- Recommended compression level - 9
- The file name should end in *.bin.gz *
- the file is in the same way sent to a chat with a bot or sent to it
- In the firmware before connecting all libraries, you need to declare `#define atomic_fs_update`

<a Id="textmode"> </a>
## text design
The library supports the design of the text in messages.The marking of the design is selected using `settextmode (mode)`, where `mode`:
- `fb_text` - by default (decoration is disabled)
- `fb_markdown` - marking markdown v2
- `fb_html` - marking html

Available tags are described in [API Telegram] (https://core.telegram.org/bots/api#formatting-options).For example for Markdown:
`` `CPP
Bot.Settextmode (fb_markdown);
BOT.SENDMESSAGE ("*Bold*, ~ Strike ~,` Code`, [Alexgyver.ru] (https://alexgyver.ru/)));
`` `

It will bring to the chat: ** bold **, ~~ strike ~~, `code`, [alexgyver.ru] (https://alexgyver.ru/)

> ** Attention! ** In fb_markdown mode, you cannot use the symbols `in messages!+ #`, the message will not go.It may be possible to fix in the future (the problem of Urlencode and the screenings of reserved characters).

<a id="files"> </a>
## Sending files (v2.20+)
You can send the following types (type is indicated when sending), in telegram these are different types of messages:
- `fb_photo` - picture (jpg, png ...)
- `fb_audio` - audio (mp3, wav ...)
- `fb_doc` - document (txt, pdf ...)
- `fb_video` - video (avi, mp4 ...)
- `fb_gif` - animation (GIF)
- `fb_voice` - voice message (ogg)

> You can edit all the above types of messages except ** fb_voice **!

> When sending, you need to specify the file name with the same extension with which it is created or stored in memory.

The library supports two options for sending files: from the buffer (RAM) and from Spiffs.

### File from a buffer
To send, you need to transfer the buffer, its size, file type, its size and ID chat (without specifying the chat ID, a chat from Setchatid will be used).
For editing, you also need to specify the ID messages:
`` `CPP
Uint8_t Sendfile (Uint8_t* Buf, Uint32_t Length, FB_FileType Type, Constation String & Name, Constance String & ID);
Uint8_t Editfile (Uint8_t* Buf, Uint32_T Length, FB_FileType Type, Constation String & Name, Int32_T MSGID, COST String & ID);
`` `

Send the text in the form of a text file, thus you can conduct and unload logs:
`` `CPP
  Char Buf [] = "Hello, World!";
  BOT.SENDFILE ((Byte*) BUF, Strlen (BUF), FB_DOC, "TEST.TXT", Chat_ID);
`` `

We will send a photo from the camera (see example *sendcamphoto *):
`` `CPP
  Frame = ESP_camera_FB_GET ();
  BOT.SENDFILE ((byte*) frame-> buf, frame-> len, fb_photo, "Photo.jpg", Chat_id);
`` `

### File from memory
Instead of a buffer and its size, the sending function accepts the file, the rest - as when sending from the buffer:
`` `CPP
Uint8_t Sendfile (File & File, FB_FileTYPE TYPE, COST String & NAME, COST String & ID);
uint8_t editfile (File & File, FB_FileTYPE TYPE, COST String & NAME, Int32_T MSGID, COST String & ID);
`` `

To work with files, you need to connect the library, kkotorI determine the `File` class, for example, spiffs.h or littlefs.h.
> Connect the library to (above the code) connection Fastbot!Otherwise, functions with File will be inaccessible.

Let's send a picture from memory:
`` `CPP
  File File = Littlefs.open ("/Test.png", "R");
  BOT.SENDFILE (FILE, FB_PHOTO, "TEST.PNG", Chat_id);
  file.close ();
`` `

<a d="download"> </a>
## download files (v2.20+)
From version 2.20 in the object of the incoming message there is a link to the file if there is a file in the message.This allows you to download the file in the internal memory.

To download files by Fastbot, you need to connect the library that defines the `File` class, for example Spiffs.h or Littlefs.h.
> Connect the library to (above the code) connection Fastbot!Otherwise, functions with File will be inaccessible.

To download the file you need to open/create a file with the rights for recording and transfer it to `downloadFile ()` along with a link to the file.
`` `CPP
VOID NewMSG (FB_MSG & MSG) {
  if (msg.ismfile) {// This is a file
    Serial.print ("downloading");
    Serial.println (msg.filename);

    String Path = '/' + MSG.FileName;// Path of the species /Filename.xxx
    File F = Littlefs.open (Path, "W");// Open for recording
    Bool Status = Bot.DownLoadfile (F, MSG.Fileurl);// Download
    Serial.println (status? "OK": "error");// Status
  }
}
`` `

<a id="location"></a>
## Location
If defined `#define FB_WITH_LOCATION` bot append `location` field into message (FB_msg):

```cpp
struct FB_Location {
  String &latitude;
  String &longitude;
};
```

Fields latitude/longitude will be filled if bot received location from user.

```cpp
void newMsg(FB_msg& msg) {
  if (msg.location.latitude.length() > 0 && msg.location.longitude.length() > 0) {
    bot.sendMessage("Lat: " + msg.location.latitude + ", Lon: " + msg.location.longitude, msg.chatID);
  }
}
```

See examples in `examples/location` and `examples/sunPosition`.

<a id="tricks"></a>

## Tricks
### Reboot
Messages are noted read at the next (relative to the current message processor) update in Tick (), that is, after at least a tuned timeout.
If you want to restart ESP on command, then this is the design
```CPP
VOID Message (FB_MSG & MSG) {
  if (msg.text == "restart") ESP.Restart ();
}
```
He will lead to a bootle (endless rebooting), because the message will not be noted read.You can raise the flag by which to go into the reboot, having previously caused Tickmanual:
```CPP
Bool Res = 0;
VOID Message (FB_MSG & MSG) {
  if (msg.text == "restart") res = 1;
}
void loop() {
  bot.tick();
  if (res) {
    bot.tickManual(); // To mark the message as read
    ESP.restart();
  }
}
```

### Pass "missed" messages based on time
The library has a Skipupdates function that allows you to skip all unread messages.But sometimes it is convenient to navigate in time.

If you need to ignore the messages sent by the Juzer while the bot was offline (or turned off), then you can do this:
- Remember UNIX-time when the bot went online
- Compare the time of the current message with him.If it is less, ignore the message

An example of passing messages sent before the launch of the controller:
`` `CPP
uint32_t Startunix;// Storing time

VOID setup () {
  // connectwifi ();

  BOT.ATTACH (NewMSG);
  BOT.SENDMESSAGE ("Start", "1234");// Send a message to get time
  Startunix = Bot.getunix ();// remembered
}

// Message Processer
VOID NewMSG (FB_MSG & MSG) {
  if (msg.unix <startunix) return;// ignore messages
  // ....
}
`` `

<a id="versions"> </a>
## versions
- V1.0
- V1.1 - Optimization
- v1.2 - you can set several Chatid and send to the specified chat
- v1.3 - added the opportunity to set the text when opening and closing the menu
- v1.3.1 - Fixed errors with 1.3
- v1.4 - added the ability to delete messages
- V1.5 - optimization, the possibility of changing token, new Parsing messages (ID, name, text)
- v1.5.1 - we also get ID messages
- v1.6 - added FB_DYMIC_HTTP mode, reading user name
- V1.7:
    - removed the dynamic mode fb_dynamic_http, it works too slowly
    - corrected Warning
    - fixed the work of the bot in the "Group" (negative chat ID)
    - memory optimization
    - Accelerated work
    - The work fixed the work in the script "Echo"
  
- V2.0:
    - removed at least 3200 ms
    - added the processing of the unico (Russian, emoji).Thanks to Gleb Zhukov!
    - extra spaces are removed from the menu, it has become easier to work
    - ESP32 support
    - Great optimization
    - added collels to Inlinemenu
    - added ID user
    - added editingCranberries and a bunch of everything

- V2.1:
    - More optimization
    - added text formatting (Markdown, HTML)
    - added the answer to the message

- V2.2:
    - Big optimization of memory and performance
    - added notify () - Notifications from Bot messages
    - added a one -time keyboard show
    
- V2.3: Small optimization
- V2.4: added URL Encode for the text of messages
- V2.5: added flags to fb_MSG: the message is edited and the message was sent by the bot.Improved Parsing of the text
- V2.6: added the built -in hours of real time
- V2.7: added the sending of stickers
- V2.8: removed the extra conclusion to the series, GMT can be in minutes
- V2.9: Fixed a bug in parsing, Parsing is accelerated, the output of the formatted time has been added, the surname and message time was added
- V2.10: Added functions to change the name and description of the chat, consolidation and reinforcement of messages.Removed EDIT/DeleteMessageid, EDIDITMENUID
- V2.11:
    - Optimization, correction of bugs
    - Callback Data is now parsing separately in Data
    - Reded work from Callback
    - Added toostring () for fb_msg for debugging
    - Callback added to the processing of URL addresses
    - removed FIRST_NAME and LAST_NAME (with the preservation of Legashi)
    - Usrid and ID are renamed userid and Messageid (while preserving Legasi)
    - finally removed the old handle of incoming messages

- V2.12: Examples are adjusted, ISBOT Parsing is fixed, the mechanism of protection against long messages has been redone, initialization was redone.
- V2.13: memory optimization.Added OTA update
- V2.14: Improved lines with IDs, added a shutdown of OTA, added Parsing the name of the group/channel in username
- v2.15: patch for the ESP32 library curve
- V2.16: added Filename output, indispensable messages in MarkDown are filmed
- v2.17: output of the text of the message to which the user answered + correct work with Menu in groups
- v2.17.1: small fix https://github.com/gyverlibs/fastbot/issues/12
- V2.18: added FB_dynamic mode: the library executes the request longer, but takes 10 kb less memory in SRAM
- V2.19: OTA support with GZIP compression
- V2.20:
    - added OTA SPIFFS + Example
    - added the output of the URL file for downloading from the chat + example
    - added the opportunity to download the file from the chat
    - added the possibility of sending files (from spiffs or buffer) + example
    - added the possibility of editing files (from spiffs or buffer)
    - added an example of sending a photo from the ESP32-CAM camera
- V2.21: accelerated the sending of files to the chat
- V2.22: Small optimization, corrected the compilation error at the define fb_no_ota
- V2.23: Completed a real time source for EditMessage
- V2.24: Fix of sending large files https://github.com/gyverlibs/fastbot/pull/17
- V2.25: added Skipupdates - passing unread messages
- V2.26: Fix of an incorrect display of numbers after Russian letters https://github.com/gyverlibs/fastbot/pull/37

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code
