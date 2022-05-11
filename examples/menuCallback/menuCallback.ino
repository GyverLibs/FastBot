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
  String call1 = F("menu_1, menu_2, menu_3, https://alexgyver.ru/");
  bot.inlineMenuCallback("Menu 0", menu1, call1);
}

byte depth = 0;
void newMsg(FB_msg& msg) {
  String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
  String call1 = F("menu_1, menu_2, menu_3, back");
  
  String menu2 = F("Answer 1.1 \t Answer 1.2 \t Answer 1.3 \n Back");
  String call2 = F("answ_1_1, answ_1_2, answ_1_3, back");
  
  String menu3 = F("Answer 2.1 \t Answer 2.2 \t Answer 2.3 \n Back");
  String call3 = F("answ_2_1, answ_2_2, answ_2_3, back");
  
  String menu4 = F("Answer 3.1 \t Answer 3.2 \t Answer 3.3 \n Back");
  String call4 = F("answ_3_1, answ_3_2, answ_3_3, back");
  
  if (msg.data == "menu_1") {
    bot.editMenuCallback(bot.lastBotMsg(), menu2, call2);
    depth = 1;
  }
  if (msg.data == "menu_2") {
    bot.editMenuCallback(bot.lastBotMsg(), menu3, call3);
    depth = 1;
  }
  if (msg.data == "menu_3") {
    bot.editMenuCallback(bot.lastBotMsg(), menu4, call4);
    depth = 1;
  }
  if (msg.data == "back" && depth == 1) {
    bot.editMenuCallback(bot.lastBotMsg(), menu1, call1);
    depth = 0;
  }

  if (msg.text == "Show menu") {
    bot.inlineMenuCallback("Menu 0", menu1, call1);
  }

  Serial.println(msg.toString());
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
