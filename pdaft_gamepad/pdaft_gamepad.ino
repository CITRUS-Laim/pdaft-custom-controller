#include <GyverTimer.h>
#include <FastLED.h>
GTimer myTimer(MS);
CRGBArray<20> leds;
bool id[9];
//bool id2[9];
bool id3[3];
bool point2;
byte s = 0;
int LR[1];
int tic[1];
unsigned long time1 = 0;
#define bright 120
#define L1 7
#define L2 6
#define R1 5
#define R2 4
#define leddelay 0
#define colorcof 4
#define huedelay 33
const byte butstep = 2;
void setup() {
  myTimer.setInterval(huedelay);
  for (byte a = 18; a > 13; a--) pinMode(a, INPUT_PULLUP);
  for (byte a = 11; a > 7; a--) pinMode(a, INPUT_PULLUP);
  for (byte d = 2; d < 8; d++) {
    //pinMode(d, OUTPUT);
    digitalWrite(d, HIGH);
  }

  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, 12>(leds, 20);
}
void loop() {
  if (millis() - 7000 < time1) {
    output();
  } else {
    fadergb();
  }
}
void pointtosens(bool stat) {
  if(stat) idsens(true);
  else idsens(false);
  static  byte hue = 0;
  if (myTimer.isReady()) hue++;
  //Serial.print("pointtosens: ");
  for (int rus = 9; rus >= 0; rus--) {
    //Serial.println(k);
    if (id[rus] == true) {
      //if(k == 9) Serial.println(id[k]);
      s = rus * 2;
      if (stat) id[rus] = false;
      leds[s] = CHSV(hue, 255, bright);
      leds[s + 1] = CHSV(hue, 255, bright);
      //Serial.println(s);
    }
  }
  FastLED.show();
  FastLED.clear();
}
void idsens(bool stat) {
  //Serial.print("idsens: ");
  for (int stat = 1; stat >= 0; stat--) {
    if (stat == 1) {
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
    }
    if (stat == 0) {
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
    }
    for (byte a = 18; a > 13; a--) {
      if (digitalRead(a) == false) {
        id[a - 14 + 5 * stat] = true;

        //id2[a - 14 + 5 * stat] = true;
        //if(s == 9) Serial.println("true");
        time1 = millis();
        //Serial.println(s);
      }
    }
  }
  if(stat) ticchek(false);
}
void fadergb() {
  idsens(false);
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
  pointtosens(false);
  ticchek(true);
  for (byte i = 0; i < 10; i++) id[i] = false;
  pointtosens(true);
  if (LR[0] != tic[0]) {
    if (tic[0] < LR[0]) {
      digitalWrite(R1, LOW);
      Serial.println("R1 true");
    }
    else digitalWrite(R1, HIGH);
    if (tic[0] > LR[0]) {
      digitalWrite(L1, LOW);
      Serial.println("L1 true");
    }
    else digitalWrite(L1, HIGH);
  }
  if (LR[1] != tic[1] && point2 == true) {
    if (tic[1] < LR[1]) {
      digitalWrite(R2, LOW);
      Serial.println("R2 true");
    }
    else digitalWrite(R2, HIGH);
    if (tic[1] > LR[1]) {
      digitalWrite(L2, LOW);
      Serial.println("L2 true");
    }
    else digitalWrite(L2, HIGH);
    point2 = false;
  }
}
void ticchek(bool stat) {
  for (tic[0] = 0; tic[0] < 10; tic[0]++) {
    if (id[tic[0]]) {
      if (stat) LR[0] = tic[0];
      Serial.println(tic[0]);
      break;
    }
  }
  for (tic[1] = tic[0] + butstep; tic[1] < 10; tic[1]++) {
    point2 = true;
    if (id[tic[1]]) {
      if (stat) LR[1] = tic[1];
      Serial.println(tic[1]);
      break;
    }
  }
}
