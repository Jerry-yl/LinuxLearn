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
#include "Typedefs.h"

struct msgbuf{
	long mtype;
	char mtext[128];
};

int main(int argc, char *argv[])
{
	struct msgbuf sMsgBuf;
	int qid;
	key_t key;
	int status;
	bool bExit = FALSE;

	key = ftok("/tmp", 'b');
	if(key < 0)
	{
		perror("ftok error");
		exit(1);
	}
	qid = msgget(key, IPC_CREAT|0666);
	if(qid < 0)
	{
		perror("msgget error");
		exit(1);
	}

#if 0
	//Msg queue send process
	while(1)
	{
		printf("Please Input the Message:");
		fgets(sMsgBuf.mtext, sizeof(sMsgBuf.mtext),stdin);
		if( 0 == strncmp(sMsgBuf.mtext, "exit", 4) )
		{
			bExit = TRUE;
		}

		sMsgBuf.mtype = 123;
		status = msgsnd(qid, &sMsgBuf, 128, 0);
		if( status < 0 )
		{
			perror("msgsnd error");
			exit(1);
		}
		printf("Send Success!\n");

		if(bExit)
		{
			break;
		}
	}

#else
	//msg queue recvice process
	while(1)
	{
		memset(&sMsgBuf, 0, sizeof(sMsgBuf));
		status = msgrcv(qid, &sMsgBuf, 128, 0, 0);
		if(status < 0)
		{
			perror("sgrcv error");
			exit(1);
		}
		printf("Recv msg:");
		printf("Type=%d,Length=%d, Msg:%s\n", sMsgBuf.mtype, status, sMsgBuf.mtext);

		if( 0 == strncmp(sMsgBuf.mtext, "exit", 4) )
		{
			break;
		}
	}

#endif

	return 0;
}

