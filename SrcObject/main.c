#include "stdio.h"
#include "stdlib.h"
#include "wait.h"
#include "unistd.h"
#include "typedefs.h"
#include "sys/types.h"
#include "string.h"

void end(void)
{
    exit(2);
}

char acCmd[256];

int main()
{
	int32	i32CldRt = 0;
	pid_t	pid;
	int32 	i32Count;
	printf(">");
	fgets(acCmd, 256, stdin);
	acCmd[strlen(acCmd)-1] = 0;
	pid = fork();
	if(pid < 0)
	{
		perror("fork is failed");
		exit(1);
	}
	if(0 == pid)
	{
		printf("child process pid:%d\n",getpid());
		execv(acCmd,NULL); 
		//execv(acCmd, acCmd);
		perror("child execlp error");
		exit(1);
	}else{
		printf("father process pid:%d\n",getpid());
		wait(NULL);
		printf("Atfer wait().\n");
	}

	return 0;
}

