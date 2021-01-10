#ifndef SCHEDULEDTASKS_H_
#define SCHEDULEDTASKS_H_

// Scheduler variables
#define _TASK_SLEEP_ON_IDLE_RUN

void checkTriggerProgram();
void wifiLEDOn();
void wifiLEDOff();
void enable_wifiBlinkTask(void);
void enable_checkWifiTask(void);
void refresh_programTask(void);
void TXLEDOnfor1s(void);
void TXLEDOff(void);
void execute_runner(void);

#endif //SCHEDULEDTASKS_H_