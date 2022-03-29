// ответ на коллбэк

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "574578754"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  bot.setChatID(CHAT_ID);   // передай "" (пустую строку) чтобы отключить проверку
  bot.attach(newMsg); // подключаем функцию-обработчик
  String menu1 = "Menu 1 \t Menu 2 \t Menu 3 \n Back";
  bot.inlineMenu("Menu 0", menu1);
}

void newMsg(FB_msg& msg) {
  if (msg.query) bot.answer("Hello!", true);
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
