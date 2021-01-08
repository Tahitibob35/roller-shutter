#ifndef PREFS_H_
#define PREFS_H_

#include "Arduino.h"

#include <Preferences.h>

#define KEY_LENGTH 10
#define IP_LENGTH 16
#define NTP_SERVER_LENGTH 20
#define ACCESSPOINT_LENGTH 20
#define PASSWORD_LENGTH 20

void prefs_set_key(String key);
void prefs_get_key(char * key);
void prefs_set_token(String token);
void prefs_get_token(char * token);
void prefs_set_syslog_state(boolean syslog_state);
boolean prefs_get_syslog_state();
void prefs_get_syslog_ip(char * syslog_ip);
void prefs_set_syslog_ip(String syslog_ip);
uint16_t prefs_get_syslog_port();
void prefs_set_syslog_port(int syslog_port);
void prefs_set_ntp_server(String ntp_server);
void prefs_get_ntp_server(char * ntp_server);
void prefs_set_accesspoint(String accesspoint);
void prefs_get_accesspoint(char * accesspoint);
void prefs_set_password(String password);
void prefs_get_password(char * password);
uint16_t prefs_get_prgmcount();
void prefs_set_prgmcount(int prgmcount);
void prefs_loadprgms(void);
void prefs_storeprgms(void);
#endif //PREFS_H_
