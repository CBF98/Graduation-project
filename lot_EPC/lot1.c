#include "lot1.h"
#include "HextoTwo.h"
#include "StringtoHex.h"

int main()
{
	char id_own[25] = {0};
	long int id_int[24] = {0};
	strcat(id_own, ID);	//the string of the lot's ID
	StringtoHex(id_own, id_int, 24);	//represent id as a hexadecimal number
	// int id_int[24]={0X1,0X1,0X0,0X0,0X0,0X0,0X0,0X0,0X1,0X0,0X0,0X0,0X0,0X0,0X1,0XE,0XF,0X0,0X2,0X3,0XF,0XF,0XE,0XA};
	char id_char[13];
	memset(id_char, 0, sizeof(id_char));
	HextoTwo(id_int, 24, id_char);	//represent id by every bits
	printf("%d\n",(int)strlen(id_char));
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
	int fd = open(FIFO_sign,O_WRONLY);
	if(fd < 0)
	{
		perror("open error\n");
		return -1;
	}	//open sign fifo

	char sign[100]={0};
	char name_write[200]={0};
	char name_read[200]={0};
	strcat(sign, "0");	//type of the ID
	strcat(sign, id_char);
	write(fd, sign, strlen(sign));
	printf("%s\n", sign);

	strcat(name_write, FIFO);
	strcat(name_write, id_char);
	strcat(name_write, "_write.fifo");
	//the path of the write fifo

	strcat(name_read, FIFO);
	strcat(name_read, id_char);
	strcat(name_read, "_read.fifo");
	//the path of the read fifo

	sleep(3);
	//finished sign

	umask(0);
	if(mkfifo(name_read,0664)<0)
	{
		if(errno == EEXIST)
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
	
	char buff[1024]={0};

	int fd_read = open(name_read,O_RDONLY|O_NONBLOCK);
	int fd_write = open(name_write,O_WRONLY);

	while(1)
	{
		int n;
		if(fd_read < 0)
		{
			perror("open read");
			exit(1);
		}
		n = read(fd_read, buff, 1024); //or 1023
		if(n < 0)
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
		memset(buff, 0, sizeof(buff));	
		sleep(2);
		strcat(buff, "0");
		strcat(buff, id_char);
		strcat(buff, "0");
		strcat(buff, id_char);
		printf("%s\n",buff);
		// scanf("%s",buff);
		write(fd_write, buff, strlen(buff));
		sleep(2);
		//when the lot want to offline,it can close the fd_write and fd_read
		//close(fd_write)
		//close(fd_read)
	}
	
	return 0;
}
