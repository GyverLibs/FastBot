// общее демо библиотеки. Обязательно смотри остальные примеры и документацию на гитхаб!

// узнать токен бота можно у BotFather
// узнать chat id можно по запросу https://api.telegram.org/bot<token>/getUpdates
// искать "chat":{"id": тут id

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "574578754"

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

  // отправить сообщение в указанный ЗДЕСЬ чат/чаты (разделитель - запятая)
  //bot.sendMessage("Hello, World! External ID", "123456,7891011,12131415");

  // показать юзер меню (\t - горизонтальное разделение кнопок, \n - вертикальное
  //bot.showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");

  // аналогично в указанный конкретно ЗДЕСЬ чат/чаты
  //bot.showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4", "123456,7891011,12131415");

  // скрыть юзер меню
  //delay(3000);
  //bot.closeMenu();  // для всех в setChatID
  //bot.closeMenu("123456,7891011,12131415"); // конкретный чат/чаты

  // отправить инлайн меню (\t - горизонтальное разделение кнопок, \n - вертикальное
  // (текст сообщения, кнопки)
  //bot.inlineMenu("Choose wisely", "Answer 1 \t Answer 2 \t Answer 3 \n Answer 4");

  // инлайн меню с коллбэком
  String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
  String cback1 = F("action1,action2,action3,back");
  bot.inlineMenuCallback("Menu 1", menu1, cback1);
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  // выводим ID чата, имя юзера и текст сообщения
  Serial.print(msg.chatID);     // ID чата
  Serial.print(", ");
  Serial.print(msg.username);   // логин
  Serial.print(", ");
  Serial.print(msg.first_name); // имя
  Serial.print(", ");
  Serial.print(msg.usrID);      // ID юзера
  Serial.print(", ");
  Serial.print(msg.ID);         // ID сообщения
  Serial.print(", ");
  Serial.println(msg.text);     // текст
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
