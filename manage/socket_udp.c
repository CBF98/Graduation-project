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
	servaddr.sin_port = htons(UDP_PORT);
	
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
		n = sendto(sockfd, buf, 6, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		if(n == -1)
			perr_exit("sendto error");
	}
}
		
