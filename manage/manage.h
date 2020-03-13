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



#define FIFO "/home/cbf/fifo/"
#define FIFO_sign "/home/cbf/fifo/sign.fifo"
#define SERVER_IP "127.0.0.1"
#define MAX_THREAD 4
#define MAXLINE 200

struct lot_Attributes{
	int fd_read;
	int fd_write;
	char lot_type;
	char lot_id[100];
	char position_write[200];
	char position_read[200];
};

struct lot_Attributes lot[20];

struct Thread_data{
	int quantity;
	char send[MAXLINE];
};

struct Thread_data thread_arg[MAX_THREAD];

int Thread_status[MAX_THREAD];

pthread_t ntid[MAX_THREAD];

void make_lot(struct lot_Attributes lot[],int quantity,char *lot_id);
int make_fifo(char* fifo_name_write,char* fifo_name_read);
int make_fd(int *fd,char* name,int type);
void rearrange(int quantity,int position,struct lot_Attributes lot[]);
int match_lot(char* buff, struct lot_Attributes lot[], int quantity);
