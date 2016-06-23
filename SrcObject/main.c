#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "time.h"
#include "unistd.h"

int main(int argc, char *argv[])
{
	int iGetResult = 0;
	char *cp,tmp;
	char acBuf[1024];
	FILE *fps;
	FILE *fpd;
	time_t t;
	cp = &tmp;
	
	if( argc < 2 )
	{
		printf("Usage: %s <file>.\n", argv[1]);
		return (-1);
	}

	fps = fopen(argv[1], "r");
	if( NULL == fps )
	{
		perror("fps file to open:");
		return(-1);
	}

	fpd = fopen(argv[2], "w+");
	if( NULL == fpd )
	{
		perror("Dest file to open:");
		fclose(fps);
		return (-1);
	}

	while( 1 )
	{
//		time(&t);
//		fprintf(fpd, "%s\n", ctime(&t));
		cp = fgets(acBuf, sizeof(acBuf), fps);
		if( NULL == cp )
		{
			perror("fgets is error");
			break;
		}
//		fseek(fpd, 0, SEEK_CUR);
		if( NULL == fputs(acBuf, fpd) )
		{
			perror("fputs is error");
			break;
		}
		memset(acBuf, 0x00, sizeof(acBuf));
	}

	fclose(fpd);
	fclose(fps);
	
	return 0;
}

