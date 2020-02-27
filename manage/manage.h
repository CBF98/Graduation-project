#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "socket_tcp.h"
#include "socket_udp.h"
#include "wrap.h"

#define FIFO "/home/cbf/lot_server/fifo/"
#define FIFO_sign "/home/cbf/lot_server/fifo/sign.fifo"
#define SERVER_IP "127.0.0.1"

struct lot_Attributes{
	int fd_read;
	int fd_write;
	char lot_id[100];
	char position_write[200];
	char position_read[200];
};

pthread_t ntid;

void make_lot(struct lot_Attributes lot[],int quantity,char *lot_id);
int make_fifo(char* fifo_name_write,char* fifo_name_read);
int make_fd(int *fd,char* name,int type);
void rearrange(int quantity,int position,struct lot_Attributes lot[]);
