#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#define ID "client2"
#define FIFO "/home/cbf/lot_server/fifo/"
#define FIFO_sign "/home/cbf/lot_server/fifo/sign.fifo"

int main()
{
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
	int fd=open(FIFO_sign,O_WRONLY);
	if(fd<0)
	{
		perror("open error\n");
		return -1;
	}	//open sign fifo

	char sign[100]={0};
	char name_write[200]={0};
	char name_read[200]={0};
	strcat(sign,"0");	//type of the ID
	strcat(sign,ID);
	write(fd,sign,strlen(sign));
	printf("%s\n",sign);

	strcat(name_write,FIFO);
	strcat(name_write,ID);
	strcat(name_write,"_write.fifo");
	//the path of the write fifo

	strcat(name_read,FIFO);
	strcat(name_read,ID);
	strcat(name_read,"_read.fifo");
	//the path of the read fifo

	sleep(1);
	//finished sign

	umask(0);
	if(mkfifo(name_read,0664)<0)
	{
		if(errno==EEXIST)
		{
		
		}
		else
		{
			perror("sign error\n");
			return -1;
		}
	}
	
	printf("success:login!\n");
	//delete[] login;
	close(fd);

	while(1)
	{
		int n;
		char buff[1024]={0};

		int fd_read=open(name_read,O_RDONLY|O_NONBLOCK);
		int fd_write=open(name_write,O_WRONLY);
		if(fd_read<0)
		{
			perror("open read");
			exit(1);
		}
		n=read(fd_read,buff,1024); //or 1023
		if(n<0)
		{
			if (errno == EAGAIN)
			{
				goto next_step;// there is no data
			}	
			perror("read wrong!");
			exit(1);
		}
		else
		{
			//process the data
		}
next_step:
		//determine when to write		
		scanf("%s",buff);
		write(fd_write,buff,strlen(buff));
		
		//when the lot want to offline,it can close the fd_write and fd_read
		//close(fd_write)
		//close(fd_read)
	}
	
	return 0;
}
