#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "time.h"
#include "unistd.h"

int main(int argc, char *argv[])
{
	int iLine = 0;
	char acBuf[1024];
	FILE *fps;
	FILE *fpd;
	time_t t;

	if( argc < 2 )
	{
		printf("Usage: %s <file>.\n", argv[1]);
		return (-1);
	}

	fpd = fopen(argv[2], "w+");
	if( NULL == fpd )
	{
		perror("Dest file to open:");
		return (-1);
	}

	while( 1 )
	{
		time(&t);
		fprintf(fpd, "%s\n", ctime(&t));
//		fseek(fpd, 0, SEEK_CUR);
//		sleep(1);
		printf("%s\n",ctime(&t));
	}

	fclose(fpd);
	
	return 0;
}

