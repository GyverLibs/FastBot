// бот отправляет географические координаты в ответ на полученное местоположение (location)

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define FB_WITH_LOCATION // enable location in FB_msg

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  bot.attach(newMsg);
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());

  // в ответ на присланное местоположение (location) бот отправляет его координаты
  if (msg.location.latitude.length() > 0 && msg.location.longitude.length() > 0) {
    bot.sendMessage("Latitude: " + msg.location.latitude + ", Longitude: " + msg.location.longitude, msg.chatID);
  } else {
    bot.sendMessage("Unknown location", msg.chatID);
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