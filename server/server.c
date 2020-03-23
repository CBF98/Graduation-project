#include "server.h"
#include "wrap.h"
#include "data_processing.h"

int main(int argc, char **argv)
{

    if( NULL == mysql_init(&mysql_EPC) ){
        printf("error! %s\n", mysql_error(&mysql_EPC));
        return -1;
    }

    if( NULL == mysql_init(&mysql_Ecode) ){
        printf("error! %s\n", mysql_error(&mysql_Ecode));
        return -1;
    }

    if ( NULL == mysql_real_connect(&mysql_EPC, "localhost", "root", "hengheng", "test1", 0, NULL, 0)){
        printf("connect error ! %s", mysql_error(&mysql_EPC));
        return -1;
    }
    printf("success epc connect!\n");

    if ( NULL == mysql_real_connect(&mysql_Ecode, "localhost", "root", "hengheng", "test2", 0, NULL, 0)){
        printf("connect error ! %s", mysql_error(&mysql_Ecode));
        return -1;
    }
    printf("success Ecode connect!\n");

	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready;
	ssize_t n;
	fd_set rset, allset;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	socklen_t cliaddr_len;
	struct sockaddr_in cliaddr, servaddr;
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	bzero(&cliaddr,sizeof(cliaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	Listen(listenfd, 20);
	
	maxfd = listenfd;
	maxi = -1;
	memset(client, 0, sizeof(client));
	for (i = 0; i < FD_SETSIZE; i++)
		client[i].client_status = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	
	for( ; ; )
	{
		rset = allset;
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if (nready < 0)
			perr_exit("select error");
			
		if (FD_ISSET(listenfd, &rset))
		{
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
			
			printf("received from %s at PORT %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),ntohs(cliaddr.sin_port));
			
			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i].client_status < 0) {
					client[i].client_status = connfd;
					strcpy(client[i].client_ip, inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)));
					break;
				}
			
			if (i == FD_SETSIZE) {
				fputs("too many clients\n", stderr);
				exit(1);
			}
			
			FD_SET(connfd, &allset);
			
			if (connfd > maxfd)
				maxfd = connfd;
			if (i > maxi)
				maxi = i;
			if (--nready == 0)
				continue;
		}
		
		for (i = 0; i <= maxi; i++)
		{
			if ( (sockfd = client[i].client_status) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset))
			{
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0)
				{
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i].client_status = -1;
					memset(client[i].client_ip, 0, sizeof(client[i].client_ip));
					printf("offline!\n");
				}
				else
				{//process the data
                    Extract_data(i, buf);
                    write(connfd, "hello", n);
				}
				
				if (--nready == 0)
					break;
			}
		}
	}
}
					
