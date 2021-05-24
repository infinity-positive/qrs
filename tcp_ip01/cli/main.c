/*===============================================================
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   文件名称：main.c
 *   创 建 者：YQQ
 *   创建日期：2021年05月20日
 *   描    述：
 *
 *   更新日志：
 *
 ================================================================*/

#include "client.h"

int main(int argc, char * argv[])
{
	if(argc < 3)
	{
		printf("need three arguements\n");
		return -1;
	}

	int sockfd = client_init(argv[1], atoi(argv[2]));
	if(sockfd < 0)
	{
		return -1;
	}

	char filename[32] = {0};

	while(1)
	{
		/*获取服务器menu*/
		menu(sockfd);

		char buf[MAXSIZE] = {0};

		//向服务器输入指令
		printf("input: ");
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf) - 1] = '\0';
		send(sockfd, buf, sizeof(buf), 0);

		if(strncmp(buf, "list", 4) == 0)
		{
			listFileName(sockfd);
		}
		else if(strstr(buf, "download") != NULL)
		{
			sscanf(buf, "download %s", filename);
			printf("%s\n", filename);
			download(filename, sockfd);
		}
		else if(strstr(buf, "upload") != NULL)
		{
			sscanf(buf, "upload %s", filename);
			upload(filename, sockfd);
		}
		else if(strstr(buf, "quit") < 0)
		{
			break;
		}
		else
		{
			bzero(buf, sizeof(buf));
			recv(sockfd, buf, sizeof(buf), 0);
			fputs(buf, stdout);
		}

	}
	close(sockfd);

	return 0;
}
