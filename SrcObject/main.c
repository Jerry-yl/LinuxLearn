#include "stdio.h"
#include "stdlib.h"
#include "wait.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/wait.h"
#include "fcntl.h"
#include "syslog.h"
#include "time.h"
#include "string.h"

int main(int argc, char *argv[])
{
	pid_t pid;
	int fd[2];
	char acBuf[256];
	FILE *fp;
	
	if(pipe(fd) < 0)
	{
		perror("pipe error");
		exit(1);
	}

	pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}

	if(0 == pid)
	{
		close(fd[0]);//close read
		fp = fopen("src.txt","r");
		if(NULL == fp)
		{
			perror("fopen error");
			exit(1);
		}
		while(fgets(acBuf,sizeof(acBuf), fp) != NULL)
		{
			if(write(fd[1], acBuf, sizeof(acBuf)) < 0)
			{
				perror("write erro");
				exit(1);
			}
		}
		acBuf[0] = '\x04';//Specify the end flag
		if(write(fd[1], acBuf, sizeof(acBuf)) < 0)
		{
			perror("end write erro");
			exit(1);
		}

		close(fd[1]);
		fclose(fp);
		exit(0);
	}else{
		close(fd[1]);//close write
		
		fp = fopen("des.txt", "w");
		if(NULL == fp)
		{
			perror("fopen error");
			exit(1);
		}

		if(read(fd[0], acBuf, sizeof(acBuf)) < 0)
		{
			perror("read error");
			exit(1);
		}
		while('\x04' != acBuf[0])
		{
			if( fputs(acBuf, fp) == EOF )
			{
				perror("fputs error");
				exit(1);
			}
			if(read(fd[0], acBuf, sizeof(acBuf)) < 0)
			{
				perror("read error");
				exit(1);
			}
		}
	
		if( pid != wait(NULL) )
		{
			perror("wait error");
			exit(1);
		}

		close(fd[0]);
		fclose(fp);
		printf("Copy Done!\n");
	}
	
	return 0;
}

