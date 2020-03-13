#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "wrap.h"

#define MAXLINE 150
#define SERV_PORT 8000
#define database "/home/cbf/database/"

struct client_property{
    char client_ip[16];
    int client_status;
};

struct client_property client[FD_SETSIZE];