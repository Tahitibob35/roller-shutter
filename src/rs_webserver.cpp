#include "ESPAsyncWebServer.h"

#include "rs_webserver.h"
#include "prgm.h"
#include "prefs.h"
#include "vars.h"
#include "misc.h"
#include "rs_time.h"
#include "misc.h"
#include "rs_scheduledtasks.h"

char key[10];
char token[16]; // Token to validate API calls

AsyncWebServer server(80);
extern prgm_t prgms[PRGM_COUNT];

char HEADER[] = "<html>\
<title>Roller shutter</title>\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">\
<link rel=\"stylesheet\" href=\"https://www.w3schools.com/lib/w3-theme-green.css\"> \
<body>";

char  FOOTER[] = "<br/></div></body></html>";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


void handleWifi(AsyncWebServerRequest *request) {

  Serial.println("WEBSERVER - handleWifi - Serving wifi page");

  if (request->method() == HTTP_POST) {
    String accesspoint = request->arg("accesspoint");
    Serial.print("WEBSERVER - handleWifi - Storing accesspoint : ");
    Serial.println(accesspoint);
    prefs_set_accesspoint(accesspoint);
    String password = request->arg("password");
    Serial.print("WEBSERVER - handleWifi - Storing password : ");
    Serial.println(password);
    prefs_set_password(password);

    redirect(request, (char*)"/wifi");
    return;

  }

  // GET Method
  if (request->method() == HTTP_GET) {
    char page[PAGE_LENGTH];
    char accesspoint_str[ACCESSPOINT_LENGTH];
    prefs_get_accesspoint(accesspoint_str);

    snprintf(page, PAGE_LENGTH,"%s<header class=\"w3-container w3-card w3-theme\">\
<h1>Wifi</h1>\
</header>\
<div class=\"w3-container\">\
<form action=\"wifi\" method=\"post\">\
<p>\
<label class=\"w3-text-teal w3-xxlarge\"><b>Point d'acc&egrave;s</b></label>\
<input class=\"w3-input w3-border w3-light-grey w3-xxlarge\" id=\"accesspoint\" name=\"accesspoint\" type=\"text\" value=\"%s\"></p>\
<p>\
<p>\
<label class=\"w3-text-teal w3-xxlarge\"><b>Mot de passe</b></label>\
<input class=\"w3-input w3-border w3-light-grey w3-xxlarge \" id=\"password\" name=\"password\" type=\"text\"></p>\
<p>\
<input type=\"submit\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\" value=\"Enregistrer\">\
</form>\
<br/>\
<a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Retour</a>\
</button>\
</a>%s", HEADER, accesspoint_str, FOOTER);

    request->send(200, "text/html", page);
  }
}


void handleClock(AsyncWebServerRequest *request) {

  Serial.println("WEBSERVER - handleClock - Serving clock page");

  if (request->method() == HTTP_POST) {
    String hours = request->arg("hours");
    String minutes = request->arg("minutes");
    settime(hours.toInt(), minutes.toInt());
    redirect(request, (char*)"/clock");
    return;

  }

  if (request->method() == HTTP_GET) {
    char page[PAGE_LENGTH];

    uint8_t hours=0;
    uint8_t minutes=0;
    gettime(&hours, &minutes);
    Serial.println(hours);

    snprintf(page, PAGE_LENGTH,"%s<header class=\"w3-container w3-card w3-theme\">\
<h1>Horloge</h1>\
</header>\
<div class=\"w3-container\">\
<form action=\"clock\" method=\"post\">\
<p>\
<label class=\"w3-text-teal w3-xxlarge\"><b>Heures</b></label>\
<input class=\"w3-input w3-border w3-light-grey w3-xxlarge\" id=\"hours\" name=\"hours\" type=\"text\" value=\"%i\"></p>\
<p>\
<p>\
<label class=\"w3-text-teal w3-xxlarge\"><b>Minutes</b></label>\
<input class=\"w3-input w3-border w3-light-grey w3-xxlarge \" id=\"minutes\" name=\"minutes\" type=\"text\" value=\"%i\"></p>\
<p>\
<input type=\"submit\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\" value=\"Enregistrer\">\
</form>\
<br/>\
<a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Retour</a>\
</button>\
</a>%s", HEADER, hours, minutes, FOOTER);

    request->send(200, "text/html", page);
  }
}


