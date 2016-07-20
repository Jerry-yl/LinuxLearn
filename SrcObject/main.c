#include "stdio.h"
#include "stdlib.h"
#include "wait.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/wait.h"
#include "sys/socket.h"
#include "arpa/inet.h"
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

#define SERV_PRO

#ifdef SERV_PRO
#define SEND_PORT	  7456
#define RECV_PORT   7123
#else
#define SEND_PORT	  7123
#define RECV_PORT   7456
#endif
#define BUF_SIZE	  128

void *pvRecvThread(void *pvArg)
{
	char acBuf[BUF_SIZE];
	int sockfd;
	int ret;
	struct sockaddr_in sock_addr;
	uint32 u32AddrLen;
	
	sockfd = *((int *)pvArg);

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(SEND_PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(sock_addr.sin_zero), 8);
	u32AddrLen = sizeof(struct sockaddr_in);
	
	while(1)
	{
		ret = recvfrom(sockfd, acBuf, BUF_SIZE, 0, (struct sockaddr *)&sock_addr, &u32AddrLen);
		if( ret < 0 )
		{
			perror("recv error");
			exit(1);
		}
		printf("Recv msg:%s\n", acBuf);
		if( 0 == strncmp(acBuf, "exit", 4) )
		{
			printf("TCP exit.\n");
			break;
		}

		usleep(10000);
	}
	
	pthread_exit(NULL);
}

void *pvSendThreadFunc(void *pvArg)
{
	char acBuf[BUF_SIZE];
	int sockfd;
	int ret;
	struct sockaddr_in sock_addr;
	
	sockfd = *((int *)pvArg);

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(RECV_PORT);
	sock_addr.sin_addr.s_addr = inet_addr("192.168.0.111");
	bzero(&(sock_addr.sin_zero), 8);
	
	
	while(1)
	{
		printf("please input msg:");
		if(NULL == fgets(acBuf, sizeof(acBuf), stdin))
		{
			perror("fgets error");
			exit(1);
		}
		ret = sendto(sockfd, acBuf, BUF_SIZE, 0, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr_in));
		if( ret < 0 )
		{
			perror("send error");
			exit(1);
		}
		if( 0 == strncmp(acBuf, "exit", 4) )
		{
			printf("TCP exit.\n");
			break;
		}

		usleep(10000);
	}
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int ret;
	int sock_fd;
	int clients_fd;
	struct sockaddr_in host_addr;
	uint32 u32AddrLen;

	int  iReuseAddr = TRUE;
	pthread_t pthreadRecv_ID;
	pthread_t pthreadSend_ID;

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if( sock_fd < 0)
	{
		perror("socket error");
		exit(1);
	}

#ifdef SERV_PRO

	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(SEND_PORT);
	host_addr.sin_addr.s_addr = INADDR_ANY;  //本机IP地址
	bzero(&(host_addr.sin_zero), 8);

	ret = bind(sock_fd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr));
	if( ret < 0 )
	{
		perror("bind error");
		exit(1);
	}

	printf("Please waiting for the client connection.\n");

	ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,(char *)&iReuseAddr, sizeof(iReuseAddr));
	if( ret < 0 )
	{
		perror("setsockopt error");
		exit(1);
	}
	ret = pthread_create(&pthreadRecv_ID, NULL, pvRecvThread, &sock_fd);
	if( ret < 0 )
	{
		perror("pthread_create error");
		exit(1);
	}
	ret = pthread_create(&pthreadSend_ID, NULL, pvSendThreadFunc, &sock_fd);
	if( ret < 0 )
	{
		perror("pthread_create error");
		exit(1);
	}

	pthread_join(pthreadRecv_ID, NULL);
	pthread_join(pthreadSend_ID, NULL);
	
#else

	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(SEND_PORT);
	host_addr.sin_addr.s_addr = INADDR_ANY;  //本机IP地址
	bzero(&(host_addr.sin_zero), 8);
	ret = bind(sock_fd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr));
	if( ret < 0 )
	{
		perror("bind error");
		exit(1);
	}

	ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,(char *)&iReuseAddr, sizeof(iReuseAddr));
	if( ret < 0 )
	{
		perror("setsockopt error");
		exit(1);
	}

	ret = pthread_create(&pthreadSend_ID, NULL, pvSendThreadFunc, &sock_fd);
	if( ret < 0 )
	{
		perror("pthread_create error");
		exit(1);
	}
	ret = pthread_create(&pthreadRecv_ID, NULL, pvRecvThread, &sock_fd);
	if( ret < 0 )
	{
		perror("pthread_create error");
		exit(1);
	}
	
	pthread_join(pthreadRecv_ID, NULL);
	pthread_join(pthreadSend_ID,NULL);
#endif

	close(sock_fd);
	return 0;

}
