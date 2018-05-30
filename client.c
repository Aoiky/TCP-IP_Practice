#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc,char* argv[])
{
	//�����׽���
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		perror("socket");
		exit(-1);
	}

	int err_log = 0;
	unsigned short port = 8000;
	char server_ip[INET_ADDRSTRLEN] = "192.168.43.86";
	struct sockaddr_in server_addr;
	if (argc>1)
	{
		strcpy(server_ip, argv[1]);
	}

	// ��ʼ����������ַ
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET,server_ip,&server_addr.sin_addr);

	//connect���������������������ӣ����ӽ����ɹ���ſ��Կ�ʼ�������ݣ�����TCPЭ�飩
	err_log = connect(sockfd,(struct sockaddr*)&server_addr, sizeof(server_addr));      // �������ӷ�����  
	if (err_log != 0)
	{
		perror("connect");
		close(sockfd);
		exit(-1);
	}
	printf("ready send data to TCP server %s:%d!\n", server_ip, port);
	while(1)
	{
		char send_buf[512] = "";

		printf("Enter your message:");
		fgets(send_buf,sizeof(send_buf),stdin);
		send_buf[strlen(send_buf)-1] = '\0';

		write(sockfd,send_buf,strlen(send_buf));
		read(sockfd,send_buf,sizeof(send_buf));
		printf("recv:%s\n",send_buf);
	}

	close(sockfd);
	return 0;
}