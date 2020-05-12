
#include "WiFi.h"
#include "rs_wifi.h"
#include <DNSServer.h>
#include "prefs.h"

IPAddress apIP(10, 10, 10, 1);
DNSServer dnsServer;

void connect_to_wifi(void) {

  char ssid[ACCESSPOINT_LENGTH];
  prefs_get_accesspoint(ssid);
  char password[PASSWORD_LENGTH];
  prefs_get_password(password);



  Serial.println("WIFI - Connect to access point");
  Serial.print("WIFI - Access point : ");
  Serial.println(ssid);
  Serial.print("WIFI - Password : ");
  Serial.println(password);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
  }
  
  Serial.println("");
  Serial.println("WIFI - Connected !");
  Serial.print("WIFI IP Address : ");
  Serial.println(WiFi.localIP());
  
}

void start_softap(void) {
  
  Serial.println("WIFI - start_softap - Starting rescue mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("VoletRoulants");
  //dnsServer.start(53, "*", apIP);  //Cause a reboot in softAP mode
  
}

void handle_dns_requests(void) {
  dnsServer.processNextRequest();
}
