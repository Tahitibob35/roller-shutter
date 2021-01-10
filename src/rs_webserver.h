#ifndef RS_WEBSERVER_H_
#define RS_WEBSERVER_H_
#include "ESPAsyncWebServer.h"

void notFound(AsyncWebServerRequest *request);
void handleWifi(AsyncWebServerRequest *request);
void handleClock(AsyncWebServerRequest *request);
void handleApplication(AsyncWebServerRequest *request);
void handleConfig(AsyncWebServerRequest *request);
void handleAttach(AsyncWebServerRequest *request);
void handleApi(AsyncWebServerRequest *request);
void handleCommand(AsyncWebServerRequest *request);
void handlePrgmList(AsyncWebServerRequest *request);
void handlePrgmDelete(AsyncWebServerRequest *request);
void handlePrgmAdd(AsyncWebServerRequest *request);

void get_obfuscated_url(char * url, char * key, char * baseurl);
void redirect(AsyncWebServerRequest *request, char * url);
void getcommandname(uint8_t command, char * name);
void ws_config(int rescue_mode);

#define MAX_STRING_SIZE 40
#define PAGE_LENGTH 4000

#endif /* RS_WEBSERVER_H_ */
