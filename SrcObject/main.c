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
#include "errno.h"

void vSigHandler(int sig, siginfo_t *info, void *t)
{
	printf("Recv signal:%d\n",sig);
	printf("Recv msg:%d\n",info->si_int);

	return;
}

int main(int argc, char *argv[])
{
	int status;
	pid_t pid;

	struct sigaction act;
	union sigval sig;
	pid = getpid();

	//Recv signal process
//	act.sa_sigaction = vSigHandler;
//	sigemptyset(&act.sa_mask);
//	act.sa_flags = SA_SIGINFO;
//	status = sigaction(SIGUSR1, &act, NULL);
//	if( status < 0 )
//	{
//		perror("sigaction error");
//		exit(1);
//	}
//	printf("Recv:\n");
//	printf("PID:%d\n",pid);
//	while(1);

	//Send signal process,Select one of the compile-time
	if( argc < 2 )
	{
		printf("paramter is less");
		exit(1);
	}
	pid = atoi(argv[1]);
	sig.sival_int = getpid();
	status = sigqueue(pid, SIGUSR1, sig);
	if(status < 0)
	{
		perror("sigqueue error");
		exit(1);
	}else{
		printf("Send Done\n");
	}
	
	return 0;
}

