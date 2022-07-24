// тест отправки файлов разных типов
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
  
  // текстовый файл, отправленный бинарником из буфера
  char buf[] = "Hello, World!";
  bot.sendFile((byte*)buf, strlen(buf), FB_DOC, "test_raw.txt", CHAT_ID);
  
  // файлы из spiffs
  File file;

  file = LittleFS.open("/test.txt", "r");
  bot.sendFile(file, FB_DOC, "test.txt", CHAT_ID);
  file.close();

  file = LittleFS.open("/test.png", "r");
  bot.sendFile(file, FB_PHOTO, "test.png", CHAT_ID);
  file.close();
  
  file = LittleFS.open("/test.gif", "r");
  bot.sendFile(file, FB_GIF, "test.gif", CHAT_ID);
  file.close();
  
  file = LittleFS.open("/test.mp3", "r");
  bot.sendFile(file, FB_AUDIO, "test.mp3", CHAT_ID);
  file.close();
  
  file = LittleFS.open("/test.ogg", "r");
  bot.sendFile(file, FB_VOICE, "test.ogg", CHAT_ID);
  file.close();
  
  file = LittleFS.open("/test.mp4", "r");
  bot.sendFile(file, FB_VIDEO, "test.mp4", CHAT_ID);
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

void loop() {
}
