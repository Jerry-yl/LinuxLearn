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

//#define SERV_PRO

#define SERV_PORT	  7000
#define CLIENT_PORT 7001
#define BACKLOG	  10
#define BUF_SIZE	  128

void *pvRecvThread(void *pvArg)
{
	char acBuf[BUF_SIZE];
	int sockfd;
	int ret;

	sockfd = *((int *)pvArg);
	
	while(1)
	{
		ret = recv(sockfd, acBuf, BUF_SIZE, 0);
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

	sockfd = *((int *)pvArg);
	
	while(1)
	{
		printf("please input msg:");
		if(NULL == fgets(acBuf, sizeof(acBuf), stdin))
		{
			perror("fgets error");
			exit(1);
		}
		ret = send(sockfd, acBuf, BUF_SIZE, 0);
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
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	uint32 u32AddrLen;

	int  iReuseAddr = TRUE;
	int  clientPort;
	pthread_t pthreadRecv_ID;
	pthread_t pthreadSend_ID;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);	
	if( sock_fd < 0)
	{
		perror("socket error");
		exit(1);
	}

#ifdef SERV_PRO

	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(SERV_PORT);
	host_addr.sin_addr.s_addr = INADDR_ANY;  //本机IP地址
	bzero(&(host_addr.sin_zero), 8);

	ret = bind(sock_fd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr));
	if( ret < 0 )
	{
		perror("bind error");
		exit(1);
	}

	ret = listen(sock_fd, BACKLOG);
	if( ret < 0 )
	{
		perror("listen error");
		exit(1);
	}

	printf("Please waiting for the client connection.\n");
	while(1)
	{
		u32AddrLen = sizeof(struct sockaddr_in);
		clients_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &u32AddrLen);
		if( clients_fd < 0)
		{
			perror("accept error");
			continue;
		}
		printf("Client IP: %s, PORT: %d.\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		ret = setsockopt(clients_fd, SOL_SOCKET, SO_REUSEADDR,(char *)&iReuseAddr, sizeof(iReuseAddr));
		if( ret < 0 )
		{
			perror("setsockopt error");
			exit(1);
		}
		ret = pthread_create(&pthreadRecv_ID, NULL, pvRecvThread, &clients_fd);
		if( ret < 0 )
		{
			perror("pthread_create error");
			exit(1);
		}
		ret = pthread_create(&pthreadSend_ID, NULL, pvSendThreadFunc, &clients_fd);
		if( ret < 0 )
		{
			perror("pthread_create error");
			exit(1);
		}
	}

	pthread_join(pthreadRecv_ID, NULL);
	pthread_join(pthreadSend_ID, NULL);
	
#else

	if( argc < 2 )
	{
		printf("a few argument.\n");
		exit(1);
	}
	clientPort = atoi(argv[1]);
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(clientPort);
	host_addr.sin_addr.s_addr = INADDR_ANY;  //本机IP地址
	bzero(&(host_addr.sin_zero), 8);
	ret = bind(sock_fd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr));
	if( ret < 0 )
	{
		perror("bind error");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
//	server_addr.sin_addr.s_addr = INADDR_ANY;  //本机IP地址
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.111");  //本机IP地址
	bzero(&(server_addr.sin_zero), 8);

	ret = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if( ret < 0 )
	{
		perror("connect error");
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
	pthread_join(pthreadSend_ID, NULL);
	
#endif

	close(sock_fd);
	printf("program exit done.!\n");

	return 0;
}

