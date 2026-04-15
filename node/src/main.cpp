#include "Arduino.h"
#include <LD2450.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

const int ledPin = LED_BUILTIN;

// SENSOR INSTANCE
LD2450 ld2450;

void setup() {
  Serial.begin(115200);
  Serial.println("SETUP_STARTED");

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  ld2450.begin(Serial1, false);

  Serial.println("SETUP_FINISHED");
}

void sendTargetJSON(int id, const LD2450::RadarTarget &t) {
  Serial.print("{\"id\":");
  Serial.print(id);

  Serial.print(",\"x\":");
  Serial.print(t.x);

  Serial.print(",\"y\":");
  Serial.print(t.y);

  Serial.print(",\"speed\":");
  Serial.print(t.speed);

  Serial.print(",\"distance\":");
  Serial.print(t.distance);

  Serial.print(",\"valid\":");
  Serial.print(t.valid ? 1 : 0);

  Serial.println("}");
}

void loop() {
  if (ld2450.read() > 0) {
    Serial.println("\\TJ");
    for (int i = 0; i < ld2450.getSensorSupportedTargetCount(); i++)
      {
	sendTargetJSON(i, ld2450.getTarget(i));
      }
  }
}
