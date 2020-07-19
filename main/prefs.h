#ifndef PREFS_H_
#define PREFS_H_

#include "Arduino.h"

#include <Preferences.h>

#define KEY_LENGTH 10
#define ACCESSPOINT_LENGTH 20
#define PASSWORD_LENGTH 20

void prefs_set_key(String key);
void prefs_get_key(char * key);
void prefs_set_accesspoint(String accesspoint);
void prefs_get_accesspoint(char * accesspoint);
void prefs_set_password(String accesspoint);
void prefs_get_password(char * accesspoint);
uint16_t prefs_get_prgmcount();
void prefs_set_prgmcount(int prgmcount);
void prefs_loadprgms(void);
void prefs_storeprgms(void);
#endif //PREFS_H_
