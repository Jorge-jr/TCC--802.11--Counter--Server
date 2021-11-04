#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "server_socket.h"
#include "device_handler.h"
#include "user_interface.h"
#include <pthread.h>



Device* device_list = NULL;
int total = 0, count, remove_count;  //refatorar count -> line_count
int sleeping_time = 1;  //sets how long(seconds) the scanner will wait until the next iteration
int presence_timer = 12;  // sets how long(seconds) devices will be kept on device list
char blacklist[][17] = {"00:00:00:00:00:00", "b8:27:eb:81:3d:ed", "c0:38:96:a1:bd:7f", "68:02:b8:52:bd:b3", "d0:03:df:74:11:b8"};
char buf[256];

Device* remove_device(char* address);


int receive_device(char* address)  //receive device's address from socket
{
    Device* new_device;
    Device* devices;
    devices = device_list;
    if (devices == NULL){
        for(int i=0;i<=sizeof(blacklist)-1;i++){
        if(strncmp(address, blacklist[i], 17) == 0){
                return -1;
            }
        }
        new_device = (Device*) malloc (sizeof(Device));
        strncpy(new_device->address, address, sizeof(char) * 17);
        new_device->next = NULL;
        new_device->count = 1;
        new_device->first_detected = new_device->last_detected = time(NULL);
        device_list = new_device;
        total++;
        return 0;
    }else{
        while (devices != NULL){
            if (strncmp(devices->address, address, 17) == 0){  //strcmp()
                devices->count++;
                devices->last_detected = time(NULL);
                devices = NULL;
                return 1;
            }else if (devices->next != NULL){
                devices = devices->next;
            }else{
                for(int i=0;i<=sizeof(blacklist)-1;i++){
                    if(strncmp(address, blacklist[i], 17) == 0){
                        return -1;
                    }
                }
                new_device = (Device*) malloc (sizeof(Device));
                strncpy(new_device->address, address, sizeof(char) * 17);
                new_device->next = NULL;
                new_device->first_detected = new_device->last_detected = time(NULL);
                new_device->count = 1;
                devices->next = new_device;
                total++;
                devices=NULL;
                return 0;
            }
        }
    }
}


void device_scanner()
{
    ui_print(3, 1, 2, "Sniffing ...");
    Device* dev;

    while (true) {

        sprintf(buf, "%d            ", total);
        ui_print(1, 1, 16, buf);
        dev = device_list;
        count = 0;
        while(dev != NULL){

            if (difftime(time(NULL), dev->last_detected) >= presence_timer){
                dev = remove_device(dev->address);  //the remove device function will return de next node in case of removal or the same cause it fails
            }else{
                sprintf(buf, "%d \t %s \t %.0f  seconds ago \t %d", count+1, dev->address, difftime(time(NULL), dev->last_detected), dev->count);
                count ++;
                ui_print(2, count+2, 2, buf);
                dev = dev->next;
            }
        }
        //sleep (sleeping_time);
    }
}

//void insert_device(char* address);
Device* remove_device(char* address)
{
    Device* dev = device_list;
    Device* previous = NULL;
    //int remove_line_count = 1;
    while (dev != NULL){

        if (strncmp(dev->address, address, 17) == 0){
            if (previous == NULL){
                device_list = device_list->next;
            }else{
                previous->next = dev->next;
            }
            free(dev);
            dev = NULL;
            for(int i=3;i<total+3;i++){
                ui_print(2, i, 1, "                                                                            ");
            }
            total--;
            if(previous==NULL) return NULL;
            return previous->next;
        }else{
            previous = dev;
            dev = dev->next;
        }
        //remove device's entry from ui
        //ui_print(2, remove_line_count+2, 1, "                                                                            ");
        //remove_line_count++;
    }
    if (previous==NULL){
        if(device_list==NULL) return NULL;
        return device_list->next;
    }else{
        return previous->next;
    }
}

//void update_device(char* address);
void blacklist_insert(char* address);
void blacklist_remove(char* address);
//int blacklist_check(char* address);
