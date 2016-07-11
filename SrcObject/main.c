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
#include "pthread.h"

void *pvMyThread(void *pv)
{
	int i;
	int ret;
	
	ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	if( ret != 0 )
	{
		perror("pthread_setcanceltype error");
		exit(1);
	}
	
	for(i=0;i<10;i++)
	{
		printf("----thread running.\n");
		sleep(1);
	}
	

	pthread_exit("Thank you for the cpu time.\n");
	return pv;
}

int main(int argc, char *argv[])
{
	pthread_t pthreadID;
	int ret;
	int i;
	void *pthread_result = "I like you";
	ret = pthread_create(&pthreadID, NULL, pvMyThread, NULL);
	if( ret != 0 )
	{
		perror("++++pthread_create error");
		exit(1);
	}

//	for(i=0;i<5;i++)
//	{
//		printf("Process running.\n");
//		sleep(1);
//	}
	sleep(3);
	ret = pthread_cancel(pthreadID);
	if( ret != 0 )
	{
		perror("pthread_cancel error");
		exit(1);
	}

	pthread_join(pthreadID, &pthread_result);
	printf("Thread join,it returned:%s\n",(char *)pthread_result);
	
	return 0;
}

