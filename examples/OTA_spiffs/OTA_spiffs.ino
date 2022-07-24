/*
  Пример обновления прошивки и SPIFFS из чата.
  Файл для FS генерируется через плагин для IDE:
  - esp32 data uploader https://github.com/lorol/arduino-esp32fs-plugin
  - esp8266 data uploader https://github.com/earlephilhower/arduino-esp8266littlefs-plugin
  При нажатии кнопки Data Upload (и отключенной плате) снизу в логе находим путь к папке билда. Например:
  - C:\Users\Alex\AppData\Local\Temp\arduino_build_232786
  оттуда берём bin файл с датой (spiffs). Закидываем в чат
  Если зайти на IP платы - там будет простенький файловый менеджер для проверки
*/

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

// для проверки файлов (веб-файловый менеджер)
#include <LittleFS.h>
#ifdef ESP8266
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
#else
#include <WebServer.h>
WebServer server(80);
#endif

void setup() {
  connectWiFi();
  Serial.println(WiFi.localIP());
  bot.attach(newMsg);
  setWebface();
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());

  // Обновить, если прислали bin файл.
  // В данном случае я вручную проверяю, к какому типу
  // относится загруженный бинарник:
  // если имя файла содержит mklittlefs (esp8266) или spiffs (esp32) -
  // даю команду на обновление SPIFFS, иначе - обновляем прошивку
  if (msg.OTA) {
    if (msg.fileName.indexOf("mklittlefs") > 0 ||
        msg.fileName.indexOf("spiffs") > 0) {
      bot.updateFS(); // обновить spiffs
    } else {
      bot.update();   // обновить прошивку
    }
  }
}

void loop() {
  bot.tick();
  server.handleClient();
}

// веб-сервер для проверки файлов (файловый менеджер)
void setWebface() {
  // запускаем FS для проверки
  if (!LittleFS.begin()) {
    Serial.println("FS Error");
    return;
  }

  server.begin();
  server.on("/", []() {
    String str;
    str += F("<!DOCTYPE html>\n");
    str += F("<html><body>\n");

#ifdef ESP8266
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
      if (dir.isFile()) {
        str += F("<a href=\"/");
        str += dir.fileName();
        str += F("\">");
        str += dir.fileName();
        str += F("</a><br>\n");
      }
    }
#else
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file) {
      if (!file.isDirectory()) {
        str += F("<a href=\"/");
        str += file.name();
        str += F("\">");
        str += file.name();
        str += F("</a><br>\n");
      }
      file = root.openNextFile();
    }
#endif

    str += F("</body></html>");
    server.send(200, "text/html", str);
  });

  // выбор файла (url - имя файла) - выводит файл в браузере
  server.onNotFound([]() {
    File file = LittleFS.open(server.uri(), "r");
    if (!file) {
      server.send(200, "text/plain", "Error");
      return;
    }
    server.streamFile(file, "text/plain");
    file.close();
  });
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
