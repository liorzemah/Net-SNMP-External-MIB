#ifndef NICNOTIFICATION_H
#define NICNOTIFICATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* prototypes for the example */
void init_nicNotification(void);
int check_nic_status(char* device);
SNMPAlarmCallback send_nic_notification;


#ifdef __cplusplus
}
#endif

#endif /* NICNOTIFICATION_H */
