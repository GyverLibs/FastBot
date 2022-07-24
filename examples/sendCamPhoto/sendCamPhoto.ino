/*
  Отправка фото с камеры в чат Телеграм
  Съёмка и обновление фото на новое, получается "видео" ~1 кадр в секунду
  - плата: AI Thinker ESP32-CAM
  - разрешение изображения можно настроить в camera.h
*/

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include "camera.h"
camera_fb_t* frame;

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  // инициализация камеры
  if (initCamera()) Serial.println("Camera OK");
  else Serial.println("Camera Error");

  // отправить первое фото в чат
  frame = esp_camera_fb_get();
  if (frame) {
    bot.sendFile((byte*)frame->buf, frame->len, FB_PHOTO, "photo.jpg", CHAT_ID);
    esp_camera_fb_return(frame);
  } else Serial.println("Capture Error");
}

void loop() {
  // изменить первое фото на свежее
  frame = esp_camera_fb_get();
  if (frame) {
    bot.editFile((byte*)frame->buf, frame->len, FB_PHOTO, "photo.jpg", bot.lastBotMsg(), CHAT_ID);
    esp_camera_fb_return(frame);
  } else Serial.println("Capture Error");
  delay(50);
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
