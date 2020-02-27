#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include "wrap.h"

#define UDP_PORT 8001
#define MAX_DATA 200

void* socket_udp_server(void *arg);
