#include "WiFi.h"

#include "rs_wifi.h"
#include "rs_webserver.h"
#include "prefs.h"
#include "rs_time.h"
#include "pins.h"
#include "misc.h"
#include "esp32-hal-cpu.h"
#include "rs_scheduledtasks.h"
#include "prgm.h"

int rescue_mode = 0;
const long interval = 2000;
unsigned long previousMillis = 0;
uint8_t previous_hour = 0;
uint8_t previous_minute = 0;
extern bool time_set;
bool wifi_connected;

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
    enable_wifiBlinkTask();   
  }
  else {
    connect_to_wifi();
    setup_syslog();
    enable_checkWifiTask();
    if (inittime()) {
      digitalWrite(STATUS_LED_PIN, LOW);
      time_set = true;
    }
  }
  
  ws_config(rescue_mode);
  
  prefs_loadprgms();
  refresh_programTask();

  digitalWrite(TX_LED_PIN, LOW);

  write_output("End setup function");
}

void loop() {

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
  execute_runner();  
}
