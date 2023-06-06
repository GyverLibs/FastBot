// бот отправляет в ответ на присланное местоположение (location) австрономические данные о солнце (восход, заход, длительность светового дня)
// полученные с помощью библиотеки https://github.com/GyverLibs/SunPosition

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define FB_WITH_LOCATION // enable location in FB_msg

#include <SunPosition.h> // https://github.com/GyverLibs/SunPosition
#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  bot.attach(newMsg);
}

String dayTimeStr(int minutesFromMidnight) {
  int hour = minutesFromMidnight / 60;
  int min = minutesFromMidnight % 60;

  String hs(hour);
  String ms(min);

  if (hour < 10) { hs = "0" + hs; }
  if (min < 10) { ms = "0" + ms; }


  String s = hs + ":" + ms;
  return s;
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());

  // в ответ на присланное местоположение (location) бот отправляет положение солнца
  if (msg.location.latitude.length() > 0 && msg.location.longitude.length() > 0) {
    int utcOffset = 3*60; // UTC +0300 (Moscow)
    SunPosition sun(msg.location.latitude.toFloat(), msg.location.longitude.toFloat(), msg.unix, utcOffset);

    String s= "Sunrise:\t" + dayTimeStr(sun.sunrise()) + "\n";
    s += "Sunset:\t" + dayTimeStr(sun.sunset()) + "\n";
    s += "Noon:\t" + dayTimeStr(sun.noon()) + "\n";
    s += "Daylight time:\t" + dayTimeStr(sun.daylight()) + "\n";
    s += "\n";
    s += "Location: " + msg.location.latitude + ", " + msg.location.longitude + "\n";
    s += "Unix time: " + String(msg.unix) + "\n\n";
    s += "https://github.com/GyverLibs/SunPosition";

    bot.sendMessage(s, msg.chatID);
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
