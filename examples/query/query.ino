// ответ на коллбэк

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
  bot.inlineMenu("Menu 0", F("Menu 1 \t Menu 2 \t Menu 3 \n Back"));
}

void newMsg(FB_msg& msg) {
  if (msg.query) bot.answer("Hello!", FB_ALERT);  // предупреждение
  //if (msg.query) bot.answer("Hello!", FB_NOTIF);    // подсказка
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
