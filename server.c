#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include <time.h>
#include "server_socket.h"
#include "device_handler.h"


struct device{
    long int count;
    struct device *next;
    char address[17];
    time_t first_detected;
    time_t last_detected;
    //double difftime(time_t time1, time_t time2)
}typedef Device;


int main(int argc , char *argv[])
{

    pthread_t printer;
    Device* device_list = NULL;
    Device* devices;
    Device* new_device;
    int sock , new_socket , c, vread;
    long int total = 0;
    struct sockaddr_in server , client;
    char* message;
    char sniffed_address[17];
    int mx=0, my=0;

    initscr ();
    getmaxyx(stdscr, mx, my);
    WINDOW* head = newwin(4, my, 0, 0);
    WINDOW* body = newwin(mx-7, my, 4, 0);
    WINDOW* foot = newwin(3, my, mx-3, 0);
    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_GREEN,   COLOR_BLUE);
        init_pair(2, COLOR_BLACK,   COLOR_CYAN);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_BLACK,   COLOR_WHITE);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
    }
    refresh();
    box(foot, 0, 0);
    box(head, 0, 0);
    box(body, 0, 0);
    box(stdscr, 0, 0);
    curs_set (0);  //Hide cursor
    //wattrset(foot, COLOR_PAIR(1));
    wattrset(head, COLOR_PAIR(1));
    wbkgd(foot, COLOR_PAIR(5));
    mvwprintw(head, 1, 1, "Found devices: %d", total);
    mvwprintw(body, 2, 5, "Nº \t Address \t Last detected \t Count");
    refresh();
    wrefresh(head);
    wrefresh(body);
    wrefresh(foot);

    void* scanner(void* arg){

        char t[4] = {'-', '\\', '|', '/'};
        int t_i = 0;
        int mx=0, my=0;
        Device* dev;
        int line;
        struct tm * last_timeinfo;
        //struct tm * first_timeinfo;
        char last_time[80];
        //char first_time[80];

        while (true) {

            //sleep(1);
            dev = device_list;
            line = 4;
            mvwprintw(head, 1, 1, "Found devices: %d  \t %c", total, t[t_i]);
            (t_i==3?t_i=0:t_i++);
            wrefresh(head);

            while(dev != NULL){

                //first_timeinfo = localtime(&dev->first_detected);
                last_timeinfo = localtime(&dev->last_detected);
                strftime(last_time, 80, "%T", last_timeinfo);
                //strftime(first_time, 80, "%T", first_timeinfo);
                mvwprintw(body, line++, 5, "%d \t %s \t %s \t  %d ", line-3, dev->address, last_time, dev->count);
                refresh();
                wrefresh(body);
                dev = dev->next;
            }

            refresh ();
            sleep (1);
            //printf("----------------------------------------\n");
        }

        return NULL;
    }

    pthread_create(&printer, NULL, scanner, (void*) device_list);

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
	{
		printf("Could not create socket");
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );

	if(bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	//puts("bind done");

	listen(sock, 3);

	//Accept and incoming connection
	//puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (new_socket = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		//puts("Connection accepted");


        devices = device_list;
        vread = read(new_socket, &sniffed_address, sizeof(sniffed_address));
        if (devices ==  NULL){
            new_device = (Device*) malloc (sizeof(Device));
            //new_device->address[0] = sniffed_address[0];
            strncpy(new_device->address, sniffed_address, sizeof(char) * 17);
            new_device->next = NULL;
            new_device->count = 1;
            new_device->first_detected = time(NULL);
            new_device->last_detected = time(NULL);
            device_list = new_device;
            total++;
        }else{
            while (devices != NULL){

                if (strcmp(devices->address, sniffed_address)== 0){  //strcmp()
                    devices->count++;
                    new_device->last_detected = time(NULL);
                    mvwprintw(foot, 1, 1, "%F", difftime(devices->last_detected, devices->first_detected));
                    wrefresh(foot);
                    devices = NULL;

                }else if (devices->next != NULL){
                    devices = devices->next;
                }else{
                    new_device = (Device*) malloc (sizeof(Device));
                    strncpy(new_device->address, sniffed_address, sizeof(char) * 17);
                    new_device->next = NULL;
                    new_device->first_detected = time(NULL);
                    new_device->last_detected = time(NULL);
                    new_device->count = 1;
                    devices->next = new_device;
                    devices = NULL;
                    total++;
                }
            }
        }
		//message = "Message received\n";
		//write(new_socket , message , strlen(message));
		//printf("%s\n", sniffed_address);
		close(new_socket);
	}

	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}

	return 0;
}

