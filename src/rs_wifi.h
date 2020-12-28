#ifndef RS_WIFI_H_
#define RS_WIFI_H_

#include "Arduino.h"
#define MAX_FAILING_WIFI 10

void connect_to_wifi(void);
void start_softap(void);
void handle_dns_requests(void);
void check_wifi(void);

#endif /* RS_WIFI_H_ */
