
#include "Arduino.h"
#include "TaskScheduler.h"
#include "prgm.h"
#include "pins.h"
#include "rs_scheduledtasks.h"
#include "rs_time.h"
#include "rs_wifi.h"
#include "misc.h"

// Timer/Scheduler
Scheduler runner;
// Check if a program starts (checked every minute)
Task programTask(TASK_MINUTE, TASK_FOREVER, &checkTriggerProgram, &runner, false);
// Check Wifi (every 5mn)
Task checkWifiTask(TASK_MINUTE, TASK_FOREVER, &check_wifi, &runner, false);
// Do Wifi LED blink (every second)
Task wifiBlinkTask(1 * TASK_SECOND, TASK_FOREVER, &wifiLEDOn, &runner, false);

void checkTriggerProgram(){

  Serial.println("Main - checkTriggerProgram - Start");

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

void enable_wifiBlinkTask(void) {
  wifiBlinkTask.enable();
}

void enable_checkWifiTask(void) {
  checkWifiTask.enable();
}

void refresh_programTask(void) {
  Serial.print("rs_scheduledtasks - refresh_programTask - getprgmcount : ");
  Serial.println(getprgmcount());
  if(getprgmcount() > 0){
    write_output("rs_scheduledtasks - refresh_programTask - Enabling programTask");
    programTask.enable();
  }
  else {
    write_output("rs_scheduledtasks - refresh_programTask - Disabling programTask");
    programTask.disable();
  }
  
}

void execute_runner(void) {
  runner.execute();
}

