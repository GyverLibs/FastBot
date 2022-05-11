#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();
  bot.sendMessage("start", CHAT_ID);
  bot.attach(newMsg);
}

void newMsg(FB_msg& msg) {
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());
  
  FB_Time t(msg.unix, 3);
  Serial.print(t.timeString());
  Serial.print(' ');
  Serial.println(t.dateString());
}

void loop() {
  bot.tick();

  static uint32_t tmr;
  if (millis() - tmr >= 1000) {
    tmr = millis();
    // выведем время и дату
    FB_Time t = bot.getTime(3);
    
    /*Serial.print(t.hour);
    Serial.print(':');
    Serial.print(t.minute);
    Serial.print(':');
    Serial.print(t.second);
    Serial.print(' ');
    Serial.print(t.day);
    Serial.print(':');
    Serial.print(t.month);
    Serial.print(':');
    Serial.println(t.year);*/
    
    Serial.print(t.timeString());
    Serial.print(' ');
    Serial.println(t.dateString());
  }
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
