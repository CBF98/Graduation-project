#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>


#define UDP_PORT_SERVER 8001
#define UDP_PORT_CLIENT 8002
#define MAX_DATA 200
#define PURPOSE_IP "127.0.0.1"



void* socket_udp_server(void *arg);
void* socket_udp_client(void *arg);
