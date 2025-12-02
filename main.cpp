#include <Arduino.h>

#define WIFI_SSID "Ekaterina"
#define WIFI_PASS "go4f-wbu0-q446"
#define BOT_TOKEN "8382770791:AAG2RZBnU_hhXfM1_VLYaoh3aysIzOke31"
#define CHAT_ID "99172834"

#include <FastBot2.h>
FastBot2 bot;
float voltage = 0.0;
void updateh(fb::Update &u)
{
  Serial.println("NEW MESSAGE");
  Serial.println(u.message().from().username());
  Serial.println(u.message().text());

  // #1
  // отправить обратно в чат (эхо)
  bot.sendMessage(fb::Message(u.message().text(), u.message().chat().id()));

  // #2
  // String text = u.message().text();
  // text += " - ответ";
  // bot.sendMessage(fb::Message(text, u.message().chat().id()));

  // #3
  // fb::Message msg;
  // msg.text = u.message().text();
  // msg.text += " - ответ";
  // msg.chatID = u.message().chat().id();
  // bot.sendMessage(msg);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(A0, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");

  // ============
  bot.attachUpdate(updateh);  // подключить обработчик обновлений
  bot.setToken(F(BOT_TOKEN)); // установить токен

  // режим опроса обновлений. Самый быстрый - Long
  // особенности читай тут в самом низу
  // https://github.com/GyverLibs/FastBot2/blob/main/docs/3.start.md

  // bot.setPollMode(fb::Poll::Sync, 4000);  // умолч
  // bot.setPollMode(fb::Poll::Async, 4000);
  bot.setPollMode(fb::Poll::Long, 60000);

  // поприветствуем админа
  bot.sendMessage(fb::Message("Hello!", CHAT_ID));
}

void loop()
{
  // вызывать тикер в loop
  bot.tick();
  delay(1000);
  // long map(long value, long fromLow, long fromHigh, long toLow, long toHigh)
  // Serial.println(analogRead(A0));

  voltage = map(analogRead(A0), 0, 1023, 0, 50);
  Serial.println(voltage/10.0);
  if ((voltage/10.0) <=2){
      bot.sendMessage(fb::Message("Низкий уровень напряжения -  прорыв", CHAT_ID));
  }
}