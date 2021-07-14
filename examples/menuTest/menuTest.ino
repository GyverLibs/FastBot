// тест инлайн меню с удалением сообщения
// отправь Show menu для открытия меню

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "574578754"

#include <ESP8266WiFi.h>
#include "FastBot.h"
FastBot bot(BOT_TOKEN);

String menu1 = "Menu 1 \t Menu 2 \t Menu 3 \n Back";
String menu2 = "Answer 1.1 \t Answer 1.2 \t Answer 1.3 \n Back";
String menu3 = "Answer 2.1 \t Answer 2.2 \t Answer 2.3 \n Back";
String menu4 = "Answer 3.1 \t Answer 3.2 \t Answer 3.3 \n Back";
byte depth = 0;

void setup() {
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

  bot.setPeriod(200);
  bot.setChatID(CHAT_ID);   // передай "" (пустую строку) чтобы отключить проверку
  bot.attach(parseHandler); // подключаем функцию-обработчик
  bot.inlineMenu("Menu 0", menu1);
}

// создать свою функцию вида имя(String& ник, String& сообщение)
void parseHandler(String& name, String& msg) {
  Serial.println(msg);
  bot.deleteMessage(0);
  if (msg.indexOf("Menu 1") >= 0) {
    bot.inlineMenu("Menu 1", menu2);
    depth = 1;
  }
  if (msg.indexOf("Menu 2") >= 0) {
    bot.inlineMenu("Menu 2", menu3);
    depth = 1;
  }
  if (msg.indexOf("Menu 3") >= 0) {
    bot.inlineMenu("Menu 3", menu4);
    depth = 1;
  }
  if (msg.indexOf("Back") >= 0 && depth == 1) {
    bot.inlineMenu("Menu 0", menu1);
    depth = 0;
  }

  if (msg.indexOf("Show menu") >= 0) {
    bot.inlineMenu("Menu 0", menu1);
  }
}

void loop() {
  bot.tick();
}
