#include "WiFi.h"

#include "vars.h"
#include "rs_wifi.h"
#include "rs_webserver.h"
#include "prgm.h"
#include "prefs.h"
#include "rs_time.h"
#include "pins.h"
#include "misc.h"
#include "esp32-hal-cpu.h"

int rescue_mode = 0;
const long interval = 2000;
unsigned long previousMillis = 0;
uint8_t previous_hour = 0;
uint8_t previous_minute = 0;
extern bool time_set;

void setup() {
  
  Serial.begin(115200);
  Serial.println("\n\n\n** Boot in progress....");

  setCpuFrequencyMhz(80);
  int cpuSpeed = getCpuFrequencyMhz();
  Serial.print("CPU Frequency :");
  Serial.println(cpuSpeed);

  pinMode(RESCUE_PIN, INPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(TX_LED_PIN, OUTPUT);

  digitalWrite(STATUS_LED_PIN, HIGH);

  rescue_mode = digitalRead(RESCUE_PIN);
  rescue_mode = 1;
  Serial.println(rescue_mode);
  if (rescue_mode) {
    start_softap();    
  }
  else {
    connect_to_wifi();
    inittime();
    if (gettime(&previous_hour, &previous_minute)) {
      digitalWrite(STATUS_LED_PIN, LOW);
      time_set = true;
    }
  }
  
  ws_config(rescue_mode);
  
  prefs_loadprgms();
  
}

void loop() {
  
  uint8_t hour=0;
  uint8_t minute=0;

  // Handle web requests
  ws_handle_client();

  //Cause a reboot in softAP mode
  if (rescue_mode) {
    handle_dns_requests();
  }

  if (rescue_mode) {
    wifiblink();
  }
  else {

    if (time_set) {
  
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (gettime(&hour, &minute)) {
          if (minute != previous_minute) {
            //Serial.println("LOOP - Minute changed");
            checkprgms(hour, minute);
            previous_hour = hour;
            previous_minute = minute;
          }      
        }
      }
    }
  }


  
  
}
