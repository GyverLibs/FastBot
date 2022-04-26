// отправь стикер боту @idstickerbot
// он вернёт ID стикера!

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();
  bot.setChatID(CHAT_ID);
  String sticker = F("CAACAgIAAxkBAAEEj8ZiZ9h67kyCKj6ZWNgc1Nn9jMxzvgACLA0AAvb9IUq2Q93aVm1qyCQE");
  bot.sendSticker(sticker);
}

void loop() {
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
