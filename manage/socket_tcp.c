#include "socket_tcp.h"
int socket_tcp(char message[],char ip_server[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd, n;
	char *str;
	
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip_server, &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
	
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	Write(sockfd, message, strlen(message));
	
	n = Read(sockfd, buf, MAXLINE);
	
	printf("Response from server:\n");
	write(STDOUT_FILENO, buf, n);
	printf("\n");
	Close(sockfd);
	return 0;
}
