#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 200
#define SERV_PORT 8000

int socket_tcp(char message[],char ip_server[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd, n;
	char *str;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip_server, &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
	
	n=connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	write(sockfd, message, strlen(message));
	
	n = read(sockfd, buf, MAXLINE);
	
	printf("Response from server:\n");
	write(STDOUT_FILENO, buf, n);
	printf("\n");
	close(sockfd);
	return 0;
}
