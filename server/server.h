#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <mysql.h>
#include <errmsg.h>

#define MAXLINE 150
#define SERV_PORT 8000
#define database "/home/cbf/database/"

MYSQL mysql_EPC;
MYSQL mysql_Ecode;
MYSQL mysql_OID;

struct client_property{
    char client_ip[16];
    int client_status;
};

struct client_property client[FD_SETSIZE];