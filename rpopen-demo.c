/*
	demo program that uses rpopen
*/

#include <stdio.h>
#define BSIZE 1024

FILE *rpopen(char *host, int port, char *cmd);

main(int argc, char **argv)
{
	FILE *fp;
	char buf[BSIZE];

	if ((fp = rpopen("127.0.0.1", 12345, "ls -lL /etc")) != 0) {
		/* read the output from the command */
		//printf("socket opened: %d\n");

		while (fgets(buf, BSIZE, fp) != 0)
			fputs(buf, stdout);
		fclose(fp);
	}
}
