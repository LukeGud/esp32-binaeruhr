#include <Arduino.h>

int latch = 16;
int clockPin = 17;
int dataSerial = 18;
int powerPin = 19;

unsigned long letzteLichtZeit = 0;
unsigned long letzteTasterZeit = 0;
int lichtIntervall = 200;
int tasterIntervall = 30;

bool speicher = false;
bool anSignal = true;
int counter = 0;

void setup() {
  pinMode(latch, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataSerial, OUTPUT);
  pinMode(powerPin, INPUT_PULLUP);
}

void loop() {
  unsigned long jetzt = millis();

  bool tasterGedrueckt = !digitalRead(powerPin);

  if(tasterGedrueckt && !speicher && (jetzt - letzteTasterZeit > tasterIntervall)) {
    anSignal = !anSignal;
    letzteTasterZeit = millis();
    speicher = tasterGedrueckt;
  }
  

  if(anSignal && (jetzt - letzteLichtZeit > lichtIntervall)) {
    digitalWrite(latch, LOW);
    shiftOut(dataSerial, clockPin, MSBFIRST, 1 << counter);
    digitalWrite(latch, HIGH);
    counter = (counter + 1) % 8;
    letzteLichtZeit = millis();
  }

}
