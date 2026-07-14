#include <Arduino.h>
#include "config.h"
#include <FastBot2.h>

FastBot2 bot;
float voltage = 0.0;

void updateh(fb::Update &u)
{
  Serial.println("NEW MESSAGE");
  Serial.println(u.message().from().username());
  Serial.println(u.message().text());

  bot.sendMessage(fb::Message(u.message().text(), u.message().chat().id()));
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

  bot.attachUpdate(updateh);
  bot.setToken(F(BOT_TOKEN));
  bot.setPollMode(fb::Poll::Long, 60000);
  bot.sendMessage(fb::Message("Hello!", CHAT_ID));
}

void loop()
{
  bot.tick();
  delay(1000);

  voltage = map(analogRead(A0), 0, 1023, 0, 50);
  Serial.println(voltage / 10.0);

  if ((voltage / 10.0) <= 2)
  {
    bot.sendMessage(fb::Message("Низкий уровень напряжения - прорыв", CHAT_ID));
  }
}
