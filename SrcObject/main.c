#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "stdlib.h"
#include "typedefs.h"
#include "sched.h"
#include "signal.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/wait.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
	pid_t pid;
	int status,i;
	FILE *fp;
	char *buf = "This is a Daemon.\n";

	pid = fork();   // step one
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}else if( pid > 0 ){
		printf("this is parent process, exit.\n");
		exit(0);
	}

	printf("Front, PID:%d, GUID:%d.\n", getpid(), getpgrp());
	
	if( setsid() < 0 )  //step two
	{
		perror("setsid error");
		exit(1);
	}

	printf("After, PID:%d, GUID:%d.\n", getpid(), getpgrp());

	pid = fork();  //step three
	if( pid < 0 )
	{
		perror("child fork error");
		exit(1);
	}else if(pid > 0){
		printf("this is first child process, exit.\n");
		exit(0);
	}

	for( i=0;i<getdtablesize();i++)   //step four
	{
		close(i);
	}

	chdir("/tmp");   //step five

	umask(0);    //step six

	signal(SIGCHLD, SIG_IGN);    //step seven

	while(1)
	{
		fp = fopen("daemon.log", "a");
		if(NULL == fp)
		{
			perror("fopen error");
			exit(1);
		}
		fputs(buf,fp);
		fclose(fp);
		sleep(3);
	}
		
	return 0;
}

