
#include "pins.h"
#include "Arduino.h"
#include "pins.h"
#include "SomfyRTS.h"
#include "Syslog.h"
#include "WiFi.h"
#include "prefs.h"

SomfyRTS somfy(RF433_PIN);
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udpClient;
boolean syslog_working=false;
Syslog syslog(udpClient);

void write_output(String message){
  if(syslog_working){
    syslog.log(LOG_INFO, message);
  }
  Serial.println(message);
}

void setup_syslog(){
  boolean syslog_state = false;
  char syslog_ip[IP_LENGTH];
  int16_t syslog_port;
  syslog_state = prefs_get_syslog_state();
  prefs_get_syslog_ip(syslog_ip);
  syslog_port = prefs_get_syslog_port();

  IPAddress syslog_server_ip;
  if (syslog_state && syslog_server_ip.fromString(syslog_ip)) {
    Serial.println("MISC - Enabling syslog");
    syslog = Syslog(udpClient, syslog_server_ip, syslog_port, "SomfyRTS", "SomfyRTS", LOG_KERN);
    delay(500);
    syslog_working=true;
    write_output("MISC - setup_syslog done");
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