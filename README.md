![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# FastBot
Очень простая и быстрая библиотека для телеграм бота
- Оптимизирована для большой нагрузки (спокойно принимает 50 сообщ в секунду)
- Опциональная установка ID чата для общения с ботом
- Проверка обновлений вручную или по таймеру
- Сообщения приходят в функцию-обработчик
- Отправка сообщений в чат
- Вывод меню вместо клавиатуры
- Вывод инлайн меню в сообщении
- Возможность включить ручной инкремент новых сообщений

### Совместимость
ESP8266

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **FastBot** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/FastBot/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
FastBot bot;
FastBot bot(токен);
FastBot bot(токен, лимит);
FastBot bot(токен, лимит, порог);
FastBot bot(токен, лимит, порог, период);
// токен - уникальный код бота, берётся у BotFather
// лимит - количество сообщений, получаемое из одного запроса (по умолч. 10)
// порог - количество символов, при котором API запрос будет считаться слишком большим и будет пропущен (по умолч. 10000)
// период - период автоматического опроса бота в мс (по умолч. 1000)

// ДЕФАЙНЫ НАСТРОЕК
// дефайнить перед подключением библиотеки
#define FB_DYNAMIC_HTTP     // создавать WiFiClientSecure динамически при вызове tick (экономит статик оперативку)
```

<a id="usage"></a>
## Использование
```cpp
// настройки
void setChatID(String chatID);                  // установка ID чата для парсинга сообщений. Можно указать несколько через запятую
void setLimit(int limit);                       // макс кол-во сообщений на запрос
void setOvf(int ovf);                           // макс символов
void setPeriod(int period);                     // период опроса
void setToken(String token);                    // изменить/задать токен

// парсинг
void attach(void (*handler)(String&, String&)); // подключение обработчика сообщений
void attach(void (*FB_msg)(msg&));              // подключение обработчика сообщений
void detach();                                  // отключение ВСЕХ ОБРАБОТЧИКОВ СООБЩЕНИЙ

// структура входящих сообщений FB_msg
String& chatID      // id чата
String& username    // логин пользователя
String& first_name  // имя пользователя
String& text        // текст сообщения
String& ID          // ID сообщения

// тикер
uint8_t tickManual();                           // ручная проверка обновлений
uint8_t tick();                                 // проверка обновлений по таймеру

// сообщения
uint8_t sendMessage(String msg);                // отправить сообщение в указанный в setChatID чат/чаты
uint8_t sendMessage(String msg, String id);     // отправить сообщение в указанный здесь чат/чаты (через запятую)
uint8_t deleteMessage(int offset);              // удалить сообщение со смещением offset в указанный в setChatID чат/чаты
uint8_t deleteMessage(int offset, String id);   // удалить сообщение со смещением offset в указанном здесь ID чата/чатов

// меню
uint8_t showMenu(String str);                   // показать меню в указанном в setChatID чате
uint8_t showMenu(String str, String id);        // показать меню в указанном здесь чате/чатах (через запятую)
uint8_t closeMenu();                            // скрыть меню в указанном в setChatID чате
uint8_t closeMenu(String id);                   // скрыть меню в указанном здесь чате/чатах (через запятую)

uint8_t showMenuText(String msg, String str);               // показать меню str в указанном в setChatID чате + сообщение msg
uint8_t showMenuText(String msg, String str, String id);    // показать меню str в указанном здесь чате/чатах (через запятую) + сообщение msg
uint8_t closeMenuText(String msg);                          // скрыть меню в указанном в setChatID чате + сообщение msg
uint8_t closeMenuText(String msg, String id);               // скрыть меню в указанном здесь чате/чатах (через запятую) + сообщение msg

// инлайн меню
uint8_t inlineMenu(String msg, String str);     // показать инлайн меню в указанном в setChatID чате
uint8_t inlineMenu(String msg, String str, String id);  // показать инлайн меню в указанном здесь чате/чатах (через запятую)

// сервис
uint8_t sendRequest(String& req);               // отправить запрос
void autoIncrement(boolean incr);               // авто инкремент сообщений (по умолч включен)
void incrementID(uint8_t val);                  // вручную инкрементировать ID на val
String chatIDs;                                 // указанная в setChatID строка, для отладки и редактирования списка

// для отправки меню используется следующая конструкция:
// \t - горизонтальное разделение кнопок, \n - вертикальное
// Меню 3x1: "Menu1 \t Menu2 \t Menu3 \n Menu4"

// Многие функции возвращают статус:
// 0 - ожидание
// 1 - ОК
// 2 - Переполнен по ovf
// 3 - Ошибка телеграм
// 4 - Ошибка подключения
// 5 - не задан chat ID
// 6 - множественная отправка, статус неизвестен
```

<a id="example"></a>
## Особенности парсинга сообщений
Сообщения автоматически читаются в `tick()`, при поступлении нового сообщения вызывается указанная функция-обработчик. Но тут есть варианты:
- Если задан ID чата/чатов (через `setChatID()`) - происходит автоматическое отсеивание сообщений НЕ из указанных чатов
- Если ID чата/чатов не задан - сообщение будет обработано, т.е. вызван обработчик

Обработчик подключается при помощи `attach(FB_msg&)` или `attach(String&, String&)` (устаревший)
- Создаём в скетче свою функцию вида `void функция(FB_msg& сообщение)`
- Вызываем `attach(функция)`
- Эта функция будет автоматически вызвана при входящем сообщении, если ID чата совпадают или не настроены
- Внутри этой функции можно пользоваться переданной переменной `сообщение`, которая имеет тип `FB_msg` (структура) и содержит в себе:
    - `chatID` - id чата
    - `username` - логин пользователя
    - `first_name` - имя пользователя
    - `text` - текст сообщения
    - `ID` - ID сообщения

Минимальный пример:
```cpp
void setup() {
  // подключаемся к WiFi
  // .............
  
  // подключаем обработчик сообщений
  bot.attach(newMsg);
}

// функция-обработчик
void newMsg(FB_msg& msg) {
  // выводим ID чата, имя юзера и текст сообщения
  Serial.print(msg.chatID);
  Serial.print(", ");
  Serial.print(msg.username);
  Serial.print(", ");
  Serial.println(msg.text);
}

// проверка происходит в лупе
void loop() {
  bot.tick();
}
```

## Пример
```cpp
// узнать токен бота можно у BotFather

#define WIFI_SSID "login"
#define WIFI_PASS "pass"
#define BOT_TOKEN "2654326546:asjhAsfAsfkllgUsaOuiz_axfkj_AsfkjhB"
#define CHAT_ID "574578754"

#include <ESP8266WiFi.h>
#include "FastBot.h"
FastBot bot(BOT_TOKEN);

void setup() {
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
  
  // можно сменить токен
  //bot.setToken(BOT_TOKEN);

  // установить ID чата, чтобы принимать сообщения только из него
  // узнать ID можно из ручного запроса в браузере
  bot.setChatID(CHAT_ID); // передай "" (пустую строку) чтобы отключить проверку

  // подключаем функцию-обработчик
  bot.attach(newMsg);

  // отправить сообщение (работает, если указан CHAT_ID)
  bot.sendMessage("Hello, World!");
  
  //bot.sendMessage("Hello, World!", "123456789");  // отправить в чат с указанным ID

  // показать юзер меню (\t - горизонтальное разделение кнопок, \n - вертикальное
  bot.showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");

  // скрыть юзер меню
  //delay(3000);
  //bot.closeMenu();

  // отправить инлайн меню (\t - горизонтальное разделение кнопок, \n - вертикальное
  // (текст сообщения, кнопки)
  bot.inlineMenu("Choose wisely", "Answer 1 \t Answer 2 \t Answer 3 \n Answer 4");
}

// создать свою функцию вида имя(FB_msg& сообщение)
void newMsg(FB_msg& msg) {
  // выводим ID чата, имя юзера и текст сообщения
  Serial.print(msg.chatID);
  Serial.print(", ");
  Serial.print(msg.username);
  Serial.print(", ");
  Serial.println(msg.text);  
}

// можно вручную дёргать по одному сообщению при помощи tickManual()
// тогда "лимит" при инициализации поставить 1
void loop() {
  // тикаем в луп
  bot.tick();
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - оптимизация
- v1.2 - можно задать несколько chatID и отправлять в указанный чат
- v1.3 - добавлена возможность задать текст при открытии и закрытии меню
- v1.3.1 - исправлены ошибки с 1.3
- v1.4 - добавлена возможность удалять сообщения
- v1.5 - оптимизация, возможность смены токена, новый парсинг сообщений (id, имя, текст)
- v1.5.1 - получаем также ID сообщения
- v1.6 - добавлен режим FB_DYNAMIC_HTTP, чтение имени пользователя

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
