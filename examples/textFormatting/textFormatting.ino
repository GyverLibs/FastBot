/*
    Оформление текста с использованием FB_formatting.

    В боте:
      - Отправляем /start для просмотра работы с динамическими переменными.
        В данном примере переменная - случайное число (функция random()).
      - Отправляем любой текст и получаем этот же текст со случайным форматированием.
*/


#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

FB_formatting f;                            // Инициализация

String format(String input) {
  switch (random(6)) {
    case 0: return f.bold(input); break;
    case 1: return f.italic(input); break;
    case 2: return f.underline(input); break;
    case 3: return f.strikethrough(input); break;
    case 4: return f.spoiler(input); break;
    case 5: return f.code(input); break;
    case 6: return f.link("https://alexgyver.ru/", input); break;
  }
}

void setup() {
  connectWiFi();

  bot.setTextMode(FB_HTML);                 // Устанавливаем разметку `FB_HTML`
  bot.attach(newMsg);
}

void loop() {
  bot.tick();
}

void newMsg (FB_msg& msg){
  if (msg.text == "/start"){
  // В чат пишется случайное число со случайным форматированием
    bot.sendMessage(format(String(random())), msg.chatID);
  }
  else {
    // Пишем ручками любой текст и получаем этот же текст обратно со случайным форматированием
    bot.replyMessage(format(msg.text), msg.messageID, msg.chatID);
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


