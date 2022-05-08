// обычное меню. С кнопкой закрыть (для примера)

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();
  bot.setChatID(CHAT_ID);

  // подключаем функцию-обработчик
  bot.attach(newMsg);

  // показать юзер меню (\t - горизонтальное разделение кнопок, \n - вертикальное
  bot.showMenu("Menu1 \t Menu2 \t Menu3 \n Close");
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  if (msg.text == "Close") bot.closeMenu();
  else Serial.println(msg.text);
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
