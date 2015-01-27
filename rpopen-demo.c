/*
	demo program that uses rpopen
*/

#include <stdio.h>
#include "rpopen.h" /* be sure to include the header file*/
#define BSIZE 1024

int main(int argc, char **argv)
{
	FILE *fp;
	char buf[BSIZE];

	/* call the rpopen function here*/
	if ((fp = rpopen("localhost", 12345, "ls -lL /etc")) != 0) {
		/* read the output from the command */
		//printf("socket opened: %d\n");

		while (fgets(buf, BSIZE, fp) != 0)
			fputs(buf, stdout);
		fclose(fp);
	}
}
