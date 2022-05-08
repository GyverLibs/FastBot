// тест инлайн меню с изменением сообщения
// отправь Show menu для открытия меню

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  bot.setChatID(CHAT_ID);
  bot.attach(newMsg);
  String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
  bot.inlineMenu("Menu 0", menu1);
}

byte depth = 0;
void newMsg(FB_msg& msg) {
  String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
  String menu2 = F("Answer 1.1 \t Answer 1.2 \t Answer 1.3 \n Back");
  String menu3 = F("Answer 2.1 \t Answer 2.2 \t Answer 2.3 \n Back");
  String menu4 = F("Answer 3.1 \t Answer 3.2 \t Answer 3.3 \n Back");
  
  if (msg.text == "Menu 1") {
    bot.editMenu(bot.lastBotMsg(), menu2);
    depth = 1;
  }
  if (msg.text == "Menu 2") {
    bot.editMenu(bot.lastBotMsg(), menu3);
    depth = 1;
  }
  if (msg.text == "Menu 3") {
    bot.editMenu(bot.lastBotMsg(), menu4);
    depth = 1;
  }
  if (msg.text == "Back" && depth == 1) {
    bot.editMenu(bot.lastBotMsg(), menu1);
    depth = 0;
  }

  if (msg.text == "Show menu") {
    bot.inlineMenu("Menu 0", menu1);
  }

  Serial.println(msg.text);
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
