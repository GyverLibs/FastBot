// закрепление и открепление сообщений
// напиши unpin для открепления всех сообщений

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();
  bot.setChatID(CHAT_ID);
  bot.attach(newMsg);
  bot.clearServiceMessages(true);
  bot.sendMessage("Hello!");
  bot.pinMessage(bot.lastBotMsg()); // закрепить последнее сообщение
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  if (msg.text == "unpin") bot.unpinAll();
}

void loop() {
  bot.tick();   // тикаем в луп
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
