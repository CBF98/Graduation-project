#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "socket_tcp.h"

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

void make_lot(struct lot_Attributes lot[],int quantity,char *lot_id);
int make_fifo(char* fifo_name_write,char* fifo_name_read);
int make_fd(int *fd,char* name,int type);
void rearrange(int quantity,int position,struct lot_Attributes lot[]);


int main()
{
	char server_ip[20]={0};
	umask(0);
	if(mkfifo(FIFO_sign,0664)<0)
	{
		if(errno==EEXIST)
		{
		}
		else
		{
			perror("mkfifo error\n");
			return -1;
		}
	}
	int fd_sign,n=0;
	struct lot_Attributes lot[20];
	int quantity=0;
	fd_sign=open(FIFO_sign,O_RDONLY|O_NONBLOCK);

	if(fd_sign<0)
	{
		perror("open error\n");
		return -1;
	}	//open sign fifo
	
	char buff[MAXLINE]={0};			//receive from fifo and socket 
	char message[MAXLINE]={0};		//write the date which send to server
	int i=0;
	
	while(1)
	{
		memset(buff,0,sizeof(buff));
		n=read(fd_sign,buff,MAXLINE);
		if(n<0)
		{	// there is no lot sign up
			if (errno == EAGAIN)
			{
				goto communication;
			}	//continue
			perror("read fd_sign");
			exit(1);
		}
		if(n==0)
		{
			goto communication;
		}
		
		//sign up
		memset(server_ip,0,sizeof(server_ip));
		strcat(server_ip,SERVER_IP);
		memset(message,0,sizeof(message));
		strcat(message,"0");
		strcat(message,&buff[1]);
		n=socket_tcp(message,server_ip);
		if(n==0)
		{//success to sign up
			make_lot(lot,quantity,&buff[1]);	//after success making lot,quantity++
			quantity++;
		}
		else
		{//fail to sign up
			printf("%s sign error\n",&buff[1]);
		}
		
communication:
		for(i=0;i<quantity;i++)
		{
			n=read(lot[i].fd_read,buff,MAXLINE);
			if(n<0)
			{
				if (errno == EAGAIN)
				{
				}
				else
				{
					perror("read error in lot");
					exit(1);
					/*/the lot is offline
					//send the offline date to switch
					printf("the %s is offline\n",lot[i].lot_id);
					rearrange(quantity,i,lot);
					quantity--;*/
				}
			}
			else if(n==0)
			{
				//the lot is offline
				memset(server_ip,0,sizeof(server_ip));
				strcat(server_ip,SERVER_IP);
				memset(message,0,sizeof(message));
				strcat(message,"1");
				strcat(message,lot[i].lot_id);
				n=socket_tcp(message,server_ip);
				if(n==0)
				{//success offline
					printf("the %s is offline now\n",lot[i].lot_id);
					rearrange(quantity,i,lot);
					quantity--;
					for(int k=0;k<quantity;k++)
					{
						printf("%d   %s\n",k,lot[i].lot_id);
					}
				}
				else
				{//fail offline
					printf("fail offline!\n");
				}
			}
			else
			{
				//process the data
				printf("%s\n",buff);
			}
		}
	}
}



void make_lot(struct lot_Attributes lot[],int quantity,char *lot_id)
{
	int n=0;
	strcat(lot[quantity].lot_id,lot_id);
	
	strcat(lot[quantity].position_write,FIFO);
	strcat(lot[quantity].position_write,lot_id);
	strcat(lot[quantity].position_write,"_write.fifo");
	
	strcat(lot[quantity].position_read,FIFO);
	strcat(lot[quantity].position_read,lot_id);
	strcat(lot[quantity].position_read,"_read.fifo");
	
	n=make_fifo(lot[quantity].position_write,lot[quantity].position_read);
	if(n==-1){printf("make fifo error!\n");}
	
	n=make_fd(&lot[quantity].fd_write,lot[quantity].position_read,1);
	if(n==-1){printf("make fd error!\n");}
	
	n=make_fd(&lot[quantity].fd_read,lot[quantity].position_write,0);
	if(n==-1){printf("make fd error!\n");}
	
}

int make_fifo(char* fifo_name_write,char* fifo_name_read)
{	//it can make a fifo named fifo_name at the FIFO(#define) with way of type
	//the write and read is for lot,so it is opposite in server
	umask(0);
	if(mkfifo(fifo_name_write,0664)<0)
	{
		if(errno==EEXIST)
		{
		}
		else
		{
			perror("fifo_name_write error\n");
			return -1;
		}
	}
	umask(0);
	if(mkfifo(fifo_name_read,0664)<0)
	{
		if(errno==EEXIST)
		{
		}
		else
		{
			perror("fifo_name_read error\n");
			return -1;
		}
	}
	return 0;
}

int make_fd(int *fd,char* name,int type)
{
	//open the fd which named name in this type
	//type==1 => write  type==0 => read
	char position[300]={0};
	strcat(position,name);
	if(type == 0)
	{
		fd[0] = open(position,O_RDONLY|O_NONBLOCK);
		if(fd[0]<0)
		{
			perror("open error read\n");
			return -1;
		}
	}
	else if(type == 1)
	{
		fd[0] = open(position,O_WRONLY);
		if(fd[0]<0)
		{
			perror("open error read\n");
			return -1;
		}
	}
	else
	{
		perror("type error!\n");
		return -1;
	}
	return 0;
}

void rearrange(int quantity,int position,struct lot_Attributes lot[])
{
	int i=position;
	for(;i<quantity-1;i++)
	{
		lot[i]=lot[i+1];
	}
	memset(&lot[i],0,sizeof(struct lot_Attributes));
}
