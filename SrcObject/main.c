
#include "stdio.h"
#include "string.h"
#include "errno.h"

int main(int argc, char *argv[])
{
	int iLine = 0x12345678;
	char acBuf[6] = {1,2,0,4,5,6};
	char ca = 1;
	char cb = 2;
	char cc = 3;
	char cd = 4;
	int i = 0x12345678;
	FILE *fp;

//	if( argc < 2 )
//	{
//		printf("Usage: %s <file>.\n", argv);
//		return (-1);
//	}

//	fp = fopen(argv[1], "r");
//	if( NULL == fp )
//	{
//		perror("file to open:");
//		return (-1);
//	}

//	while( fgets(acBuf, 1024, fp) != NULL)
//	{
//	//	if( acBuf[10])
//		iLine++;
//	}

	printf("The array length is:%d.\n", strlen(acBuf));
	acBuf[strlen(acBuf)-1] = 8;

	printf("The file line is:%d.\n", acBuf[5]);
	
	return 0;
}

