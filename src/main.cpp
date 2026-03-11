#include <Arduino.h>

int latch = 16;
int clockPin = 17;
int dataSerial = 18;
int powerPin = 19;
bool speicher = false;
bool anSignal = true;

void setup() {
  pinMode(latch, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataSerial, OUTPUT);
  pinMode(powerPin, INPUT_PULLUP);
}

void loop() {
  if(anSignal == true) {
    for (int i= 0; i < 8; i++) {
    digitalWrite(latch, LOW);
    shiftOut(dataSerial, clockPin, MSBFIRST, 1 << i);
    digitalWrite(latch, HIGH);
    delay(200);
    }
  }

  if(!digitalRead(powerPin) && !speicher) {
    anSignal = !anSignal;
  }
  speicher = !digitalRead(powerPin);
  delay(50);


}
