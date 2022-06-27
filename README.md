[![Foo](https://img.shields.io/badge/Version-2.17-brightgreen.svg?style=flat-square)](#versions)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/FastBot?_x_tr_sl=ru&_x_tr_tl=en) 
[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)


# FastBot
Очень простая и быстрая библиотека для телеграм бота на esp8266/esp32
- Работает на стандартных библиотеках без SSL
- Опциональный "белый список" ID чатов
- Проверка обновлений вручную или по таймеру
- Отправка/удаление/редактирование/ответ на сообщения
- Работает (чтение и отправка) в чатах, группах, каналах
- Изменение названия и описания чата
- Закрепление/открепление сообщений
- Отправка стикеров, отправка с форматированием markdown/html
- Вывод обычного и инлайн меню с поддержкой кнопок-ссылок
- Поддержка Unicode (другие языки + эмодзи) для входящих сообщений
- Встроенный urlencode для исходящих сообщений
- Встроенные часы реального времени с синхронизацией от сервера Telegram
- Возможность OTA обновления прошивки .bin файлом из чата Telegram

### Дополнительно
Используйте библиотеку [CharDisplay](https://github.com/GyverLibs/CharDisplay) для вывода графиков и рисования в чате!

### Совместимость
ESP8266 (SDK v2.6+), ESP32

## Документация и проекты
Подробные уроки по работе с Телеграм ботом при помощи этой библиотеки можно найти на [сайте Arduino набора GyverKIT](https://kit.alexgyver.ru/tutorials-category/telegram/)

## Сравнение с Universal-Arduino-Telegram-Bot
[Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)  

Для сравнения использовался минимальный пример с отправкой сообщения в чат и выводом входящих сообщений в сериал:
- **send** - отправка сообщения в чат
- **update** - проверка входящих сообщений
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
    - [Отправка сообщений](#send)
    - [Парсинг сообщений](#inbox)
    - [Тикер](#tick)
    - [Минимальный пример](#example)
    - [Обращение к сообщениям](#msgid)
    - [Отправка стикеров](#sticker)
    - [Меню](#menu)
    - [Обычное меню](#basic)
    - [Инлайн меню](#inline)
    - [Инлайн меню с коллбэком](#callb)
    - [Ответ на коллбэк](#answer)
    - [Модуль времени](#unix)
    - [Время получения сообщения](#time)
    - [Часы реального времени](#rtc)
    - [Обновление прошивки из чата](#ota)
    - [Оформление текста](#textmode)
    - [Всякие трюки](#tricks)
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
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!


<a id="init"></a>
## Инициализация
- [Инструкция как создать и настроить Telegram бота](https://kit.alexgyver.ru/tutorials/telegram-basic/)

```cpp
FastBot bot;
FastBot bot(токен); // с указанием токена
```

<a id="docs"></a>
## Документация
```cpp
// ============== НАСТРОЙКИ ==============
void setToken(String token);                    // изменить/задать токен бота
void setChatID(String chatID);                  // установка ID чата (белый список), необязательно. Можно несколько через запятую ("id1,id2,id3")
void setChatID(int64_t id);                     // то же самое, но в int64_t. Передай 0, чтобы отключить
void setPeriod(int period);                     // период опроса в мс (по умолч. 3500)
void setLimit(int limit);                       // кол-во сообщений, которое обрабатывается за один запрос, 1..100. (по умолч. 10)
void setBufferSizes(uint16_t rx, uint16_t tx);  // установить размеры буфера на приём и отправку, по умолч. 512 и 512 байт (только для esp8266)

void setTextMode(uint8_t mode);                 // режим текста "для отправки": FB_TEXT, FB_MARKDOWN, FB_HTML (см. пример textMode)
void notify(bool mode);                         // true/false вкл/выкл уведомления от сообщений бота (по умолч. вкл)
void clearServiceMessages(bool state);          // удалять из чата сервисные сообщения о смене названия и закреплении сообщений (умолч. false)


// =============== ПАРСИНГ ===============
void attach(callback);                          // подключение функции-обработчика сообщений
void detach();                                  // отключение обработчика сообщений


// ================ ТИКЕР ================
uint8_t tick();                                 // проверка обновлений по таймеру
uint8_t tickManual();                           // ручная проверка обновлений


// ============== СООБЩЕНИЯ ==============
// отправить сообщение в указанный в setChatID чат/чаты ИЛИ передать id чата
uint8_t sendMessage(String msg);
uint8_t sendMessage(String msg, String id);

// редактировать сообщение (msgid) в указанном в setChatID чате ИЛИ передать id чата
uint8_t editMessage(int32_t msgid, String text);
uint8_t editMessage(int32_t msgid, String text, String id);

// ответить на сообщение с id (replyID) в указанный в setChatID чат ИЛИ указать чат
uint8_t replyMessage(String msg, int32_t replyID);
uint8_t replyMessage(String msg, int32_t replyID, String id);

// отправить стикер в указанный в setChatID чат/чаты ИЛИ передать id чата
uint8_t sendSticker(String stickerID);
uint8_t sendSticker(String stickerID, String id);

// ответить на callback текстом (text) и режимом (alert): FB_NOTIF - уведомление в чате, FB_ALERT - окно с кнопкой ОК
uint8_t answer(String text, bool alert);


// =============== УДАЛЕНИЕ ===============
// удалить сообщение с id (msgid) в указанном в setChatID чате ИЛИ передать id чата
// удаляет любые типы сообщений (текст, стикер, инлайн меню)
uint8_t deleteMessage(int32_t msgid);
uint8_t deleteMessage(int32_t msgid, String id);


// ============= ОБЫЧНОЕ МЕНЮ =============
// показать меню (menu) в указанном в setChatID чате/чатах ИЛИ передать id чата/чатов 
uint8_t showMenu(String menu);
uint8_t showMenu(String menu, String id);

// единоразовое меню (закроется при выборе) в текущем чате ИЛИ передать id чата
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

// редактировать меню (msgid) текстом (menu) в указанном в setChatID чате ИЛИ передать id чата
uint8_t editMenu(int32_t msgid, String menu);
uint8_t editMenu(int32_t msgid, String menu, String id);


// ======= ИНЛАЙН МЕНЮ С КОЛЛБЭКОМ =======
// сообщение (msg) с инлайн меню (menu) и коллбэком (cbck) в указанном в setChatID чате/чатах ИЛИ передать id чата/чатов
uint8_t inlineMenuCallback(String msg, String menu, String cbck);
uint8_t inlineMenuCallback(String msg, String menu, String cbck, String id);

// редактировать меню (msgid) текстом (menu) и коллбэком (cback) в указанном в setChatID чате ИЛИ передать id чата
uint8_t editMenuCallback(int32_t msgid, String menu, String cback);
uint8_t editMenuCallback(int32_t msgid, String menu, String cback, String id);


// ============== ГРУППОВЫЕ ==============
// для всех групповых команд бот должен быть админом в чате!

// установить имя группы в указанном в setChatID чате ИЛИ передать id чата
uint8_t setChatTitle(String& title);
uint8_t setChatTitle(String& title, String& id);

// установить описание группы в указанном в setChatID чате ИЛИ передать id чата
uint8_t setChatDescription(String& description);
uint8_t setChatDescription(String& description, String& id);

// закрепить сообщение с ID msgid в указанном в setChatID чате ИЛИ передать id чата
uint8_t pinMessage(int32_t msgid);
uint8_t pinMessage(int32_t msgid, String& id);

// открепить сообщение с ID msgid в указанном в setChatID чате ИЛИ передать id чата
uint8_t unpinMessage(int32_t msgid);
uint8_t unpinMessage(int32_t msgid, String& id);

// открепить все сообщения в указанном в setChatID чате ИЛИ передать id чата
uint8_t unpinAll();
uint8_t unpinAll(String& id);


// ============= КОМАНДА API =============
// отправить команду API в указанном в setChatID чате ИЛИ передать id чата (id сам добавится в команду)
// (пример команды: "/sendSticker?sticker=123456")
uint8_t sendCommand(String& cmd);
uint8_t sendCommand(String& cmd, String& id);


// ================ СЕРВИС ===============
int32_t lastBotMsg();               // ID последнего отправленного ботом сообщения
int32_t lastUsrMsg();               // ID последнего отправленного юзером сообщения
String chatIDs;                     // указанная в setChatID строка, для отладки и редактирования списка

uint8_t sendRequest(String& req);   // отправить запрос (https://api.telegram.org/bot...)
void autoIncrement(boolean incr);   // авто инкремент сообщений (по умолч включен)
void incrementID(uint8_t val);      // вручную инкрементировать ID на val


// ============== СООБЩЕНИЕ ===============
// структура FB_msg
String& userID;         // ID юзера
String& username;       // ник (в API это first_name)
String& chatID;         // ID чата
int32_t messageID;      // ID сообщения
String& text;           // текст
String& data;           // callback дата
bool query;             // запрос
bool& edited;           // сообщение отредактировано
bool isBot;             // сообщение от бота
bool OTA;               // запрос на OTA обновление
uint32_t unix;          // время сообщения
String toString();      // вся информация одной строкой


// ================ ВРЕМЯ =================
FB_Time getTime(int16_t gmt);   // получить текущее время, указать часовой пояс (например Москва 3) в часах или минутах
bool timeSynced();              // проверка, синхронизировано ли время
uint32_t getUnix();             // получить текущее unix время

// структура FB_Time
uint8_t second;         // секунды
uint8_t minute;         // минуты
uint8_t hour;           // часы
uint8_t day;            // день месяца
uint8_t month;          // месяц
uint8_t dayWeek;        // день недели (пн..вс 1..7)
uint16_t year;          // год
String timeString();    // строка времени формата ЧЧ:ММ:СС
String dateString();    // строка даты формата ДД.ММ.ГГГГ


// =============== СТАТУС ================
// Многие функции возвращают статус:
// 0 - ожидание
// 1 - ОК
// 2 - Переполнен
// 3 - Ошибка телеграм
// 4 - Ошибка подключения
// 5 - не задан chat ID
// 6 - множественная отправка, статус неизвестен
// 7 - не подключен обработчик


// =============== УТИЛИТЫ ===============
void FB_unicode(String &s);                 // перевести unicode
void FB_urlencode(String& s, String& dest); // urlencode из s в dest

int64_t FB_str64(const String &s);  // перевод из String в int64_t
String FB_64str(int64_t id);        // перевод из int64_t в String


// ========== ДЕФАЙНЫ НАСТРОЕК ===========
// объявлять ПЕРЕД подключением библиотеки
#define FB_NO_UNICODE       // отключить конвертацию Unicode для входящих сообщений (чуть ускорит программу)
#define FB_NO_URLENCODE     // отключить конвертацию urlencode для исходящих сообщений (чуть ускорит программу)
#define FB_NO_OTA           // отключить поддержку OTA обновлений из чата
```

<a id="usage"></a>
## Использование

<a id="send"></a>
## Отправка сообщений
Для отправки в чат (сообщения, стикеры, меню и так далее) обязательно должен быть указан ID чата, в который будет осуществляться отправка. Можно указать 
несколько ID через запятую, в пределах одной строки. Есть два способа указать ID:
- Непосредственно в отправляющую функцию, у них у всех есть такой вариант (см. документацию выше)
```cpp
bot.sendMessage("Hello!", "123456");            // в один чат
bot.sendMessage("Hello!", "123456,7891011");    // в два чата
```
- Установить ID через `setChatID()` и все отправления пойдут в эти чат/чаты, если в отправляющей функции не указан другой ID
```cpp
bot.setChatID("123456");             // один чат
//bot.setChatID("123456,7891011");   // несколько чатов
// ...
bot.sendMessage("Hello!");           // уйдёт в "123456"
bot.sendMessage("Hello!", "112233"); // уйдёт в "112233"
```
> Примечание: Телеграм разделяет текст на несколько сообщений, если длина текста превышает ~4000 символов! Эти сообщения будут иметь разный messageID в чате.

### Лимиты
Телеграм устанавливает следующие лимиты на **отправку** сообщений ботом ([документация](https://core.telegram.org/bots/faq#my-bot-is-hitting-limits-how-do-i-avoid-this))
- В чат: не чаще раза в секунду. *Отправлять чаще можно, но сообщение может не дойти*
- В группу: не чаще 20 сообщений в минуту
- Суммарный лимит: не чаще 30 сообщений в секунду

Также бот может читать сообщения, с момента отправки которых прошло меньше 24 часов.

<a id="inbox"></a>
## Парсинг сообщений
Сообщения автоматически запрашиваются и читаются в `tick()`, при поступлении нового сообщения вызывается указанная функция-обработчик:
- Создаём в скетче свою функцию вида `void функция(FB_msg& сообщение)`
- Вызываем `attach(функция)`
- Эта функция будет автоматически вызвана при входящем сообщении, если ID чата совпадает или не настроен
- Если обработчик не подключен - сообщения не будут проверяться
- Внутри этой функции можно пользоваться переданной переменной `сообщение`, которая имеет тип `FB_msg` (структура) и содержит в себе:
    - `String userID` - ID пользователя
    - `String username` - имя пользователя или канала
    - `String chatID` - ID чата
    - `int32_t messageID` - ID сообщения в чате
    - `String text` - текст сообщения или попдпись к файлу
    - `String replyText` - текст ответа, если он есть
    - `String data` - callback данные из меню (если есть)
    - `bool query` - запрос
    - `bool edited` - сообщение отредактировано
    - `bool isBot` - сообщение от бота
    - `bool OTA` - запрос на OTA обновление (получен .bin файл)
    - `uint32_t unix` - время сообщения
    - `String fileName` - имя файла

А также `String toString()` - вся информация из сообщения, удобно для отладки (с версии 2.11)

**Примечания:**
- Телеграм разделяет текст на несколько сообщений, если длина текста превышает ~4000 символов! Эти сообщения будут иметь разный messageID в чате.
- При ответе на сообщение библиотека парсит текст исходного сообщения, а не ответа

### Белый список
В библиотеке реализован механизм белого списка: можно указать в `setChatID()` ID чата (или нескольких через запятую), сообщения из которого будут приниматься. 
Сообщения из остальных чатов будут игнорироваться.

<a id="tick"></a>
## Тикер
Для опроса входящих сообщений нужно подключить обработчик сообщений и вызывать `tick()` в главном цикле программы `loop()`, опрос происходит по встроенному таймеру. 
По умолчанию период опроса установлен 3600 миллисекунд.

Можно опрашивать чаще (сменить период через `setPeriod()`), но лично у меня с ~2021 года сервер Телеграм стал отвечать не 
раньше, чем через ~3 секунды. Если запрашивать обновления чаще этого периода, программа зависает внутри `tick()` (внутри GET запроса) 
в ожидании ответа сервера на остаток от 3 секунд. При периоде ~3600 мс этого не происходит, поэтому я сделал его по умолчанию. 
Возможно это зависит от провайдера или страны.

<a id="example"></a>
## Минимальный пример
```cpp
void setup() {
  // подключаемся к WiFi
  bot.attach(newMsg);   // подключаем обработчик сообщений
}

void newMsg(FB_msg& msg) {
  // выводим имя юзера и текст сообщения
  //Serial.print(msg.username);
  //Serial.print(", ");
  //Serial.println(msg.text);
  
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());
}

void loop() {
  bot.tick();
}
```

<a id="msgid"></a>
## Обращение к сообщениям
Для редактирования и удаления сообщений и меню, а также закрепления сообщений, нужно знать ID сообщения (его номер в чате):
- ID входящего сообщения приходит в обработчик входящих сообщений
- ID последнего принятого сообщения можно получить из `lastUsrMsg()`
- ID последнего отправленного ботом сообщения можно получить из `lastBotMsg()`

Будьте внимательны с ID чата, у всех чатов своя нумерация сообщений!

<a id="sticker"></a>
## Отправка стикеров
Для отправки стикера нужно знать ID стикера. Отправь нужный стикер боту *@idstickerbot*, он пришлёт ID стикера. 
Этот ID нужно передать в функцию `sendSticker()`.

<a id="menu"></a>
## Меню
> Примечание: для всех вариантов меню *не производится* url encode. Избегайте символов `#` и `&` или используйте уже закодированный url!

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

<a id="basic"></a>
## Обычное меню
Большое меню в нижней части чата.
```cpp
showMenu("Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Нажатие на кнопку отправляет текст с кнопки (поле сообщения `text`).

<a id="inline"></a>
## Инлайн меню
Меню в сообщении. Требует ввода имени меню.
```cpp
inlineMenu("MyMenu", "Menu1 \t Menu2 \t Menu3 \n Menu4");
```
Нажатие на кнопку отправляет имя меню (поле сообщения `text`) и текст с кнопки (поле сообщения `data`).

<a id="callb"></a>
## Инлайн меню с коллбэком
Меню в сообщении. Позволяет задать каждой кнопке уникальный текст, который будет отправляться ботом вместе с именем меню. 
Список коллбэков перечисляется через запятую по порядку кнопок меню:
```cpp
String menu1 = F("Menu 1 \t Menu 2 \t Menu 3 \n Back");
String cback1 = F("action1,action2,action3,back");
bot.inlineMenuCallback("Menu 1", menu1, cback1);
```
Нажатие на кнопку отправляет имя меню (поле сообщения `text`) и указанные данные (поле сообщения `data`).
- (С версии 2.11) если callback задан как http/https адрес, кнопка автоматически станет **кнопкой-ссылкой**

<a id="answer"></a>
## Ответ на коллбэк
При нажатии на кнопку инлайн-меню боту отправляется коллбэк, в обработчике сообщения будет поднят флаг `query`. Сервер Телеграм будет ждать ответа. 
Ответить на коллбэк можно при помощи:
- `answer(текст, FB_NOTIF)` - всплывающий текст-уведомление
- `answer(текст, FB_ALERT)` - окно с предупреждением и кнопкой ОК

Отвечать нужно **внутри обработчика сообщения**! Пример:
```cpp
void newMsg(FB_msg& msg) {
  if (msg.query) bot.answer("Hello!", true);
}
```

> Если ничего не отвечать, библиотека сама отправит пустой ответ и "таймер" на кнопке исчезнет.

<a id="unix"></a>
## Модуль времени
В библиотеке есть тип данных `FB_Time`, который является структурой с полями:
```cpp
uint8_t second;     // секунды
uint8_t minute;     // минуты
uint8_t hour;       // часы
uint8_t day;        // день месяца
uint8_t month;      // месяц
uint8_t dayWeek;    // день недели (пн..вс 1..7)
uint16_t year;      // год
```

При создании структуры можно указать unix время и часовой пояс в часах или минутах (например 3 часа ИЛИ 180 минут для Москвы (UTC+3:00), 
330 минут для Индии (UTC+5:30)). После этого можно забирать нужные значения времени:

```cpp
FB_Time t(1651694501, 3);
Serial.print(t.hour);
Serial.print(':');
Serial.print(t.minute);
Serial.print(':');
Serial.print(t.second);
Serial.print(' ');
Serial.print(t.day);
Serial.print(':');
Serial.print(t.month);
Serial.print(':');
Serial.println(t.year);
```

С версии 2.9 библиотека умеет выводить форматированное время (String):
```cpp
Serial.print(t.timeString());   // ЧЧ:ММ:СС
Serial.print(' ');
Serial.println(t.dateString()); // ДД.ММ.ГГГГ
```

<a id="time"></a>
## Время получения сообщения
В обработчике входящих сообщений у структуры `FB_msg` есть поле `unix`, оно хранит время сообщения в unix формате. 
Для перевода в более читаемый формат действуем по описанной выше схеме:
```cpp
void newMsg(FB_msg& msg) {
  FB_Time t(msg.unix, 3);   // передали unix и часовой пояс
  Serial.print(t.timeString());
  Serial.print(' ');
  Serial.println(t.dateString());
}
```

<a id="rtc"></a>
## Часы реального времени
В ответ на любое сообщение от бота сервер сообщает время отправки в формате unix. С версии 2.6 это время парсится 
библиотекой и **счёт продолжается дальше** при помощи стандартных функций времени. Таким образом достаточно один раз отправить 
сообщение после включения платы, чтобы библиотека синхронизировала часы. При дальнейших отправках время также будет синхронизироваться 
и уточняться, т.к. вычисляемое средствами esp время будет уходить (~2 секунды в сутки). Инструменты:

- `uint32_t getUnix()` - вернёт текущее время в unix формате или `0`, если время не синхронизировано.
- `bool timeSynced()` - вернёт `true` если часы синхронизированы. 
- `FB_Time getTime(gmt)` - нужно передать свой часовой пояс, она вернёт `FB_Time`.

Таким образом получить время можно двумя способами (см. пример timeTest):
```cpp
FB_Time t = bot.getTime(3);
// или
FB_Time t(bot.getUnix(), 3);
```

<a id="ota"></a>
## Обновление прошивки из чата
С версии библиотеки 2.13 появилось обновление прошивки "по воздуху" (OTA) через чат. Для обновления нужно:
- Скомпилировать программу в файл: *Arduino IDE/Скетч/Экспорт бинарного файла* (файл **.bin** появится в папке со скетчем)
- Отправить файл в чат с ботом, можно добавить подпись
- Файл будет обработан как обычное входящее сообщение от пользователя
    - Подпись к файлу можно получить из поля `text`
    - Будет поднят флаг `OTA`
- Для запуска процесса обновления нужно вызвать `update` внутри обработчика сообщений
- В тот же чат чат будет отправлен статус обновления (*OK* или *error*)
- После успешного обновления esp перезагрузится

```cpp
// обновить, если просто прислали bin файл
if (msg.OTA) bot.update();

// обновить, если файл имеет нужную подпись
if (msg.OTA && msg.text == "update") bot.update();

// обновить, если прислал известный человек (админ)
if (msg.OTA && msg.chatID == "123456") bot.update();
```

<a id="textmode"></a>
## Оформление текста
Библиотека поддерживает оформление текста в сообщениях. Разметка оформления выбирается при помощи `setTextMode(mode)`, где `mode`:
- `FB_TEXT` - по умолчанию (оформление отключено)
- `FB_MARKDOWN`	- разметка Markdown v2
- `FB_HTML`	- разметка HTML

Доступные теги описаны в [API Telegram](https://core.telegram.org/bots/api#formatting-options). Например для Markdown:
```cpp
bot.setTextMode(FB_MARKDOWN);
bot.sendMessage(F("*Bold*, ~Strike~, `code`, [alexgyver.ru](https://alexgyver.ru/)"));
```

Выведет в чат: **Bold**, ~~Strike~~, `code`, [alexgyver.ru](https://alexgyver.ru/)

> **Внимание!** В режиме FB_MARKDOWN нельзя использовать в сообщениях символы `! + #`, сообщение не отправится. Возможно получится исправить в будущем (проблема urlencode и экранирования зарезервированных символов).


<a id="tricks"></a>
## Трюки
### Перезагрузка
Сообщения отмечаются прочитанными при следующем (относительно текущего обработчика сообщений) обновлении в tick(), то есть спустя как минимум настроенный тайм-аут. 
Если хочется перезагрузить esp по команде, то вот такая конструкция
```cpp
void message(FB_msg &msg) {
  if (msg.text == "restart") ESP.restart();
}
```
Приведёт к бутлупу (бесконечной перезагрузке), потому что сообщение не отметится прочитанным. Можно поднять флаг, по которому уходить в перезагрузку, предварительно вызвав tickManual:
```cpp
bool res = 0;
void message(FB_msg &msg) {
  if (msg.text == "restart") res = 1;
}
void loop() {
  if (res) {
    bot.tickManual();
    ESP.restart();
  }
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
    
- v2.3: Небольшая оптимизация
- v2.4: Добавил url encode для текста сообщений
- v2.5: Добавил флаги в FB_msg: сообщение отредактировано и сообщение отправлено ботом. Улучшил парсинг текста
- v2.6: Добавил встроенные часы реального времени
- v2.7: Добавил отправку стикеров
- v2.8: Убрал лишний вывод в сериал, GMT можно в минутах
- v2.9: Исправлена бага в парсинге, парсинг ускорен, добавлен вывод форматированного времени, добавлена фамилия и время сообщения
- v2.10: Добавлены функции для изменения названия и описания чата, закрепления и открепления сообщений. Убраны edit/deleteMessageID, editMenuID
- v2.11: 
    - Оптимизация, исправление багов
    - Callback data теперь парсится отдельно в data
    - Переделана работа с callback
    - Добавлен toString() для FB_msg для отладки
    - В callback добавлена обработка url адресов
    - Убраны first_name и last_name (с сохранением легаси)
    - usrID и ID переименованы в userID и messageID (с сохранением легаси)
    - Окончательно убран старый обработчик входящих сообщений

- v2.12: поправлены примеры, исправлен парсинг isBot, переделан механизм защиты от длинных сообщений, переделана инициализация
- v2.13: Оптимизация памяти. Добавил OTA обновление
- v2.14: Улучшен парсинг строки с ID, добавил отключение OTA, добавил парсинг названия группы/канала в username
- v2.15: Заплатка для кривой библиотеки ESP32
- v2.16: добавлен вывод fileName, пофикшены неотправляемые сообщения в Markdown режиме
- v2.17: вывод текста сообщения, на которое ответил юзер + корректная работа с menu в группах

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код