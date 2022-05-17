// бот-модератор. Удаляет сообщения, содержащие указанные сочетания букв

// стоп-лист
#define FILT F("жопа,дурак,редиска,мразь,гадина,блудница")

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();
  bot.attach(newMsg);
}

void newMsg(FB_msg& msg) {
  String filt(FILT);
  // FB_Parser - встроенный инструмент для парсинга разделённых запятой данных
  FB_Parser p;
  while (p.parse(filt)) {
    // парсим все стоп-слова. Если в сообщении содержится сочетание - удаляем его
    if (msg.text.indexOf(p.str) >= 0) bot.deleteMessage(msg.ID, msg.chatID);
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