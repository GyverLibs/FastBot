// бот отправляет обратно сообщения из чата (эхо-бот)

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "574578754"

#include <ESP8266WiFi.h>
#include "FastBot.h"
FastBot bot;

void setup() {
  connectWiFi();

  bot.setToken(BOT_TOKEN);
  bot.attach(newMsg);
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  // выводим ID чата, имя юзера и текст сообщения
  Serial.print(msg.chatID);
  Serial.print(", ");
  Serial.print(msg.name);
  Serial.print(", ");
  Serial.println(msg.text);    
    
  // отправить сообщение обратно
  bot.sendMessage(msg.text, msg.chatID);  
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
