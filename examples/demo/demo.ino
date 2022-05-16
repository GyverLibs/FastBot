// общее демо библиотеки. Обязательно смотри остальные примеры и документацию на гитхаб!
// где получить токен и ID https://kit.alexgyver.ru/tutorials/telegram-basic/

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "123456789"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  connectWiFi();

  // можно сменить токен
  //bot.setToken(BOT_TOKEN);

  // можно сменить размер буфера на (приём, отправку), по умолч. 512, 512
  //bot.setBufferSizes(1024, 512);

  // установить ID чата, чтобы принимать сообщения только из него
  // узнать ID можно из ручного запроса в браузере
  bot.setChatID(CHAT_ID); // передай "" (пустую строку) чтобы отключить проверку

  // можно указать несколько ID через запятую
  //bot.setChatID("123456,7891011,12131415");

  // подключаем функцию-обработчик
  bot.attach(newMsg);

  // отправить сообщение в указанный в setChatID
  bot.sendMessage("Hello, World!");
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  // выводим имя юзера и текст сообщения
  //Serial.print(msg.username);
  //Serial.print(", ");
  //Serial.println(msg.text);
  
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());
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
