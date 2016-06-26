#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "time.h"
#include "unistd.h"
#include "sys/stat.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
	char acBuf[1024];
	int iReadCount = 0;
	int fds, fdp;
	
	if( argc < 2 )
	{
		printf("Usage: %s <file>.\n", argv[1]);
		return (-1);
	}

	fds = open(argv[1], O_RDONLY, 0666);
	if( fds < 0)
	{
		perror("fds file to open");
		return(-1);
	}
	fdp = open(argv[2], O_WRONLY|O_TRUNC, 0666);
	if( fdp < 0 )
	{
		perror("Dest file to open");
		close(fds);
		return (-1);
	}

	if( lseek(fds, -10240, SEEK_END) < 0)//跳转到距离文件末尾10KB字节处
	{
		perror("lseek is error");
		close(fds);
		close(fdp);
		return(-1);
	}
	
	while( 1 )
	{
		iReadCount = read(fds, acBuf, 1024);
		if( iReadCount <= 0 )
		{
			perror("read is error");
			break;
		}
		if( write(fdp, acBuf, iReadCount) < 0)
		{
			perror("write is error");
			break;
		}
		
	}

	close(fds);
	close(fdp);
	
	return 0;
}

