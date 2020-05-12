#include "prefs.h"
#include "prgm.h"

Preferences preferences;

char buffer[PRGM_COUNT * sizeof(prgm_t)];
prgm_t *prgms = (prgm_t *) buffer;

void prefs_set_key(String key) {
  
  Serial.println("PREFS - Storing key");
  preferences.begin("application",false);
  preferences.putString("key", key);
  preferences.end();
  
}


void prefs_get_key(char * key) {
  
  Serial.print("PREFS - Reading key : ");
  preferences.begin("application",false);
  preferences.getString("key").toCharArray(key, KEY_LENGTH);
  Serial.println(key);
  preferences.end();
    
}


void prefs_set_accesspoint(String accesspoint) {
  
  Serial.println("PREFS - Storing accesspoint");
  preferences.begin("wifi",false);
  preferences.putString("accesspoint", accesspoint);
  preferences.end();
  
}


void prefs_get_accesspoint(char * accesspoint) {
  
  Serial.print("PREFS - Reading accesspoint : ");
  preferences.begin("wifi",false);
  preferences.getString("accesspoint").toCharArray(accesspoint, ACCESSPOINT_LENGTH);
  Serial.println(accesspoint);
  preferences.end();
  
}


void prefs_set_password(String password) {
  
  Serial.println("PREFS - Storing password");
  preferences.begin("wifi",false);
  preferences.putString("password", password);
  preferences.end();
  
}


void prefs_get_password(char * password) {
  
  Serial.print("PREFS - Reading password : ");
  preferences.begin("wifi",false);
  preferences.getString("password").toCharArray(password, PASSWORD_LENGTH);
  Serial.println(password);
  preferences.end();
  
}


uint16_t prefs_get_prgmcount() {

  uint16_t value = 0;
  
  Serial.print("PREFS - Reading prgmcount : ");
  preferences.begin("prgms",false);
  value = preferences.getUShort("prgmcount");
  Serial.println(value);
  preferences.end();

  return value;
  
}


void prefs_set_prgmcount(int prgmcount) {
  
  Serial.println("PREFS - Storing prgmcount");
  preferences.begin("prgms",false);
  preferences.putUShort("prgmcount", prgmcount);
  preferences.end();
  
}


void prefs_loadprgms() {
  
  Serial.println("PREFS - loadprgms");
  preferences.begin("prgms",false);

  size_t prgms_length = preferences.getBytesLength("prgms");

  Serial.print("PREFS - loadprgms - Size in memory :");
  Serial.println(prgms_length);

  if (prgms_length != PRGM_COUNT * sizeof(prgm_t)) { // simple check that data fits
    Serial.println("PRGM - loadprgms - Memory invalid or empty, cleaning");
    preferences.putBytes("prgms", prgms, PRGM_COUNT * sizeof(prgm_t));
  }
  
  preferences.getBytes("prgms", buffer, prgms_length);
  Serial.println("PREFS - loadprgms - Data loaded");

  preferences.end();
}


void prefs_storeprgms() {
  
  Serial.println("PREFS - storeprgms");
  preferences.begin("prgms",false);
  preferences.putBytes("prgms", prgms, PRGM_COUNT * sizeof(prgm_t));
  preferences.end();
}
