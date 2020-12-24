#include "WebServer.h"

#include "rs_webserver.h"
#include "prgm.h"
#include "prefs.h"
#include "vars.h"
#include "misc.h"
#include "rs_time.h"
#include "misc.h"

char key[10];

WebServer server(80);
extern prgm_t prgms[PRGM_COUNT];

char HEADER[] = "<html>\
<title>Roller shutter</title>\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">\
<link rel=\"stylesheet\" href=\"https://www.w3schools.com/lib/w3-theme-green.css\"> \
<body>";

char  FOOTER[] = "<br/></div></body></html>";

const char *remote_name[REMOTES_COUNT] = { 
  (char *)"Cuisine", 
  (char *)"Baie vitr&eacute;e",
  (char *)"Buffet",
  (char *)"Porte fen&ecirc;tre",
  (char *)"Bureau",
  (char *)"Chambre 1",
  (char *)"Chambre 2",
  (char *)"Chambre 3",
  (char *)"Tout bas",
  (char *)"Tout haut",
};

void ws_handle_client(void) {
  
  server.handleClient();
  
}


void handleWifi() {
  
  Serial.println("WEBSERVER - handleWifi - Serving wifi page");

  // GET Method

  char page[2000];

  if (server.method() == HTTP_POST) {
    String accesspoint = server.arg("accesspoint");
    Serial.print("WEBSERVER - handleWifi - Storing accesspoint : ");
    Serial.println(accesspoint);
    prefs_set_accesspoint(accesspoint);
    String password = server.arg("password");
    Serial.print("WEBSERVER - handleWifi - Storing password : ");
    Serial.println(password);
    prefs_set_password(password);
    
    redirect((char*)"/wifi");
    return;

  }

  char accesspoint_str[ACCESSPOINT_LENGTH];
  prefs_get_accesspoint(accesspoint_str);

  snprintf(page, 2000,"%s<header class=\"w3-container w3-card w3-theme\">\
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

  server.send(200, "text/html", page);
}


void handleClock() {
  
  Serial.println("WEBSERVER - handleClock - Serving clock page");

  // GET Method

  char page[2000];

  if (server.method() == HTTP_POST) {
    String hours = server.arg("hours");
    String minutes = server.arg("minutes");
    settime(hours.toInt(), minutes.toInt());
    redirect((char*)"/clock");
    return;

  }

  uint8_t hours=0;
  uint8_t minutes=0;
  gettime(&hours, &minutes);
  Serial.println(hours);

  snprintf(page, 2000,"%s<header class=\"w3-container w3-card w3-theme\">\
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

  server.send(200, "text/html", page);
}


void handleApplication() {
  
  Serial.println("WEBSERVER - handleApplication - Serving application page");

  // GET Method

  char page[2000];

  if (server.method() == HTTP_POST) {
    String key = server.arg("key");
    Serial.print("WEBSERVER - handleApplication - Storing key : ");
    Serial.println(key);
    prefs_set_key(key);


    redirect((char*)"/application");
    return;

  }

  char key_str[KEY_LENGTH];
  prefs_get_key(key_str);

  snprintf(page, 2000,"%s<header class=\"w3-container w3-card w3-theme\">\
<h1>Application</h1>\
</header>\
<div class=\"w3-container\">\
<form action=\"application\" method=\"post\">\
<p>\
<label class=\"w3-text-teal w3-xxlarge\"><b>Cl&eacute;</b></label>\
<input class=\"w3-input w3-border w3-light-grey w3-xxlarge\" id=\"key\" name=\"key\" type=\"text\" value=\"%s\"></p>\
<p>\
<input type=\"submit\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\" value=\"Enregistrer\">\
</form>\
<br/>\
<a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Retour</a>\
</button>\
</a>%s", HEADER, key_str, FOOTER);

  server.send(200, "text/html", page);
}


void handleConfig() {
  
  Serial.println("WEBSERVER - handleConfig - Serving config page");

  char page[2000];

  snprintf(page, 2000,"%s\
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
        
  server.send(200, "text/html", page);
  
}


void handleAttach() {
  
  Serial.println("WEBSERVER - handleAttach - Serving config config");

  String roller_str = server.arg("roller");
    if (roller_str.length() > 0) {
    int roller = roller_str.toInt();

    Serial.print("WEBSERVER - handleAttach - Attaching roller : ");
    Serial.println(roller);
    prog(roller);

    redirect((char*)"/attach");
    return;
  }

  int buffer_size=2000;

  char page[buffer_size];

  snprintf(page, buffer_size,"%s\
<header class=\"w3-container w3-card w3-theme\">\
<h1>Attacher</h1>\
</header>\
<div class=\"w3-container\">\
<br/>", HEADER);
// Loop on remotes
for (size_t i = 0; i < REMOTES_COUNT; i++)
{
  snprintf(page + strlen(page), buffer_size - strlen(page),"<a href=\"attach?roller=%d\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">%s</a>\
<br/>\
",i,remote_name[i]);
}
snprintf(page + strlen(page), buffer_size - strlen(page),"<a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">retour</a>%s", FOOTER);
        
  server.send(200, "text/html", page);
  
}


void handleCommand() {
  
  Serial.println("WEBSERVER - handleCommand - Serving command page");

  String roller_str = server.arg("roller");
  String command_str = server.arg("command");
  if (roller_str.length() > 0 && command_str.length() > 0) {
    int roller = roller_str.toInt();
    int command = command_str.toInt();
    if (command == 0) {
      Serial.print("WEBSERVER - handleCommand - Move down roller : ");
      Serial.println(roller);
      movedown(roller);
    }
    else if (command == 1) {
      Serial.print("WEBSERVER - handleCommand - Move up roller : ");
      Serial.println(roller);
      moveup(roller);
    }
    else if (command == 2) {
      Serial.print("WEBSERVER - handleCommand - Stop roller : ");
      Serial.println(roller);
      stop(roller);
    }
    
    redirect((char*)"/command");
    return;
  }

  // Page as: 290 char for top + 626 char per REMOTE + 44 char for end
  server.setContentLength(290 + 626 * REMOTES_COUNT + 44);
  server.send(200, "text/html", "");

  int buffer_size=2000;
  char page[buffer_size];
  // 290 char
  snprintf(page, buffer_size,"%s<div>\
<table  class=\"w3-table\">\
",HEADER);
  server.sendContent_P(page);

// Loop on remotes 
// 626 char per remote
for (size_t i = 0; i < REMOTES_COUNT; i++)
{
  snprintf(page, buffer_size,
  "<tr><td colspan=\"3\"><header class=\"w3-container w3-card w3-theme\">\
<h1>%s</h1>\
</header></td>\
</tr>\
<tr>\
<td><a href=\"command?roller=%d&command=0\" class=\"w3-button w3-red w3-xlarge w3-round-large\" style=\"width:100%%\"><span style='font-size:60px;'>&#8681;</span></a></td>\
<td><a href=\"command?roller=%d&command=2\" class=\"w3-button w3-grey w3-xlarge w3-round-large\" style=\"width:100%%\"><span style='font-size:60px;color:white;'>&#9634;</span></a></td>\
<td><a href=\"command?roller=%d&command=1\" class=\"w3-button w3-teal w3-xlarge w3-round-large\" style=\"width:100%%\"><span style='font-size:60px;'>&#8679;</span></a></td>\
</tr>",remote_name[i],i,i,i);
  server.sendContent_P(page);
}

  // 44 char
  snprintf(page, buffer_size, "</table>\
</div>\
<br/>%s", FOOTER);
server.sendContent_P(page);
  
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


void redirect(char * url) {

  char obfuscated_url[40];

  get_obfuscated_url(obfuscated_url, key, url);
  server.sendHeader("Location", obfuscated_url, true);
  server.send ( 302, "text/plain", "");

}


void handlePrgmList() {
  
  Serial.println("WEBSERVER - handlePrgmList - Serving programs list page");

  char page[2000] = "";
  uint8_t roller = 0;
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t command = 0;
  uint8_t result = 0;
  char rollername[20];
  char commandname[20];
  
  snprintf(page, 2000,"%s%s<header class=\"w3-container w3-card w3-theme\">\
  <h1>Programmes</h1>\
</header>\
<div class=\"w3-container\">\
<br/>\
<a href=\"prgmadd\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Nouveau</a>\
<br/>\
<table class=\"w3-table w3-large w3-striped\">\
<tr class=\"w3-teal\"><th>#</th><th>Volet</th><th>h</th><th>m</th><th>cmd</th><th></th>\
    </tr>", page, HEADER);
  int i = 1;
  for (int prgm=0; prgm<PRGM_COUNT;prgm++) {
    result = getprgm(prgm, &roller, &hour, &minute, &command);
    if (result) {
      getrollername(roller, rollername);
      getcommandname(command, commandname);
      snprintf(page, 2000,"%s\
      <tr><th scope=\"row\">%i</th><td>%s</td><td>%i</td><td>%i</td><td>%s</td>\
      <td><button class=\"w3-button w3-circle w3-red w3-button w3-small\"><a href=\"prgmdelete?prgm=%i\">&times;</a></button></td>\
      </td></tr>", page, i, rollername, hour, minute, commandname, prgm);

      i++;
     }
    
  }
  snprintf(page, 2000,"%s</table><br/><a href=\"config\" class=\"w3-button w3-teal w3-xxlarge w3-round-large w3-block\">Retour</a><br/></div>%s", page, FOOTER);
        
  server.send(200, "text/html", page);
}


void handlePrgmDelete(void) {
  
  Serial.println("WEBSERVER - handlePrgmDelete - Serving programs delete page");
  
  String prgm = server.arg("prgm");
  
  Serial.print("WEBSERVER - handlePrgmDelete - Deleting prgm : ");
  Serial.print(prgm);
  delprgm(prgm.toInt());

  redirect((char*)"/prgmlist");
  
}


void handlePrgmAdd() {
  
  Serial.println("WEBSERVER - handlePrgmAdd - Serving program add page");

  // GET Method

  char page[2500];

  if (server.method() == HTTP_POST) {
    String roller = server.arg("roller");
    Serial.print("WEBSERVER - handlePrgmAdd - Roller : ");
    Serial.println(roller);
    String hour = server.arg("hour");
    Serial.print("WEBSERVER - handlePrgmAdd - Hour : ");
    Serial.println(hour);
    String minute = server.arg("minute");
    Serial.print("WEBSERVER - handlePrgmAdd - Minute : ");
    Serial.println(minute);
    String command = server.arg("command");
    Serial.print("WEBSERVER - handlePrgmAdd - Command : ");
    Serial.println(command);

    if (roller.length() != 0 && hour .length() != 0 && minute.length() != 0 && command.length() != 0)
      addprgm(roller.toInt(), hour.toInt(), minute.toInt(), command.toInt());
    char url[40];
    get_obfuscated_url(url, key, (char*)"/prgmlist");
    server.sendHeader("Location", url, true);
    server.send ( 302, "text/plain", "");
    return;
  }

  snprintf(page, 2500,"%s<header class=\"w3-container w3-card w3-theme\">\
  <h1>Nouveau</h1>\
</header>\
<div class=\"w3-container\">\
<br/>\
      <form action=\"prgmadd\" method=\"post\">\
      <div class=\"form-group\">\
      <select class=\"w3-select w3-xxlarge\" id=\"roller\" name=\"roller\">\
        <option value=\"\" disabled selected>Volet</option>\
        <option value=\"0\">Cuisine</option>\
        <option value=\"1\">Salle &agrave; manger</option>\
        <option value=\"2\">Salon</option>\
      </select><br/><br/>\
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
<br/></div>%s", HEADER, FOOTER);

  server.send(200, "text/html", page);
}



void getrollername(uint8_t roller, char * name) {
  switch (roller) {
    case 0 :
      strcpy(name, "Cuisine");
      break;
    case 1 :
      strcpy(name, "Salle &agrave; m.");
      break;
    case 2 :
      strcpy(name, "Salon");
      break;
  }
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
    //eeprom_get_key(key);
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

  server.onNotFound(handleCommand);

  server.begin();

  Serial.println("WEBSERVER - ws_config - Web server started");
  
}
