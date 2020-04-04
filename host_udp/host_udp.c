#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#define UDP_PORT_SERVER 8000
#define HOST_IP "192.168.109.99"
#define SERVER_IP "6.6.6.6"
#define MAX_DATA 200

void socket_udp_client(char* ip, char* buff);

int main()
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int sockfd;
	char buf[MAX_DATA];
    char ip[16];
	char str[INET_ADDRSTRLEN]; //INET_ADDRSTRLEN = 16 in IPv4, = 46 in IPv6
	int i, n;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        printf("error in socket!\n");
        return 0;
    }
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(UDP_PORT_SERVER);
	
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        printf("error in bind!\n");
        return 0;
    }
	
	printf("Accepting connections ...\n");
	while (1)
	{
		cliaddr_len = sizeof(cliaddr);
		n = recvfrom(sockfd, buf, MAX_DATA, 0, (struct sockaddr *)&cliaddr, &cliaddr_len);
		if (n == -1)
			printf("recvfrom error");
		printf("received from %s at PORT %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),ntohs(cliaddr.sin_port));
		//process the data
		printf("%s\n", buf);
        memset(ip, 0, sizeof(ip));
        strcpy(ip, (char*)inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)));
        if(strcmp(ip, HOST_IP) != 0){
		    socket_udp_client(HOST_IP, buf);
		    memset(buf, 0, sizeof(buf));
        }
        else{
            socket_udp_client(SERVER_IP, buf);
            memset(buf, 0, sizeof(buf));
        }
		
	}
    return 0;
}

void socket_udp_client(char* ip, char* buff)
{
	
	struct Thread_data *my_data;
	struct sockaddr_in servaddr;
	int sockfd, n;
	char buf[MAX_DATA];
	
    strcpy(buf, buff);
	
	char str[INET_ADDRSTRLEN];
	socklen_t servaddr_len;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        printf("error in socket\n");
    }
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &servaddr.sin_addr);  //the PURPOSE_IP is the purpose ip
	servaddr.sin_port = htons(UDP_PORT_SERVER);
	
	n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (n == -1)
		printf("sendto error");
}
