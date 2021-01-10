
#include "WiFi.h"
#include "rs_wifi.h"
#include "DNSServer.h"
#include "prefs.h"
#include "ESP32Ping.h"
#include "misc.h"

IPAddress apIP(10, 10, 10, 1);
DNSServer dnsServer;
// IP to ping
IPAddress IPtoPing(8, 8, 8, 8);
int pingThreshold = 2000;
int failingWifi = 0;

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
  
  IPtoPing = WiFi.gatewayIP();
  Serial.print("Gateway IP Address : ");
  Serial.println(IPtoPing.toString());
}

void check_wifi(void){
  int avg_ms = 2000;
  boolean ping_sucess = Ping.ping(IPtoPing, 9);
  avg_ms = Ping.averageTime();

  write_output("ping:" + String(avg_ms) + "ms");

  if(avg_ms < pingThreshold && ping_sucess){
    failingWifi=0;
  }else{
    write_output("Connection Failed! Reconnecting...");
    WiFi.reconnect();
    delay(2000);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      write_output("Connection failed again.");
      failingWifi=failingWifi+1;
      if (failingWifi>MAX_FAILING_WIFI){
        write_output("ESP will restart now...");
        delay(100);
        ESP.restart();
      }
    }
    write_output("Connection fails but reconnected :)");
  }
}

void start_softap(void) {
  
  write_output("WIFI - start_softap - Starting rescue mode");
  WiFi.mode(WIFI_AP);
  delay(2000);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("VoletRoulants");
  dnsServer.start(53, "*", apIP);  //Cause a reboot in softAP mode
  
}

void handle_dns_requests(void) {
  dnsServer.processNextRequest();
}
