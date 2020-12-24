#ifndef RS_WEBSERVER_H_
#define RS_WEBSERVER_H_

void ws_config(int rescue_mode);
void ws_handle_client(void);
void handleHome();
void getrollername(uint8_t roller, char * name);
void getcommandname(uint8_t command, char * name);
void handlePrgmDelete(void);
void redirect(char * url);

#define REMOTES_COUNT 10
#define MAX_STRING_SIZE 40

#endif /* RS_WEBSERVER_H_ */
