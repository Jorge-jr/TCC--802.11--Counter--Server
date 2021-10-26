#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "server_socket.h"
#include "device_handler.h"
#include "user_interface.h"


Device* device_list = NULL;
int total = 0, count;
int sleeping_time = 1;
int presence_timer = 12;  // excludes devices from list after 2 minutes without detection
char blacklist[] = {"00:00:00:00:00:00"};
char buf[256];

void remove_device(char* address);


void receive_device(char* address)  //receive device's address from socket
{
    Device* new_device;
    Device* devices;
    devices = device_list;
    if (devices ==  NULL){
        new_device = (Device*) malloc (sizeof(Device));
        strncpy(new_device->address, address, sizeof(char) * 17);
        new_device->next = NULL;
        new_device->count = 1;
        new_device->first_detected = new_device->last_detected = time(NULL);
        device_list = new_device;
        total++;
    }else{
        while (devices != NULL){
            if (strcmp(devices->address, address)== 0){  //strcmp()
                devices->count++;
                devices->last_detected = time(NULL);
                devices = NULL;
            }else if (devices->next != NULL){
                devices = devices->next;
            }else{
                new_device = (Device*) malloc (sizeof(Device));
                strncpy(new_device->address, address, sizeof(char) * 17);
                new_device->next = NULL;
                new_device->first_detected = new_device->last_detected = time(NULL);
                new_device->count = 1;
                devices->next = new_device;
                total++;
                devices=NULL;
            }
        }
    }
}

void device_scanner()
{
    ui_print(3, 1, 2, "Starting device scanner ...");
    Device* dev;

    while (true) {

        sprintf(buf, "%d", total);
        ui_print(1, 1, 16, buf);
        dev = device_list;
        count = 0;
        while(dev != NULL){

            if (difftime(time(NULL), dev->last_detected) > presence_timer){
                remove_device(dev->address);
            }else{
                sprintf(buf, "%d \t %s \t %s \t %d \t %f", count+1, dev->address, "test", dev->count, difftime(time(NULL), dev->last_detected));
                count ++;
                ui_print(2, count+2, 2, buf);
            }
            dev = dev->next;
        }
        sleep (sleeping_time);
    }
}

//void insert_device(char* address);
void remove_device(char* address)
{
    Device* dev = device_list;
    Device* previous = NULL;

    while (dev != NULL){

        if (strcmp(dev->address, address)== 0){
            if (dev->next == NULL){
                //if the device is the last one it's needed to remove its entry from ui
                ui_print(2, total+2, 1, "                                                                            ");
            }
            if (previous == NULL){
                device_list = device_list->next;
            }else{
                previous->next = dev->next;
            }
            free(dev);
            dev = NULL;
            total--;
        }else{
            previous = dev;
            dev = dev->next;
        }
    }
}

//void update_device(char* address);
void blacklist_insert(char* address);
void blacklist_remove(char* address);
//int blacklist_check(char* address);
