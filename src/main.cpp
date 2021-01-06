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
#include "TaskScheduler.h"

int rescue_mode = 0;
const long interval = 2000;
unsigned long previousMillis = 0;
uint8_t previous_hour = 0;
uint8_t previous_minute = 0;
extern bool time_set;
bool wifi_connected;

// Scheduler variables
#define _TASK_SLEEP_ON_IDLE_RUN
// Timer/Scheduler
Scheduler runner;
// List tasks
void checkTriggerProgam();
void wifiLEDOn();
void wifiLEDOff();
// Check if a program starts (checked every minute)
Task programTask(TASK_MINUTE, TASK_FOREVER, &checkTriggerProgam, &runner, false);
// Check Wifi (every 5mn)
Task checkWifiTask(TASK_MINUTE, TASK_FOREVER, &check_wifi, &runner, false);
// Do Wifi LED blink (every second)
Task wifiBlinkTask(1 * TASK_SECOND, TASK_FOREVER, &wifiLEDOn, &runner, false);

void checkTriggerProgam(){
  uint8_t hour=0;
  uint8_t minute=0;
  
  gettime(&hour, &minute);
  checkprgms(hour, minute);
}

void wifiLEDOn(void) {
  digitalWrite(STATUS_LED_PIN, HIGH);
  wifiBlinkTask.setCallback(&wifiLEDOff);
}

void wifiLEDOff(void) {
  digitalWrite(STATUS_LED_PIN, LOW);
  wifiBlinkTask.setCallback(&wifiLEDOn);
}

void setup() {
  wifi_connected =  false;
  
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
  digitalWrite(TX_LED_PIN, HIGH);

  rescue_mode = digitalRead(RESCUE_PIN);
  
  if (rescue_mode) {
    start_softap();
    wifiBlinkTask.enable();   
  }
  else {
    connect_to_wifi();
    setup_syslog();
    checkWifiTask.enable();
    if (inittime()) {
      digitalWrite(STATUS_LED_PIN, LOW);
      time_set = true;
    }
  }
  
  ws_config(rescue_mode);
  
  prefs_loadprgms();
  if(prefs_get_prgmcount() > 0){
    programTask.enable();
  } 

  digitalWrite(TX_LED_PIN, LOW);

  write_output("End setup function");
}

void loop() {
  // Handle web requests
  ws_handle_client();

  if (rescue_mode) {
    handle_dns_requests();
  }else
  {
    if(!time_set) {
      write_output("Main.Loop - Time not set, trying to get it...");
      if (inittime()) {
        digitalWrite(STATUS_LED_PIN, LOW);
        write_output("Main.Loop - Time set");
        time_set = true;
      }
    }
  }
  // Task scheduler
  runner.execute();  
}
