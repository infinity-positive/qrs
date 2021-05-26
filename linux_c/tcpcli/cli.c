/*===============================================================
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   文件名称：cli.c
 *   创 建 者：YQQ
 *   创建日期：2021年05月25日
 *   描    述：
 *
 *   更新日志：
 *
 ================================================================*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>

#define MAXSIZE 128

int cli_init(char *ip, int port)
{
	int cliSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(cliSockfd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in serAddr;
	serAddr.sin_family				= AF_INET;
	serAddr.sin_port				= htons(port);
	serAddr.sin_addr.s_addr			= inet_addr(ip);

	if(connect(cliSockfd, (struct sockaddr *)&serAddr, sizeof(serAddr)) < 0)
	{
		perror("connect");
		return -1;
	}

	return cliSockfd;
}


void get_menu(int cliSockfd)
{
	char buf[MAXSIZE] = {0};
	for(int i = 0; i < 4; i++)
	{
		bzero(buf, sizeof(buf));
		recv(cliSockfd, buf, sizeof(buf), 0);
		puts(buf);
	}
	/*
	   bzero(buf, sizeof(buf));
	   recv(cliSockfd, buf, sizeof(buf), 0);
	   puts(buf);

	   bzero(buf, sizeof(buf));
	   recv(cliSockfd, buf, sizeof(buf), 0);
	   puts(buf);

	   bzero(buf, sizeof(buf));
	   recv(cliSockfd, buf, sizeof(buf), 0);
	   puts(buf);
	   */
}

void filename_list(int cliSockfd)
{
	char buf[MAXSIZE] = {0};
	recv(cliSockfd, buf, sizeof(buf), 0);
	puts(buf);
}

int download(int cliSockfd, char *filename)
{
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
	if(fd < 0)
	{
		perror("opendownload");
		exit(-1);
	}
	
	char buf[MAXSIZE] = {0};
	recv(cliSockfd, buf, sizeof(buf), 0);
	int len;
	sscanf(buf, "len: %d", &len);
	printf("%d\n", len);

	while(1)
	{
		bzero(buf, sizeof(buf));
		int ret = recv(cliSockfd, buf, sizeof(buf), 0);
//		printf("%d\n", ret);
		if(ret < len)
		{
			write(fd, buf, ret);
		}
		else
		{
			write(fd, buf, len);
			break;
		}

		len -= ret;
	}
	close(fd);
}

int upload(int cliSockfd, char *filename)
{
	int fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		perror("openupload");
		return -1;
	}

	int len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	char buf[MAXSIZE] = {0};
	sprintf(buf, "len: %d", len);
	send(cliSockfd, buf, sizeof(buf), 0);

	while(1)
	{
		bzero(buf, sizeof(buf));
		int ret = read(fd, buf, sizeof(buf));
		if(ret == 0)
		{
			break;
		}
		send(cliSockfd, buf, ret, 0);
	}
	close(fd);
}

int link_ser(int cliSockfd)
{
	while(1)
	{
		get_menu(cliSockfd);

		char buf[MAXSIZE] = {0};
		char filename[MAXSIZE] = {0};

		printf("\ninput: ");
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf) -1] = '\0';
		send(cliSockfd, buf, sizeof(buf), 0);

		if(strstr(buf, "list") != NULL)
		{
			filename_list(cliSockfd);
			puts("");
		}
		else if(strstr(buf, "download ") != NULL)
		{
			sscanf(buf, "download %s", filename);
			download(cliSockfd, filename);
			puts("download finished");
		}
		else if(strstr(buf, "upload ") != NULL)
		{
			sscanf(buf, "upload %s", filename);
			upload(cliSockfd, filename);
			puts("upload finished");
		}
		else if(strstr(buf, "quit") != NULL)
		{
			puts("client quit");
			break;
		}
		else
		{
			bzero(buf, sizeof(buf));
			recv(cliSockfd, buf, sizeof(buf), 0);
			puts(buf);
		}
	}

}


int main(int argc, char * argv[])
{
	if(argc < 3)
	{
		puts("<ip>:<port>");
		return -1;
	}

	int cliSockfd = cli_init(argv[1], atoi(argv[2]));
	if(cliSockfd < 0)
	{
		return -1;
	}

	link_ser(cliSockfd);

	return 0;
}
