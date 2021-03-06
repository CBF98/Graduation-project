#include "manage.h"
#include "socket_tcp.h"
#include "socket_udp.h"
#include "wrap.h"
#include "data_processing.h"
#include "pcap_send.h"

int main()
{
	char server_ip[20]={0};
	char buff[MAXLINE] = {0};			//receive from fifo and socket 
	char send[MAXLINE] = {0};	
	char message[MAXLINE] = {0};		//write the date which send to server
	int i = 0, j = 0, thread_quantity = 0;
	char des_id[20] = {0};
	char sour_id[20] = {0};
	memset(Thread_status, 0, sizeof(Thread_status));
	
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

	int fd_sign, n=0, err;
	int quantity = 0;
	int socket_status = 0;
	int match_status = 0;

	fd_sign = open(FIFO_sign,O_RDONLY|O_NONBLOCK);

	if(fd_sign<0)
	{
		perror("open error\n");
		return -1;
	}	//open sign fifo
	

	
	err = pthread_create(&ntid[0], NULL, socket_udp_server, NULL);	//udp_server
	if (err != 0)
	{
		fprintf(stderr, "can't create thread: %s\n", strerror(err));
		exit(1);
	}
	Thread_status[0] = 1;
	
	while(1)
	{
		memset(buff, 0, sizeof(buff));
		n = read(fd_sign, buff, MAXLINE);
		if(n < 0)
		{	// there is no lot sign up
			if (errno == EAGAIN)
			{
				goto communication;
			}	//continue
			perror("read fd_sign");
			exit(1);
		}
		if(n == 0)
		{
			goto communication;
		}
		
		//sign up
		memset(server_ip, 0, sizeof(server_ip));
		strcat(server_ip, SERVER_IP);
		
		memset(message, 0, sizeof(message));
		message[0] = '0';
		strcat(message, buff);
		
		n = socket_tcp(message, server_ip);
		
		if(n == 0)
		{//success to sign up
			make_lot(lot, quantity, buff);	//after success making lot,quantity++
			quantity++;
		}
		else
		{//fail to sign up
			printf("%s sign error\n", &buff[1]);
		}
		
communication:
		for(i=0;i<quantity;i++)
		{
			
			memset(buff, 0, sizeof(buff));
			memset(des_id, 0, sizeof(des_id));
			memset(sour_id, 0, sizeof(sour_id));
			
			n = read(lot[i].fd_read, buff, MAXLINE);
			
			if(n < 0)
			{
				if (errno == EAGAIN)
				{
				}
				else
				{
					perror("read error in lot");
					exit(1);
				}
			}
			
			else if(n == 0)
			{
				//the lot is offline
				memset(server_ip, 0, sizeof(server_ip));
				strcat(server_ip, SERVER_IP);
				memset(message, 0, sizeof(message));
				message[0] = '1';
				message[1] = lot[i].lot_type;
				strcat(message, lot[i].lot_id);
				socket_status = socket_tcp(message, server_ip);
				if(socket_status == 0)
				{//success offline
					printf("the %s is offline now\n",lot[i].lot_id);
					rearrange(quantity, i, lot);
					quantity--;
					for(int k = 0; k < quantity; k++)
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
				printf("%s\n", buff);
				obtain_lot_id(buff, des_id, sour_id);
				match_status = match_lot(des_id, lot, quantity);
				printf("%s\n",des_id);
				printf("%s\n",sour_id);
				printf("%s\n",lot[i].lot_id);
				printf("%d\n",match_status);
				if(match_status != -1)
				{	//the lot is belong the manage
					printf("success send!\n");
					memset(buff, 0, sizeof(buff));
					strcat(buff, "hello");
					write(lot[match_status].fd_write, buff, strlen(buff));
				}

				else
				{
					pcap_send(buff, NULL, NULL, 1);
				}

			/*	else
				{	//the lot is not belong the manage
				thread_next:
				printf("what happened?\n");
				sleep(2);
					for(thread_quantity = 1; thread_quantity < MAX_THREAD; thread_quantity++)
					{
						if(Thread_status[thread_quantity] == 0)
						{
							memset(thread_arg[thread_quantity].send, 0, sizeof(send));
							strcat(thread_arg[thread_quantity].send, buff);
							thread_arg[thread_quantity].quantity = thread_quantity;
							err = pthread_create(&ntid[thread_quantity], NULL, socket_udp_client, &(thread_arg[thread_quantity]));
							if (err != 0)
							{
								fprintf(stderr, "can't create thread: %s\n", strerror(err));
								exit(1);
							}
							break;
						}
					}
					goto thread_next;
					
				}
			*/
			}
		}
	}
}



void make_lot(struct lot_Attributes lot[],int quantity,char *lot_id)
{ //lot_id[0] is the type the lot, others is lot's id
	int n = 0;
	lot[quantity].lot_type = lot_id[0];
	strcat(lot[quantity].lot_id, &lot_id[1]);
	
	strcat(lot[quantity].position_write, FIFO);
	strcat(lot[quantity].position_write, &lot_id[1]);
	strcat(lot[quantity].position_write, "_write.fifo");
	
	strcat(lot[quantity].position_read, FIFO);
	strcat(lot[quantity].position_read, &lot_id[1]);
	strcat(lot[quantity].position_read, "_read.fifo");
	
	n = make_fifo(lot[quantity].position_write, lot[quantity].position_read);
	if(n == -1){printf("make fifo error!\n");}
	
	n = make_fd(&lot[quantity].fd_write, lot[quantity].position_read,1);
	if(n == -1){printf("make fd error!\n");}
	
	n = make_fd(&lot[quantity].fd_read, lot[quantity].position_write,0);
	if(n == -1){printf("make fd error!\n");}
	
}

int make_fifo(char* fifo_name_write,char* fifo_name_read)
{	//it can make a fifo named fifo_name at the FIFO(#define) with way of type
	//the write and read is for lot,so it is opposite in server
	umask(0);
	if(mkfifo(fifo_name_write,0664) < 0)
	{
		if(errno == EEXIST)
		{
		}
		else
		{
			perror("fifo_name_write error\n");
			return -1;
		}
	}
	umask(0);
	if(mkfifo(fifo_name_read,0664) < 0)
	{
		if(errno == EEXIST)
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
	int i = position;
	for(; i < quantity-1; i++)
	{
		lot[i] = lot[i+1];
	}
	memset(&lot[i], 0, sizeof(struct lot_Attributes));
}

int match_lot(char* buff, struct lot_Attributes lot[], int quantity)
{
	int i;
	for(i = 0; i < quantity; i++)
	{
		if(buff[0] == lot[i].lot_type)
		{
			printf("look here!\n");
			if(strcmp(&buff[1], lot[i].lot_id) == 0)
			{
				printf("here!\n");
				return i;
			}
		}
	}
	return -1;
}
