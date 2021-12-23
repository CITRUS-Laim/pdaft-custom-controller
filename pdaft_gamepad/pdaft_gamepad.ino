#include <Wire.h>
#include <Adafruit_MPR121.h>
#include <GyverTimer.h>
#include <FastLED.h>
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
int LR[] = {0, 0, 0, 0};
GTimer myTimer(MS);
Adafruit_MPR121 cap = Adafruit_MPR121();
CRGBArray<20> leds;
bool id[9];
//bool id2[9];
bool id3[3];
bool point2;
byte s = 0;
int bruh;
int bruh2;
unsigned long time1 = 0;
unsigned long time2[3];
#define bright 120
#define L1 7
#define L2 6
#define R1 5
#define R2 4
#define leddelay 0
#define colorcof 4
#define huedelay 33
#define butstep 1
void setup() {
  cap.begin(0x5A);
  myTimer.setInterval(huedelay);
  for (byte a = 11; a > 7; a--) pinMode(a, INPUT_PULLUP);
  for (byte d = 2; d < 8; d++) {
    pinMode(d, OUTPUT);
    digitalWrite(d, HIGH);
  }
  for (byte a = 0; a < 10; a++) id[a] = false;
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 13>(leds, 20);
}
void loop() {
  if (millis() - 7000 < time1) {
    output();
  } else {
    fadergb();
  }
}
void pointtosens() {
  idsens();
  static  byte hue = 0;
  if (myTimer.isReady()) hue++;
  leds[18] = CHSV(0, 0, 0);
  leds[19] = CHSV(0, 0, 0);
  for (int rus = 9; rus >= 0; rus--) {
    //Serial.println(k);
    if (id[rus]) {
      s = rus * 2;
      leds[s] = CHSV(hue, 255, bright);
      leds[s + 1] = CHSV(hue, 255, bright);
    }
  }
  FastLED.show();
  FastLED.clear();
}
void idsens() {
  currtouched = cap.touched();
  for (byte a = 0; a < 12; a++) {
    if ((currtouched & _BV(a)) && !(lasttouched & _BV(a)) ) {
      id[a] = true;
      time1 = millis();
    }
    if (!(currtouched & _BV(a)) && (lasttouched & _BV(a)) ) {
      id[a] = false;
    }
  }
  lasttouched = currtouched;
}
void fadergb() {
  for (byte a = 8; a < 12; a ++) {
    if (digitalRead(a) == false) id3[a - 8] = true;
  }
  for (byte k = 0; k < 4; k++) {
    if (id3[0]) leds[k] = CHSV(0 + k * colorcof, 255, bright);
    if (id3[1]) {
      leds[k + 7] = CHSV(160 + k * colorcof, 255, bright);
      leds[6 - k] = CHSV(160 + k * colorcof, 255, bright);
    }
    if (id3[2]) {
      leds[k + 13] = CHSV(64 + k * colorcof, 255, bright);
      leds[12 - k] = CHSV(64 + k * colorcof, 255, bright);
    }
    if (id3[3]) leds[19 - k] = CHSV(96 + k * colorcof, 255, bright);
    FastLED.delay(leddelay);
  }
  FastLED.show();
  FastLED.clear();
  for (byte a = 0; a < 4; a++) id3[a] = false;
}
void output() {
  pointtosens();
  for (bruh = 0; bruh < 10; bruh++) {
    if (id[bruh]) {
      LR[0] = bruh;
      break;
    }
  }
  for (bruh2 = bruh + butstep; bruh2 < 10; bruh2++) {
    if (id[bruh2]) {
      LR[1] = bruh2;
      break;
    }
  }
  pointtosens();
  for (bruh = 0; bruh < 10; bruh++) {
    if (id[bruh]) {
      LR[2] = bruh;
      break;
    }
  }
  for (bruh2 = bruh + butstep; bruh2 < 10; bruh2++) {
    if (id[bruh2]) {
      LR[3] = bruh2;
      break;
    }
  }
  if (LR[2] > LR[0]) {
    time2[0] = millis();
    digitalWrite(L1, LOW);
  }
  if (LR[3] > LR[1]) {
    time2[1] = millis();
    digitalWrite(L2, LOW);
  }
  if (LR[2] < LR[0]) {
    time2[2] = millis();
    digitalWrite(R1, LOW);
  }
  if (LR[3] < LR[1]) {
    time2[3] = millis();
    digitalWrite(R2, LOW);
  }
  for (int a = 0; a < 4; a++) {
    if (millis() - 50 >= time2[a]) digitalWrite(7 - a, HIGH);
  }
}
