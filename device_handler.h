#ifndef DEVICE_HANDLER_H_INCLUDED
#define DEVICE_HANDLER_H_INCLUDED


struct device{
    int count;
    struct device *next;
    char address[17];
    time_t first_detected;
    time_t last_detected;
    //double difftime(time_t time1, time_t time2)
}typedef Device;


void receive_device(char* address);  //receive device's address from socket

//void insert_device(char* address);
//void remove_device(char* address);
//void update_device(char* address);
void blacklist_insert(char* address);
void blacklist_remove(char* address);
void device_scanner();

#endif // DEVICE_HANDLER_H_INCLUDED
