#include "socket_udp.h"

void* socket_udp_server(void *arg)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int sockfd;
	char buf[MAX_DATA];
	char str[INET_ADDRSTRLEN]; //INET_ADDRSTRLEN = 16 in IPv4, = 46 in IPv6
	int i, n;
	
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(UDP_PORT_SERVER);
	
	Bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	printf("Accepting connections ...\n");
	while (1)
	{
		cliaddr_len = sizeof(cliaddr);
		n = recvfrom(sockfd, buf, MAX_DATA, 0, (struct sockaddr *)&cliaddr, &cliaddr_len);
		if (n == -1)
			perr_exit("recvfrom error");
		printf("received from %s at PORT %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),ntohs(cliaddr.sin_port));
		//process the data
		printf("%s\n", buf);
		memset(buf, 0, sizeof(buf));
		strcat(buf, "hello");
		n = sendto(sockfd, buf, MAX_DATA, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		if(n == -1)
			perr_exit("sendto error");
		memset(buf, 0, sizeof(buf));
	}
}
		
void* socket_udp_client(void *arg)
{
	struct sockaddr_in servaddr;
	int sockfd, n;
	char buf[MAX_DATA] = {0};
	strcat(buf, arg);
	char str[INET_ADDRSTRLEN];
	socklen_t servaddr_len;
	
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, PURPOSE_IP, &servaddr.sin_addr);  //the PURPOSE_IP is the purpose ip
	servaddr.sin_port = htons(UDP_PORT_SERVER);
	
	n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (n == -1)
		perr_exit("sendto error");
	n = recvfrom(sockfd, buf, MAX_DATA, 0, NULL, 0);
	if (n == -1)
		perr_exit("recvfrom error");
	//process the received data
	printf("%s\n",buf);
}
