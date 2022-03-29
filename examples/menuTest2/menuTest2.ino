// тест инлайн меню с изменением сообщения
// отправь Show menu для открытия меню

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "574578754"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

String menu1 = "Menu 1 \t Menu 2 \t Menu 3 \n Back";
String menu2 = "Answer 1.1 \t Answer 1.2 \t Answer 1.3 \n Back";
String menu3 = "Answer 2.1 \t Answer 2.2 \t Answer 2.3 \n Back";
String menu4 = "Answer 3.1 \t Answer 3.2 \t Answer 3.3 \n Back";
byte depth = 0;

void setup() {
  connectWiFi();
  
  bot.setChatID(CHAT_ID);   // передай "" (пустую строку) чтобы отключить проверку
  bot.attach(newMsg); // подключаем функцию-обработчик
  bot.inlineMenu("Menu 0", menu1);
}

// создать свою функцию вида имя(String& ник, String& сообщение)
void newMsg(FB_msg& msg) {
  Serial.println(msg.text);
  
  if (msg.text == "Menu 1") {
    bot.editMenuID(bot.lastBotMsg(), menu2, "", msg.chatID);
    depth = 1;
  }
  if (msg.text == "Menu 2") {
    bot.editMenuID(bot.lastBotMsg(), menu3, "", msg.chatID);
    depth = 1;
  }
  if (msg.text == "Menu 3") {
    bot.editMenuID(bot.lastBotMsg(), menu4, "", msg.chatID);
    depth = 1;
  }
  if (msg.text == "Back" && depth == 1) {
    bot.editMenuID(bot.lastBotMsg(), menu1, "", msg.chatID);
    depth = 0;
  }

  if (msg.text == "Show menu") {
    bot.inlineMenu("Menu 0", menu1);
  }
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
