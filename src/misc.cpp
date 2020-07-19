
#include "pins.h"
#include "Arduino.h"
#include "pins.h"
#include "SomfyRTS.h"


SomfyRTS somfy(RF433_PIN);

void wifiblink(void) {

  const long interval = 500;
  static unsigned long previousMillis = 0;
  static int ledState = LOW;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(STATUS_LED_PIN, ledState);
  }

}


void moveup(int roller) {

  digitalWrite(TX_LED_PIN, HIGH);
  somfy.moveup(roller);
  delay(500);
  digitalWrite(TX_LED_PIN, LOW);

}


void movedown(int roller) {

  digitalWrite(TX_LED_PIN, HIGH);
  somfy.movedown(roller);
  delay(500);
  digitalWrite(TX_LED_PIN, LOW);
  
}


void prog(int roller) {

  digitalWrite(TX_LED_PIN, HIGH);
  somfy.prog(roller);
  delay(500);
  digitalWrite(TX_LED_PIN, LOW);
  
}


void stop(int roller) {

  digitalWrite(TX_LED_PIN, HIGH);
  somfy.stop(roller);
  delay(500);
  digitalWrite(TX_LED_PIN, LOW);
  
}