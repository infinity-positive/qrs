/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：ioClass.cpp
*   创 建 者：YQQ
*   创建日期：2021年05月27日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

using namespace std;

class File{
	private:
		const char *filename;
		unsigned int size;

	public:
		int fd;
		//构造函数
		File(const char *filename, unsigned int size = 32)
		{
			this->size = size;
			this->fd = -1;
			this->filename = new char[size];
			this->filename = filename;
		}
		//析构函数
		File()
		{

		}
	public:
		int openfile(int flags);
		int openfile(int flags, mode_t mode);
		int readfile(char *buf, int length);
		int writefile(char *buf, int length);
		int lseekfile(int offset, int flags);
		void closefile();
};

int File::openfile(int flags)
{
	fd = open(filename, flags);
	return fd;
}

int File::openfile(int flags, mode_t mode)
{
	fd = open(filename, flags, mode);
	return fd;
}

int File::readfile(char *buf, int length)
{
	return read(fd, buf, length);
}

int File::writefile(char * buf, int length)
{
	return write(fd, buf, length);
}

int File::lseekfile(int offset, int flags)
{
	return lseek(fd, offset, flags);
}

void File::closefile()
{
	close(fd);
}


int main(int argc, char * argv[])
{
	File f("1.txt"), d("2.txt");
	f.fd = f.openfile(O_RDONLY);
	d.fd = d.openfile(O_WRONLY | O_CREAT | O_TRUNC, 0640);
	if(f.fd < 0 || d.fd < 0)
	{
		perror("open");
		return -1;
	}

	char buf[128] = {0};
	while(1)
	{
		memset(buf, 0, sizeof(buf));
	int ret = f.readfile(buf, sizeof(buf));
	if(ret == 0)
		break;
	d.writefile(buf, ret);
	}

	f.closefile();
	d.closefile();
	
	return 0;
}
