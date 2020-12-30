#ifndef MISC_H_
#define MISC_H_

#include "Arduino.h"

void write_output(String message);
void setup_syslog();
void wifiblink(void);
void moveup(int roller);
void movedown(int roller);
void prog(int roller);
void stop(int roller);

#endif //MISC_H_