void handleApplication(AsyncWebServerRequest *request) {

  Serial.println("WEBSERVER - handleApplication - Serving application page");

  if (request->method() == HTTP_POST) {
    if(request->hasArg("key")){
      String key = request->arg("key");
      write_output("WEBSERVER - handleApplication - Storing key : " + key);
      prefs_set_key(key);
    }

    if(request->hasArg("token")){
      String token_arg = request->arg("token");
      write_output("WEBSERVER - handleApplication - Storing token : " + token_arg);
      prefs_set_token(token_arg);
    }

    if(request->hasArg("syslog_state")){
      String syslog_state = request->arg("syslog_state");
      write_output("WEBSERVER - handleApplication - Storing syslog_state : " + syslog_state);
      if(syslog_state.compareTo("yes") == 0){
        // syslog_state == "yes"
        prefs_set_syslog_state(true);
      }else{
        // syslog_state != "yes
        prefs_set_syslog_state(false);
      }
    }

    if(request->hasArg("syslog_ip") ){
      String syslog_ip = request->arg("syslog_ip");
      write_output("WEBSERVER - handleApplication - Storing syslog_ip : " + syslog_ip);
      prefs_set_syslog_ip(syslog_ip);
    }

    if(request->hasArg("syslog_port")){
      String syslog_port = request->arg("syslog_port");
      write_output("WEBSERVER - handleApplication - Storing syslog_port : " + syslog_port);
      prefs_set_syslog_port(syslog_port.toInt());
    }

    if(request->hasArg("ntp_server")){
      String ntp_server = request->arg("ntp_server");
      write_output("WEBSERVER - handleApplication - Storing ntp_server : " + ntp_server);
      prefs_set_ntp_server(ntp_server);
    }

    redirect(request, (char*)"/application");
    return;
  }


  // GET Method
  if (request->method() == HTTP_GET) {
    char page[PAGE_LENGTH];
    char key_str[KEY_LENGTH];
    prefs_get_key(key_str);
    prefs_get_token(token);

    boolean syslog_state = false;
    char syslog_ip[IP_LENGTH];
    int16_t syslog_port;
    syslog_state = prefs_get_syslog_state();
    prefs_get_syslog_ip(syslog_ip);
    syslog_port = prefs_get_syslog_port();

    char ntp_server[NTP_SERVER_LENGTH];
    prefs_get_ntp_server(ntp_server);

    snprintf(page, PAGE_LENGTH,"%s<header class=\"w3-container w3-card w3-theme\">\
<h1>Application</h1>\
</header>\
<div class=\"w3-container\">\
<form action=\"application\" method=\"post\">\
<p>\
  <label class=\"w3-text-teal w3-xxlarge\"><b>Cl&eacute;</b></label>\
  <input class=\"w3-input w3-border w3-light-grey w3-xxlarge\" id=\"key\" name=\"key\" type=\"text\" value=\"%s\"></p>\
<p>\
<p>\
  <label class=\"w3-text-teal w3-xxlarge\"><b>Token</b></label>\
  <input class=\"w3-input w3-border w3-light-grey w3-xxlarge\" id=\"token\" name=\"token\" type=\"text\" value=\"%s\"></p>\
<p>\
<p>\
  <label class=\"w3-text-teal w3-xxlarge\" for=\"fname\">Activer syslog externe:</label><br/>\
  <input type=\"radio\" id=\"syslog_state\" name=\"syslog_state\" value=\"yes\" %s>\
  <label class=\"w3-text-teal w3-xxlarge\" for=\"yes\">Oui</label><br/>\
  <input type=\"radio\" id=\"syslog_state\" name=\"syslog_state\" value=\"no\" %s>\
  <label class=\"w3-text-teal w3-xxlarge\" for=\"no\">Non</label><br/>\
  <label for=\"syslog_ip\">Syslog serveur IP:</label><br/>\
  <input class=\"w3-input w3-border w3-light-grey w3-xxlarge\" type=\"text\" id=\"syslog_ip\" name=\"syslog_ip\" value=\"%s\"><br/>\
  <label for=\"syslog_port\">Syslog serveur port:</label><br/>\
  <input class=\"w3-input w3-border w3-light-grey w3-xxlarge\" type=\"text\" id=\"syslog_port\" name=\"syslog_port\" value=\"%d\"><br/>\
  <label for=\"ntp_server\">NTP serveur:</label><br/>\
  <input class=\"w3-input w3-border w3-light-grey w3-xxlarge\" type=\"text\" id=\"ntp_server\" name=\"ntp_server\" value=\"%s\"><br/>\
</p>\
<input type=\"submit\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\" value=\"Enregistrer\">\
</form>\
<br/>\
<a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Retour</a>\
</button>\
</a>%s", HEADER, key_str, token, syslog_state ? "checked" : " ", syslog_state ? " " : "checked", syslog_ip, syslog_port, ntp_server, FOOTER);

    request->send(200, "text/html", page);
    return;
  }
}


