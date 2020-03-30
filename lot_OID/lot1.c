#include "lot1.h"
#include "HextoTwo.h"
#include "StringtoHex.h"

int main()
{
    //because the Ecode coding will be two consecutive 0 ,so i use the 10 to represent 0
	char id_own[34] = {0};
	strcat(id_own, ID);	//the string of the lot's ID

	unsigned char id_char[10];
	memset(id_char, 0, sizeof(id_char));
	Divide_OID(id_own, id_char);	//represent id by every bits

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
	strcat(sign, lot_type);	//type of the ID
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
		strcat(buff, lot_type);
		strcat(buff, id_char);
		strcat(buff, lot_type);
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
