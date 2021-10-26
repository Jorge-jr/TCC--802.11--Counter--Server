#include <stdio.h>
#include <pthread.h>
#include "server_socket.h"
#include "user_interface.h"
#include "device_handler.h"


int main(int argc, char const *argv[])
{

    pthread_t scanner, ui;
    ui_start();
    pthread_create(&scanner, NULL, device_scanner, NULL);
    socket_start_udp();  //start socket listening and device handler
    //start.interface();


	return 0;
}
