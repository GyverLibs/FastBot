[![Foo](https://img.shields.io/badge/Version-2.2-brightgreen.svg?style=flat-square)](#versions)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)

# FastBot
Очень простая и быстрая библиотека для телеграм бота на esp8266/esp32
- Работает на стандартных библиотеках
- Работает без SSL
- Оптимизирована для большой нагрузки
- Опциональная установка ID чата для общения с ботом
- Проверка обновлений вручную или по таймеру
- Отправка/редактирование/ответ на сообщения
- Вывод меню вместо клавиатуры
- Вывод инлайн меню в сообщении
- Поддержка Unicode (другие языки + эмодзи)

### Совместимость
ESP8266, ESP32

## Документация и проекты
Подробные уроки по работе с Телеграм ботом при помощи этой библиотеки можно найти на [сайте Arduino набора GyverKIT](https://kit.alexgyver.ru/tutorials-category/telegram/)

## Сравнение с Universal-Arduino-Telegram-Bot
[Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)  

Для сравнения использовался минимальный пример с отправкой сообщения в чат и выводом входящих сообщений в сериал:
- **send** - функция, которая отправляет сообщение в чат
- **update** - функция, которую нужно постоянно вызывать для чтения ботом входящих сообщений в чате
- **free heap** - объём свободной оперативной памяти во время работы программы

| Library   | Flash, B | SRAM, B | send, ms | update, ms | free heap, B |
|-----------|----------|---------|----------|------------|--------------|
| Univ..Bot | 400004   | 29848   | 2000     | 1900       | 38592        |
| FastBot   | 393220   | 28036   | 70       | 70         | 37552        |
| diff      | 6784     | 1812    | 1930     | 1830       | 1040         |

- FastBot легче почти на 7 кБ Flash и 2 кБ SRAM, но занимает на 1 кБ в SRAM больше во время работы программы. Итого легче на 2-1 = 1 кБ SRAM.
- FastBot значительно быстрее обрабатывает чат и отправляет сообщения (на 2 секунды) за счёт ручного парсинга ответа сервера и статически выделенных HTTP клиентов

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Документация](#docs)
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
```

<a id="docs"></a>
## Документация
```cpp
// ============== НАСТРОЙКИ ==============
void setToken(String token);                    // изменить/задать токен бота
void setChatID(String chatID);                  // установка ID чата. Можно несколько через запятую ("id1,id2,id3")
void setPeriod(int period);                     // период опроса в мс (по умолч. 1000)
void setLimit(int limit);                       // макс кол-во сообщений на запрос
void setOvf(int ovf);                           // макс кол-во символов на запрос (защита от флуда)
void setBufferSizes(uint16_t rx, uint16_t tx);  // установить размеры буфера на приём и отправку, по умолч. 512 и 512 байт (только для esp8266)
   

// =============== ПАРСИНГ ===============
void attach(void (*FB_msg)(msg&));              // подключение обработчика сообщений
void attach(void (*handler)(String&, String&)); // подключение обработчика сообщений (устаревший)
void detach();                                  // отключение ВСЕХ ОБРАБОТЧИКОВ СООБЩЕНИЙ


// ================ ТИКЕР =================
uint8_t tickManual();                           // ручная проверка обновлений
uint8_t tick();                                 // проверка обновлений по таймеру


// ============== СООБЩЕНИЯ ===============
uint8_t sendMessage(String msg);                // отправить сообщение в указанный в setChatID чат/чаты
uint8_t sendMessage(String msg, String id);     // отправить сообщение в указанный здесь чат/чаты

// ответить на сообщение с id (replyID) в указанный в setChatID чат ИЛИ указать чат
uint8_t replyMessage(String msg, int32_t replyID);
uint8_t replyMessage(String msg, int32_t replyID, String id);

void answer(String text, bool alert = false);   // ответить на callback текстом (text) и режимом (alert) (true - предупреждение)
void setTextMode(uint8_t mode);                 // режим обработки текста: FB_TEXT, FB_MARKDOWN, FB_HTML
void notify(bool mode);                         // true/false вкл/выкл уведомления от сообщений бота (по умолч. вкл)

int32_t lastBotMsg();                           // ID последнего отправленного ботом сообщения
int32_t lastUsrMsg();                           // ID последнего отправленного юзером сообщения

// ============== УДАЛИТЬ ===============
uint8_t deleteMessage(int32_t offset);              // удалить сообщение со смещением (offset) в указанном в setChatID чате
uint8_t deleteMessage(int32_t offset, String id);   // удалить сообщение со смещением (offset) в указанном здесь чате
uint8_t deleteMessageID(int32_t msgid);             // удалить сообщение (msgid) в указанном в setChatID чате
uint8_t deleteMessageID(int32_t msgid, String id);  // удалить сообщение (msgid) в указанном чате


// ============ РЕДАКТИРОВАТЬ =============
// редактировать сообщение со смещением (offset) в указанном в setChatID чате
uint8_t editMessage(int32_t offset, String text);

// редактировать сообщение (msgid) в указанном в setChatID чате ИЛИ передать id чата
uint8_t editMessageID(int32_t msgid, String text);
uint8_t editMessageID(int32_t msgid, String text, String id);
   

// ============= ОБЫЧНОЕ МЕНЮ =============
// показать меню (menu) в указанном в setChatID чате/чатах ИЛИ передать id чата/чатов 
uint8_t showMenu(String menu);
uint8_t showMenu(String menu, String id);

// единоразовое меню (закроется при выборе)
uint8_t showMenu(String menu, true);
uint8_t showMenu(String menu, String id, true);

// скрыть меню в указанном в setChatID чате/чатах ИЛИ передать id чата/чатов 
uint8_t closeMenu();
uint8_t closeMenu(String id);


// ======== ОБЫЧНОЕ МЕНЮ С ТЕКСТОМ =========
// сообщение (msg) + показать меню (menu) в указанном в setChatID чате/чатах ИЛИ передать id чата/чатов 
uint8_t showMenuText(String msg, String menu);
uint8_t showMenuText(String msg, String menu, String id);

// единоразовое меню (закроется при выборе)
uint8_t showMenuText(String msg, String menu, true);
uint8_t showMenuText(String msg, String menu, String id, true);

// сообщение (msg) + скрыть меню в указанном в setChatID чате/чатах ИЛИ передать id чата/чатов 
uint8_t closeMenuText(String msg);
uint8_t closeMenuText(String msg, String id);


// ============= ИНЛАЙН МЕНЮ =============
// сообщение (msg) с инлайн меню (menu) в указанном в setChatID чате/чатах ИЛИ передать id чата/чатов
uint8_t inlineMenu(String msg, String menu);
uint8_t inlineMenu(String msg, String menu, String id);


// ======= ИНЛАЙН МЕНЮ С КОЛЛБЭКОМ =======
// сообщение (msg) с инлайн меню (menu) и коллбэком (cbck) в указанном в setChatID чате/чатах ИЛИ передать id чата/чатов
uint8_t inlineMenuCallback(String msg, String menu, String cbck);
uint8_t inlineMenuCallback(String msg, String menu, String cbck, String id);


// ======= РЕДАКТИРОВАТЬ ИНЛАЙН МЕНЮ С КОЛЛБЭКОМ =======
// редактировать меню (msgid) текстом (menu) в указанном в setChatID чате ИЛИ передать id чата
uint8_t editMenuID(int32_t msgid, String menu, String cback);
uint8_t editMenuID(int32_t msgid, String menu, String cback, String id);


// ================ СЕРВИС ===============
uint8_t sendRequest(String& req);               // отправить запрос (https://api.telegram.org/bot...)
void autoIncrement(boolean incr);               // авто инкремент сообщений (по умолч включен)
void incrementID(uint8_t val);                  // вручную инкрементировать ID на val
String chatIDs;                                 // указанная в setChatID строка, для отладки и редактирования списка


// =============== СТАТУС ================
// Многие функции возвращают статус:
// 0 - ожидание
// 1 - ОК
// 2 - Переполнен по ovf
// 3 - Ошибка телеграм
// 4 - Ошибка подключения
// 5 - не задан chat ID
// 6 - множественная отправка, статус неизвестен


// ========== ДЕФАЙНЫ НАСТРОЕК ==========
#define FB_NO_UNICODE   // отключить конвертацию Unicode
```

<a id="usage"></a>
## Использование
### ID чата/чатов
Устанавливается через `setChatID()`, куда можно передать одиночный ID или сразу несколько через запятую: `setChatID("id1,id2,id3")`

### Парсинг сообщений
Сообщения автоматически читаются в `tick()`, при поступлении нового сообщения вызывается указанная функция-обработчик. Но тут есть варианты:
- Если задан ID чата/чатов (через `setChatID()`) - происходит автоматическое отсеивание сообщений НЕ из указанных чатов
- Если ID чата/чатов не задан (через `setChatID()`) - сообщение будет обработано, т.е. вызван обработчик

Обработчик подключается при помощи `attach(FB_msg&)` или `attach(String&, String&)` (устаревший)
- Создаём в скетче свою функцию вида `void функция(FB_msg& сообщение)`
- Вызываем `attach(функция)`
- Эта функция будет автоматически вызвана при входящем сообщении, если ID чата совпадают или не настроены
- Внутри этой функции можно пользоваться переданной переменной `сообщение`, которая имеет тип `FB_msg` (структура) и содержит в себе:
    - `int32_t ID` - ID сообщения
    - `String usrID` - ID юзера
    - `String first_name` - имя юзера
    - `String username` - ник юзера
    - `String chatID` - ID чата
    - `String text` - текст сообщения
    - `bool query` - флаг запроса

### Минимальный пример
```cpp
void setup() {
  // подключаемся к WiFi
  bot.attach(newMsg);   // подключаем обработчик сообщений
}

void newMsg(FB_msg& msg) {
  // выводим имя юзера и текст сообщения
  Serial.print(msg.username);
  Serial.print(", ");
  Serial.println(msg.text);
}

void loop() {
  bot.tick();
}
```

### Форматирование меню
Для отправки меню используется строка с именами кнопок и специальным форматированием:
- `\t` - горизонтальное разделение кнопок
- `\n` - вертикальное разделение кнопок
- Лишние пробелы вырезаются автоматически

Пример меню 3x1: `"Menu1 \t Menu2 \t Menu3 \n Menu4"`

Результат:
```cpp
 _______________________
|       |       |       |
| Menu1 | Menu2 | Menu3 |
|_______|_______|_______|
|                       |
|       M e n u 4       |
|_______________________|
```

### Обычное меню
Большое меню в нижней части чата.
```cpp
showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Нажатие на кнопку **отправляет текст с кнопки**.

### Инлайн меню
Меню в сообщении. Требует ввода имени меню:
```cpp
inlineMenu("MyMenu", "Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Нажатие на кнопку **отправляет текст с кнопки**.

### Инлайн меню с коллбэком
Меню в сообщении. Коллбэк позволяет задать каждой кнопке уникальный текст, который будет отправляться боту вместо текста с кнопки. 
Список коллбэков перечисляется через запятую по порядку:
```cpp
String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
String cback1 = F("action1,action2,action3,back");
bot.inlineMenuCallback("Menu 1", menu1, cback1);
```
Нажатие на кнопку **отправляет заданный текст**.

### Ответ на коллбэк
При нажатии на кнопку инлайн-меню боту отправляется коллбэк, в обработчике сообщения будет поднят флаг `query`. 
Можно ответить на коллбэк при помощи:
- `answer(текст)` - всплывающий текст
- `answer(текст, true)` - окно с предупреждением и кнопкой ОК

Пример:
```cpp
void newMsg(FB_msg& msg) {
  if (msg.query) bot.answer("Hello!", true);
}
```

<a id="example"></a>
## Пример
```cpp

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
- v1.7:
  - Убрал динамический режим FB_DYNAMIC_HTTP, работает слишком медленно
  - Исправил warningи
  - Починил работу бота в "группах" (отрицательный ID чата)
  - Оптимизация памяти
  - Ускорил работу
  - Пофиксил работу через раз в сценарии "эхо"
- v2.0:
    - Убрал минимум в 3200 мс
    - Добавил обработку Юникода (русский язык, эмодзи). Спасибо Глебу Жукову!
    - Из меню удаляются лишние пробелы, работать стало проще
    - Поддержка esp32
    - Большая оптимизация
    - Добавил коллбэки в inlineMenu
    - Добавил ID юзера
    - Добавил редактирование сообщений и кучу всего

- v2.1: 
    - Ещё оптимизация
    - Добавил форматирование текста (markdown, html)
    - Добавил ответ на сообщение
- v2.2:
    - Большая оптимизация памяти и производительности
    - Добавил notify() - уведомления от сообщений бота
    - Добавил единоразовый показ клавиатуры

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
