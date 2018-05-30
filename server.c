#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc,char *argv[])
{
	//创建套接字
	int sockfd = 0;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		perror("socket");
		exit(-1);
	}

	//bind：将本地协议地址与sockfd绑定
	int err_log = 0;
	struct sockaddr_in my_addr;
	unsigned short port = 8000;

	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if(err_log!=0)
	{
		perror("binding");
		close(sockfd);
		exit(-1);
	}

	//listen：将套接字由主动修改为被动
	err_log = listen(sockfd, 10);
	if(err_log!=0)
	{
		perror("listen");
		close(sockfd);
		exit(-1);
	}
	printf("waiting for accept...\n");

	//accept：从已连接队列中取出一个已经建立的连接，如果没有任何连接可用，则进入睡眠等待
	while(1)
	{
		struct sockaddr_in client_addr;
		char recv_buf[512] = "";
		char cli_ip[INET_ADDRSTRLEN] = "";
		socklen_t client_addr_len = sizeof(client_addr);

		int connfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_addr_len);
		if(connfd<0)
		{
			perror("accept");
			close(sockfd);
			exit(-1);
		}

		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,cli_ip,INET_ADDRSTRLEN);
		read(connfd, recv_buf, sizeof(recv_buf));
		printf("cli_ip:%s\tport=%d\tmessage:%s\n", cli_ip, ntohs(client_addr.sin_port), recv_buf);

		write(connfd, recv_buf, strlen(recv_buf));
		
		close(connfd);//关闭已连接套接字
	}
	close(sockfd); //关闭监听套接字
	return 0;
}