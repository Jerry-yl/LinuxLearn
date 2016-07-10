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
#include "sys/ipc.h"
#include "sys/msg.h"
#include "sys/sem.h"
#include "sys/shm.h"
#include "Typedefs.h"

#define SHM_SIZE (1024)

int main(int argc, char *argv[])
{
	int shmid, semid;
	key_t key;
	int ret;
	bool bExit = FALSE;
	pid_t pid;
	char *pshmAddr;
	char acBuf[256];

	struct sembuf sSemLock = {0, -1, SEM_UNDO};
	struct sembuf sSemUnlock = {0, 1, SEM_UNDO|IPC_NOWAIT};

	key = ftok("/tmp", 'b');
	if(key < 0)
	{
		perror("ftok error");
		exit(1);
	}
	shmid = shmget(key, SHM_SIZE, IPC_CREAT|0666);
	if(shmid < 0)
	{
		perror("shmget error");
		exit(1);
	}
	semid = semget(key, 1, IPC_CREAT |0666);
	if( semid < 0 )
	{
		perror("semget error");

		shmctl(shmid, IPC_RMID, 0);
		exit(1);
	}
	ret = semctl(semid, 0, SETVAL, 1);
	if(ret < 0)
	{
		perror("semctl error");
		exit(1);
	}

	pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	if( 0 == pid )
	{
		pshmAddr = shmat(shmid, NULL, 0);
		if( ((void *)(-1)) == pshmAddr )
		{
			perror("shmat error");
			exit(1);
		}
		
		while(1)
		{
			ret = semop(semid, &sSemLock, 1);
			if( ret < 0 )
			{
				perror("semop lock error");
				exit(1);
			}
			printf("Please input msg:");
			if(NULL == fgets(acBuf, 256, stdin))
			{
				perror("fgets error");
				exit(1);
			}

			strcpy(pshmAddr, acBuf);

			ret = semop(semid, &sSemUnlock, 1);
			if( ret < 0 )
			{
				perror("semop unlock error");
				exit(1);
			}

			if( 0 == strncmp(acBuf+1, "exit", 4) )
			{
				break;
			}
		}

		ret = shmdt((void *)pshmAddr);
		if( ret < 0 )
		{
			perror("shmdt error");
			exit(1);
		}

		printf("child process quit\n");
		exit(0);
	}else{
		pshmAddr = shmat(shmid, NULL, 0);
		if( ((void *)(-1)) == pshmAddr)
		{
			perror("shmat error");
			exit(1);
		}
		
		while(1)
		{
			ret = semop(semid, &sSemLock, 1);
			if( ret < 0 )
			{
				perror("father semop lock error");
				exit(1);
			}
			if(0 == strncmp(pshmAddr, "b", 1) )
			{
				printf("Recv msg:%s\n", pshmAddr+1);
				strcpy(acBuf, pshmAddr);
				memset(pshmAddr, 0, SHM_SIZE);
			}
			ret = semop(semid, &sSemUnlock, 1);
			if( ret < 0 )
			{
				perror("father semop unlock error");
				exit(1);
			}
			if(0 == strncmp(acBuf+1, "exit", 4))
			{
				break;
			}
		}

		if(pid != wait(NULL))
		{
			perror("wait error");
//			exit(1);
		}

		ret = shmdt((void *)pshmAddr);
		if( ret < 0 )
		{
			perror("shmdt error");
			exit(1);
		}

		ret = shmctl(shmid, IPC_RMID, 0);
		if( ret < 0 )
		{
			perror("shmctl error");
			exit(1);
		}

		ret = semctl(semid, IPC_RMID, 0);
		if( ret < 0 )
		{
			perror("semctl error");
			exit(1);
		}

		printf("father process quit\n");
	}

	return 0;
}

