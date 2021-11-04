#ifndef DEVICE_HANDLER_H_INCLUDED
#define DEVICE_HANDLER_H_INCLUDED


/*
struct detection{
    time_t detection_time;
    struct detection* next;
    struct detection* previous;
    //int detected_by;
}typedef Detection;
*/

struct device{
    int count;
    struct device* next;
    char address[17];
    time_t first_detected;
    time_t last_detected;
    int blacklist;
    //int is_AP;
    //Detection* detections;
}typedef Device;


int receive_device(char* address);  //receive device's address from socket

//void insert_device(char* address);
//void remove_device(char* address);
//void update_device(char* address);
void blacklist_insert(char* address);
void blacklist_remove(char* address);
void device_scanner();

#endif // DEVICE_HANDLER_H_INCLUDED
