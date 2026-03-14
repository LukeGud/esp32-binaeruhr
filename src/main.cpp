#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

//Wifi
const char* name = "BLLLM";
const char* password = "Schanzenshop100";

//Pins
int latch = 16;
int clockPin = 17;
int dataSerial = 18;
int powerPin = 19;

//Debounce + Intervalle
unsigned long letzteLichtZeit = 0;
unsigned long letzteTasterZeit = 0;
unsigned long letzteUhrzeit = 0;
int lichtIntervall = 200;
int tasterIntervall = 30;

//Variablen
bool speicher = false;
bool anSignal = true;
int counter = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(name, password);
  pinMode(latch, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataSerial, OUTPUT);
  pinMode(powerPin, INPUT_PULLUP);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WLAN verbunden");
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");
}

void loop() {
  unsigned long jetzt = millis();
  bool tasterGedrueckt = !digitalRead(powerPin);

  if (jetzt - letzteUhrzeit > 1000) {
    struct tm zeitinfo;
    letzteUhrzeit = jetzt;

    if (!getLocalTime(&zeitinfo)) {
        Serial.println("Zeit konnte nicht geladen werden");
    } else {
    Serial.printf("%02d:%02d:%02d\n", zeitinfo.tm_hour, zeitinfo.tm_min, zeitinfo.tm_sec);
    }
  }

  if ((jetzt - letzteTasterZeit > tasterIntervall) && tasterGedrueckt != speicher) {
    letzteTasterZeit = jetzt;
    speicher = tasterGedrueckt;

    if (tasterGedrueckt)
    {
      anSignal = !anSignal;
    }
  }

  if (anSignal && (jetzt - letzteLichtZeit > lichtIntervall)) {
    digitalWrite(latch, LOW);
    shiftOut(dataSerial, clockPin, MSBFIRST, 1 << counter);
    digitalWrite(latch, HIGH);

    counter = (counter + 1) % 8;
    letzteLichtZeit = jetzt;
  }

  if (!anSignal) {
    digitalWrite(latch, LOW);
    shiftOut(dataSerial, clockPin, MSBFIRST, 0);
    digitalWrite(latch, HIGH);
  }

}
