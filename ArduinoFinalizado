#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// ATENÇÃO AQUI: Mudamos de _F_ para _1_ para economizar quase 1KB de RAM!
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define T 11
#define E 12
#define B 8
#define L 13

unsigned long t = 0;
bool s = 0;

void setup() {
  pinMode(T, OUTPUT);
  pinMode(E, INPUT);
  pinMode(B, OUTPUT);
  pinMode(L, OUTPUT);
  u8g2.begin();
}

void loop() {
  digitalWrite(T, HIGH);
  delayMicroseconds(10);
  digitalWrite(T, LOW);
  
  unsigned long p = pulseIn(E, HIGH, 25000);
  float d = (p > 0) ? (p / 58.82) : 0;
  float n = (d > 0) ? (12.5 - d) : 0;
  if (n < 0) n = 0;
  if (n > 12.5) n = 12.5;
  
  byte pr = (n / 12.5) * 100;
  int i = 0;
  
  if (pr >= 85) i = 80;
  else if (pr >= 70) i = 250;
  else if (pr >= 50) i = 500;

  if (i > 0) {
    if (millis() - t >= (unsigned int)i) {
      t = millis();
      s = !s;
      digitalWrite(L, s);
      digitalWrite(B, s);
    }
  } else {
    digitalWrite(L, LOW);
    digitalWrite(B, LOW);
    s = 0;
  }

  // O loop do firstPage/nextPage desenha a tela em pedaços usando quase zero de RAM
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.setCursor(0, 20);
    u8g2.print(F("N: "));
    u8g2.print(n, 1);
    u8g2.print(F("cm"));

    u8g2.setCursor(0, 45);
    u8g2.print(F("P: "));
    u8g2.print(pr);
    u8g2.print(F("%"));
  } while (u8g2.nextPage());

  delay(100);
}
