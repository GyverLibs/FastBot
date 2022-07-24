// тест изменения файла в чате (меняем одну картинку на другую)
// загрузи данные в SPIFFS (лежат в папке data)
// - esp32 data uploader https://github.com/lorol/arduino-esp32fs-plugin
// - esp8266 data uploader https://github.com/earlephilhower/arduino-esp8266littlefs-plugin

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include <LittleFS.h>
#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  if (!LittleFS.begin()) {
    Serial.println("FS Error");
    return;
  }
  
  // отправляем картинку в чат, это будет сообщение с номером bot.lastBotMsg()
  File file = LittleFS.open("/test.png", "r");
  bot.sendFile(file, FB_PHOTO, "test.png", CHAT_ID);
  file.close();
}

void loop() {
  // меняем на вторую
  File file;
  file = LittleFS.open("/test2.png", "r");
  bot.editFile(file, FB_PHOTO, "test2.png", bot.lastBotMsg(), CHAT_ID);
  file.close();

  // снова меняем на первую
  file = LittleFS.open("/test.png", "r");
  bot.editFile(file, FB_PHOTO, "test.png", bot.lastBotMsg(),  CHAT_ID);
  file.close();
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
