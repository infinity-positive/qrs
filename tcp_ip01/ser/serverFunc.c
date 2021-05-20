/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：serverFunc.c
*   创 建 者：YQQ
*   创建日期：2021年05月20日
*   描    述：
*
*   更新日志：
*
================================================================*/

#include "server.h"

//初始化
int server_init(int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family				= AF_INET;
	addr.sin_port				= htons(port);
	addr.sin_addr.s_addr		= inet_addr("0.0.0.0");
	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return -1;
	}

	if(listen(sockfd, 5) < 0)
	{
		perror("listen");
		return -1;
	}

	printf("server init succeed\n");

	return sockfd;
}

//监听客户端
int wait_for_client(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int connfd = accept(sockfd, (struct sockaddr *)&addr, &len);
	if(connfd < 0)
	{
		perror("accept");
		return -1;
	}

	printf("client: %s:%u\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	
	return connfd;
}

//操作菜单
void menu(int connfd)
{
	char buf[128] = {0};
	strcpy(buf, "-------------------------------------------------------------\n");
	send(connfd, buf, strlen(buf), 0);

	memset(buf, 0, sizeof(buf));
	strcpy(buf," 1.list  2.download filename  3.upload filename  4.quit\n");
	send(connfd, buf, strlen(buf), 0);

	memset(buf, 0, sizeof(buf));
	strcpy(buf,"-------------------------------------------------------------\n");
	send(connfd, buf, strlen(buf), 0);

	memset(buf, 0, sizeof(buf));
	strcpy(buf,"cmd >>>\n");
	send(connfd, buf, strlen(buf), 0);
}

//服务器目录列表
void list(int connfd)
{
	DIR *dir = opendir("./");
	if(dir == NULL)
	{
		perror("opendir");
		return ;
	}

	struct dirent *filestream;

	while(filestream = readdir(dir))
	{
		char *filename = filestream->d_name;
		if((strncmp(filename, ".", 1) == 0) || (strncmp(filename, "..", 2) == 0))
			continue;
		char buf[32] = {0};
		strcpy(buf, filename);
		char *buf1 = "\n";
		strcat(buf, buf1);
		write(connfd, buf, strlen(buf));
	}
}

//客户端下载数据
void download(char *filename, int connfd)
{
	int fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return ;
	}
	int length = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char buf[32] = {0};
	sprintf(buf, "len: %d", length);
	write(connfd, buf, strlen(buf));

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		int ret = read(fd, buf, sizeof(buf));
		if(ret <= 0)
		{
			break;
		}

		write(connfd, buf, ret);
	}

	close(fd);
}

//客户端上传文件
void upload(char *filename, int connfd)
{
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
	if(fd < 0)
	{
		perror("open");
		return;
	}

	char buf[128] = {0};
	int ret = recv(connfd, buf, sizeof(buf), 0);
	if(ret < 0)
	{
		perror("recv");
		return ;
	}
	else if(ret == 0)
	{
		printf("client succeed quit\n");
		close(connfd);
	}
	
	int len;
	sscanf(buf, "len: %d", &len);
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		int ret = read(connfd, buf, sizeof(buf));
		if(len)
		{
			write(fd, buf, ret);
		}
		len -= ret;
	}
	close(fd);

}
