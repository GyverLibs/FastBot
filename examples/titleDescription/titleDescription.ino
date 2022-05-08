// меняем название и описание чата (бот должен быть админом)
// нужно отправить команду /start, чтобы бот запомнил chat id
// после этого по таймеру будет случайно задано имя и описание чата

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();
  bot.attach(newMsg);
  bot.clearServiceMessages(true);
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  if (msg.text == "/start") bot.setChatID(msg.chatID);
}

void loop() {
  bot.tick();   // тикаем в луп

  static uint32_t tmr;
  static int count;
  if (millis() - tmr >= 5000) {
    tmr = millis();
    String title;
    title += "Bot [";
    title += random(100);
    title += "]";
    bot.setChatTitle(title);

    String descr;
    descr += "Description #";
    descr += count;
    count++;
    descr += '\n';
    descr += "Current millis: ";
    descr += millis();
    bot.setChatDescription(descr);
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