void handleConfig(AsyncWebServerRequest *request) {

  Serial.println("WEBSERVER - handleConfig - Serving config page");

  char page[PAGE_LENGTH];

  snprintf(page, PAGE_LENGTH,"%s\
<header class=\"w3-container w3-card w3-theme\">\
<h1>Configuration</h1>\
</header>\
<div class=\"w3-container\">\
<br/>\
<a href=\"prgmlist\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Programmation</a>\
<br/>\
<a href=\"attach\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Attacher</a>\
<br/>\
<a href=\"wifi\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Wifi</a>\
<br/>\
<a href=\"application\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Application</a>\
<br/>\
<a href=\"clock\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Horloge</a>%s", HEADER, FOOTER);

  request->send(200, "text/html", page);

}


void handleAttach(AsyncWebServerRequest *request) {

  Serial.println("WEBSERVER - handleAttach - Serving config config");

  if (request->hasArg("roller")) {
    String roller_str = request->arg("roller");
    int roller = roller_str.toInt();

    Serial.print("WEBSERVER - handleAttach - Attaching roller : ");
    Serial.println(roller);
    prog(roller);

    redirect(request, (char*)"/attach");
    return;
  }


  char page[PAGE_LENGTH];

  snprintf(page, PAGE_LENGTH,"%s\
<header class=\"w3-container w3-card w3-theme\">\
<h1>Attacher</h1>\
</header>\
<div class=\"w3-container\">\
<br/>", HEADER);
  // Loop on remotes
  for (size_t i = 0; i < REMOTES_COUNT; i++)
  {
    snprintf(page + strlen(page), PAGE_LENGTH - strlen(page),"<a href=\"attach?roller=%d\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">%s</a>\
<br/>\
",i,remote_name[i]);
  }
  snprintf(page + strlen(page), PAGE_LENGTH - strlen(page),"<a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Retour</a>%s", FOOTER);

  request->send(200, "text/html", page);

}


void handleApi(AsyncWebServerRequest *request) {
  write_output("WEBSERVER - handleAPI - Serving API page");

  if (request->hasArg("roller") && request->hasArg("command") && request->hasArg("token")) {
    String roller_str = request->arg("roller");
    String command_str = request->arg("command");
    String token_str = request->arg("token");
    int command = command_str.toInt();
    int roller = roller_str.toInt();

    if (token_str.compareTo(token) != 0){
      write_output("WEBSERVER - handleAPI - wrong token: " + token_str);
      request->send(401);
    }else{
      if (command == 0) {
        write_output("WEBSERVER - handleAPI - Move down roller : " + String(roller));
        request->send(200, "text/plain", "OK");
        movedown(roller);
        return;
      }
      else if (command == 1) {
        write_output("WEBSERVER - handleAPI - Move up roller : " + String(roller));
        request->send(200, "text/plain", "OK");
        moveup(roller);
        return;
      }
      else if (command == 2) {
        write_output("WEBSERVER - handleAPI - Stop roller : " + String(roller));
        request->send(200, "text/plain", "OK");
        stop(roller);
        return;
      }
    }
  }
}

void handleCommand(AsyncWebServerRequest *request) {

  write_output("WEBSERVER - handleCommand - Serving command page");

  if (request->hasArg("roller") && request->hasArg("command")) {
    String roller_str = request->arg("roller");
    String command_str = request->arg("command");
    int roller = roller_str.toInt();
    int command = command_str.toInt();
    if (command == 0) {
        write_output("WEBSERVER - handleCommand - Move down roller : " + String(roller));
        movedown(roller);
      }
      else if (command == 1) {
        write_output("WEBSERVER - handleCommand - Move up roller : " + String(roller));
        moveup(roller);
      }
      else if (command == 2) {
        write_output("WEBSERVER - handleCommand - Stop roller : " + String(roller));
        stop(roller);
    }

    redirect(request,(char*)"/command");
    return;
  }

  AsyncResponseStream *response = request->beginResponseStream("text/html");

  response->printf("%s<div>\
<table  class=\"w3-table\">\
",HEADER);

// Loop on remotes
for (size_t i = 0; i < REMOTES_COUNT; i++)
{
  response->printf(
  "<tr><td colspan=\"3\"><header class=\"w3-container w3-card w3-theme\">\
<h1>%s</h1>\
</header></td>\
</tr>\
<tr>\
<td><a href=\"command?roller=%d&command=0\" class=\"w3-button w3-red w3-xlarge w3-round-large\" style=\"width:100%%\"><span style='font-size:60px;'>&#8681;</span></a></td>\
<td><a href=\"command?roller=%d&command=2\" class=\"w3-button w3-grey w3-xlarge w3-round-large\" style=\"width:100%%\"><span style='font-size:60px;color:white;'>&#9634;</span></a></td>\
<td><a href=\"command?roller=%d&command=1\" class=\"w3-button w3-teal w3-xlarge w3-round-large\" style=\"width:100%%\"><span style='font-size:60px;'>&#8679;</span></a></td>\
</tr>",remote_name[i],i,i,i);
}

  response->printf("</table>\
</div>\
<br/>%s", FOOTER);

  request->send(response);
}

void handlePrgmList(AsyncWebServerRequest *request) {

  Serial.println("WEBSERVER - handlePrgmList - Serving programs list page");

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  uint8_t roller = 0;
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t command = 0;
  uint8_t result = 0;
  char commandname[20];

  response->printf("%s<header class=\"w3-container w3-card w3-theme\">\
  <h1>Programmes</h1>\
</header>\
<div class=\"w3-container\">\
<br/>\
<a href=\"prgmadd\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Nouveau</a>\
<br/>\
<table class=\"w3-table w3-large w3-striped\">\
<tr class=\"w3-teal\"><th>#</th><th>Volet</th><th>h</th><th>m</th><th>cmd</th><th></th>\
</tr>", HEADER);

  int i = 1;
  for (int prgm=0; prgm<PRGM_COUNT;prgm++) {
    result = getprgm(prgm, &roller, &hour, &minute, &command);
    if (result) {
      Serial.print("Roller id : ");
      Serial.println(roller);
      getcommandname(command, commandname);
      response->printf("\
<tr><th scope=\"row\">%i</th><td>%s</td><td>%i</td><td>%i</td><td>%s</td>\
<td><button class=\"w3-button w3-circle w3-red w3-button w3-small\"><a href=\"prgmdelete?prgm=%i\">&times;</a></button></td>\
</td></tr>", i, remote_name[roller], hour, minute, commandname, prgm);

      i++;
     }

  }
  response->printf("</table><br/><a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Retour</a><br/></div>%s", FOOTER);

  request->send(response);
}

void handlePrgmDelete(AsyncWebServerRequest *request) {

  Serial.println("WEBSERVER - handlePrgmDelete - Serving programs delete page");

  if(request->hasArg("prgm")){
    String prgm = request->arg("prgm");

    Serial.print("WEBSERVER - handlePrgmDelete - Deleting prgm : ");
    Serial.print(prgm);
    delprgm(prgm.toInt());
    refresh_programTask();
  }

  redirect(request, (char*)"/prgmlist");

}


void handlePrgmAdd(AsyncWebServerRequest *request) {

  Serial.println("WEBSERVER - handlePrgmAdd - Serving program add page");

  if (request->method() == HTTP_POST) {
    String roller = request->arg("roller");
    Serial.print("WEBSERVER - handlePrgmAdd - Roller : ");
    Serial.println(roller);
    String hour = request->arg("hour");
    Serial.print("WEBSERVER - handlePrgmAdd - Hour : ");
    Serial.println(hour);
    String minute = request->arg("minute");
    Serial.print("WEBSERVER - handlePrgmAdd - Minute : ");
    Serial.println(minute);
    String command = request->arg("command");
    Serial.print("WEBSERVER - handlePrgmAdd - Command : ");
    Serial.println(command);

    if (roller.length() != 0 && hour .length() != 0 && minute.length() != 0 && command.length() != 0) {
      addprgm(roller.toInt(), hour.toInt(), minute.toInt(), command.toInt());
      refresh_programTask();
    }
    redirect(request, (char*)"/prgmlist");
    return;
  }

  if (request->method() == HTTP_GET) {
    AsyncResponseStream *response = request->beginResponseStream("text/html");

    response->printf("%s<header class=\"w3-container w3-card w3-theme\">\
<h1>Nouveau</h1>\
</header>\
<div class=\"w3-container\">\
<br/>\
      <form action=\"prgmadd\" method=\"post\">\
      <div class=\"form-group\">\
      <select class=\"w3-select w3-xxlarge\" id=\"roller\" name=\"roller\">\
        <option value=\"\" disabled selected>Volet</option>", HEADER);

    for (size_t i = 0; i < REMOTES_COUNT; i++) {
        response->printf("<option value=\"%d\">%s</option>", i, remote_name[i]);
    }

    response->printf("</select><br/><br/>\
<select class=\"w3-select w3-xxlarge\" id=\"hour\" name=\"hour\">\
<option value=\"\" disabled selected>Heures</option>\
<option value=\"7\">7</option>\
<option value=\"8\">8</option>\
<option value=\"9\">9</option>\
<option value=\"10\">10</option>\
<option value=\"11\">11</option>\
<option value=\"12\">12</option>\
<option value=\"13\">13</option>\
<option value=\"14\">14</option>\
<option value=\"15\">15</option>\
<option value=\"16\">16</option>\
<option value=\"17\">17</option>\
<option value=\"18\">18</option>\
<option value=\"19\">19</option>\
<option value=\"20\">20</option>\
<option value=\"21\">21</option>\
<option value=\"22\">22</option>\
<option value=\"23\">23</option>\
      </select><br/><br/>\
        <select class=\"w3-select w3-xxlarge\" id=\"minute\" name=\"minute\">\
        <option value=\"\" disabled selected>Minutes</option>\
        <option value=\"0\">0</option>\
        <option value=\"5\">5</option>\
        <option value=\"10\">10</option>\
        <option value=\"15\">15</option>\
        <option value=\"20\">20</option>\
        <option value=\"25\">25</option>\
        <option value=\"30\">30</option>\
        <option value=\"35\">35</option>\
        <option value=\"40\">40</option>\
        <option value=\"45\">45</option>\
        <option value=\"50\">50</option>\
        <option value=\"55\">55</option>\
      </select><br/><br/>\
        <select class=\"w3-select w3-xxlarge\" id=\"command\" name=\"command\">\
        <option value=\"\" disabled selected>Commande</option>\
        <option value=\"0\">Fermer</option>\
        <option value=\"1\">Ouvrir</option>\
      </select>\
      </div><br/>\
      <input type=\"submit\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\" value=\"Enregistrer\">\
    </form><br/>\
    <a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Retour</a>\
</a>\
<br/></div>%s", FOOTER);

    request->send(response);
  }
}

void get_obfuscated_url(char * url, char * key, char * baseurl) {

  if (strlen(key) == 0) {
    strcpy(url, baseurl);
  }
  else {
    sprintf(url, "/%s%s", key, baseurl);
  }
  Serial.print("WEBSERVER - get_obfuscated_url - url :");
  Serial.println(url);

}

void redirect(AsyncWebServerRequest *request, char * url) {

  char obfuscated_url[40];

  get_obfuscated_url(obfuscated_url, key, url);


  request->redirect(obfuscated_url);
}

void getcommandname(uint8_t command, char * name) {
  switch (command) {
    case 0 :
      strcpy(name, "Fermer");
      break;
    case 1 :
      strcpy(name, "Ouvrir");
      break;
  }
}


void ws_config(int rescue_mode) {

  Serial.println("WEBSERVER - ws_config - Initialization");


  if (rescue_mode) {
    strcpy(key, "");
  }
  else {
    strcpy(key, "");
    prefs_get_key(key);
    write_output("Loaded application key: " + String(key));
    prefs_get_token(token);
    write_output("Loaded API token: ###########");
    Serial.println("Loaded API token: " + String(token));
  }
  char url[40];

  get_obfuscated_url(url, key, (char*)"/");
  server.on(url, handleCommand);
  get_obfuscated_url(url, key, (char*)"/command");
  server.on(url, handleCommand);
  get_obfuscated_url(url, key, (char*)"/config");
  server.on(url, handleConfig);
  get_obfuscated_url(url, key, (char*)"/wifi");
  server.on(url, handleWifi);
  get_obfuscated_url(url, key, (char*)"/application");
  server.on(url, handleApplication);
  get_obfuscated_url(url, key, (char*)"/prgmlist");
  server.on(url, handlePrgmList);
  get_obfuscated_url(url, key, (char*)"/prgmdelete");
  server.on(url, handlePrgmDelete);
  get_obfuscated_url(url, key, (char*)"/prgmadd");
  server.on(url, handlePrgmAdd);
  get_obfuscated_url(url, key, (char*)"/attach");
  server.on(url, handleAttach);
  get_obfuscated_url(url, key, (char*)"/clock");
  server.on(url, handleClock);
  get_obfuscated_url(url, key, (char*)"/api");
  server.on(url, handleApi);

  server.onNotFound(notFound);

  server.begin();

  Serial.println("WEBSERVER - ws_config - Web server started");
}
