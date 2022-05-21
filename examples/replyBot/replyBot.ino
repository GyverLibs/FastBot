// бот отвечает на сообщения из любого чата

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  bot.attach(newMsg);
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  Serial.println(msg.toString());

  // ответить
  bot.replyMessage("Hello!", msg.messageID, msg.chatID);  
}

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
