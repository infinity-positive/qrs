/*===============================================================
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   文件名称：ser.c
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

#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>

#define MAXSIZE 128
#define PTH "/home/yu/Desktop/git/ftp/"

int ser_init(int port)
{
	int serSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serSockfd < 0)
	{
		perror("socket");
		return(-1);
	}

	struct sockaddr_in serAddr, cliAddr;
	serAddr.sin_family			= AF_INET;
	serAddr.sin_port			= htons(port);
	serAddr.sin_addr.s_addr		= inet_addr("0.0.0.0");

	if(bind(serSockfd, (struct sockaddr *)&serAddr, sizeof(serAddr)) < 0)
	{
		perror("bind");
		return(-1);
	}

	if(listen(serSockfd, 10) < 0)
	{
		perror("listen");
		return -1;
	}
	puts("server init succeed");

	return serSockfd;
}

void menu(int cliSockfd)
{
	char buf[MAXSIZE] = {0};
	strcpy(buf, "------------------------------------------------------------");
	send(cliSockfd, buf, sizeof(buf), 0);

	bzero(buf, sizeof(buf));
	strcpy(buf, "-- 1.list  2.download filename  3.upload filename  4.quit");
	send(cliSockfd, buf, sizeof(buf), 0);

	bzero(buf, sizeof(buf));
	strcpy(buf, "------------------------------------------------------------");
	send(cliSockfd, buf, sizeof(buf), 0);

	bzero(buf, sizeof(buf));
	strcpy(buf, "-- cmd >>> ");
	send(cliSockfd, buf, sizeof(buf), 0);
}

int filename_len(int cliSockfd)
{
	DIR *dir = opendir(PTH);
	if(dir == NULL)
	{
		perror("opendir");
		return -1;
	}

	struct dirent *dirstream = NULL;
	int len = 0;

	while(dirstream = readdir(dir))
	{
		if((strncmp(dirstream->d_name, ".", 1) == 0) || (strncmp(dirstream->d_name, "..", 2) == 0))
		{
			continue;
		}
		len++;
	}
	char buf[MAXSIZE] = {0};
	sprintf(buf, "len: %d", len);
	send(cliSockfd, buf, sizeof(buf), 0);

	closedir(dir);
}

int filename_list(int cliSockfd)
{
	DIR *dir = opendir(PTH);
	if(dir == NULL)
	{
		perror("opendir");
		return -1;
	}

	struct dirent *dirStream;

	while((dirStream = readdir(dir)))
	{
		char *filename = dirStream->d_name;

		if((strncmp(filename, ".", 1) == 0) || (strncmp(filename, "..", 2) == 0))
		{
			continue;
		}

		send(cliSockfd, filename, MAXSIZE, 0);
	}

	closedir(dir);
}

int download(char *filename, int cliSockfd)
{
	chdir(PTH);
	int fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		perror("opendownload");
		return -1;
	}
	int length = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char buf[MAXSIZE] = {0};
	sprintf(buf, "len: %d", length);
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

int upload(char *filename, int cliSockfd)
{
	chdir(PTH);
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
	if(fd < 0)
	{
		perror("openupload");
		return -1;
	}

	char buf[MAXSIZE] = {0};
	int len;
	recv(cliSockfd, buf, sizeof(buf), 0);
	sscanf(buf, "len: %d", &len);
	printf("%d\n", len);

	while(len)
	{
		int ret = recv(cliSockfd, buf, sizeof(buf), 0);
		write(fd, buf, ret);

		len -= ret;
	}

	close(fd);
}

int link_cli(int serSockfd)
{
	struct sockaddr_in cliAddr;
	socklen_t len = sizeof(cliAddr);
	while(1)
	{
		int cliSockfd = accept(serSockfd, (struct sockaddr *)&cliAddr, &len);
		if(cliSockfd < 0)
		{
			perror("accept");
			continue;
		}
		printf("client: %s:%u\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

		char buf[MAXSIZE] = {0};
		while(1)
		{
			menu(cliSockfd);
			int ret = recv(cliSockfd, buf, sizeof(buf), 0);
			if(ret < 0)
			{
				perror("recv");
				close(cliSockfd);
				break;
			}
			else if(ret == 0)
			{
				printf("client quit\n");
				close(cliSockfd);
				break;
			}
			else
			{
				printf("recv: %s\n", buf);
				char filename[MAXSIZE] = {0};
				if(strstr(buf, "list") != NULL)
				{
					filename_len(cliSockfd);
					filename_list(cliSockfd);
				}
				else if(strstr(buf, "download ") != NULL)
				{
					bzero(filename, sizeof(filename));
					sscanf(buf, "download %s", filename);
					download(filename, cliSockfd);
					printf("download finished\n");
				}
				else if(strstr(buf, "upload ") != NULL)
				{
					bzero(filename, sizeof(filename));
					sscanf(buf, "upload %s", filename);
					upload(filename, cliSockfd);
					puts("upload finished");
				}
				else if(strstr(buf, "quit") != NULL)
				{
					puts("client quit");
					close(cliSockfd);
					break;
				}
				else
				{
					char buf1[MAXSIZE] = {0};
					strcpy(buf1, "please input according to menu");
					send(cliSockfd, buf1, sizeof(buf1), 0);
					continue;
				}
			}
		}
	}

}


int main(int argc, char * argv[])
{
	if(argc < 2)
	{
		printf("<port>\n");
		return(-1);
	}

	int serSockfd = ser_init(atoi(argv[1]));
	if(serSockfd < 0)
	{
		return -1;
	}

	link_cli(serSockfd);

	return 0;
}
