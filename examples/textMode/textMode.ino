// бот отправляет обратно сообщения из любого чата (эхо-бот)

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  bot.setChatID(CHAT_ID);
  bot.setTextMode(FB_MARKDOWN);
  // по форматированию читай тут https://core.telegram.org/bots/api#formatting-options
  // символ точка . нужно экранировать как \\.
  bot.sendMessage(F("*Bold*, ||spoiler||, ~Strike~, `code`, [alexgyver\\.ru](https://alexgyver\\.ru/)"));

  // пример из доки
  // https://core.telegram.org/bots/update56kabdkb12ibuisabdubodbasbdaosd
  bot.sendMessage(F("*bold _italic bold ~italic bold strikethrough ||italic bold strikethrough spoiler||~ __underline italic bold___ bold*"));
}

/*
   bold \*text
  _italic \*text_
  __underline__
  ~strikethrough~
  ||spoiler||
  [inline URL](http://www.example.com/)
  [inline mention of a user](tg://user?id=123456789)
  `inline fixed-width code`
  ```
  pre-formatted fixed-width code block
  ```
  ```python
  pre-formatted fixed-width code block written in the Python programming language
  ```
*/

void loop() {
  bot.tick();
}

void connectWiFi() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}
