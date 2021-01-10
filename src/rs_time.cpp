#include "WiFi.h"
#include "time.h"
#include "sys/time.h"
#include "prefs.h"


char ntpServer[NTP_SERVER_LENGTH];
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
bool time_set = false;


bool inittime(void) {
  prefs_get_ntp_server(ntpServer);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return false;
  }

  Serial.print("TIME - gettime - hour : ");
  Serial.println(timeinfo.tm_hour);
  Serial.print("TIME - gettime - minutes : ");
  Serial.println(timeinfo.tm_min);
  return true;
}


bool gettime(uint8_t * hours, uint8_t * minutes) {

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return false;
  }

  /*Serial.print("TIME - gettime - hour    : ");
  Serial.println(timeinfo.tm_hour);
  Serial.print("TIME - gettime - minutes : ");
  Serial.println(timeinfo.tm_min);*/

  *hours = timeinfo.tm_hour;
  *minutes = timeinfo.tm_min;

  return true;
  
}


bool settime(uint8_t hours, uint8_t minutes) {

  struct tm tm;
  tm.tm_year = 2017 - 1900;
  tm.tm_mon = 11;
  tm.tm_mday = 8;
  tm.tm_sec = 10;
  tm.tm_hour = hours + 1;
  tm.tm_min = minutes;
  tm.tm_sec = 0;
  time_t t = mktime(&tm);
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);

  time_set = true;

  return true;
  
}
