/*
	rpopen: remote popen function
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

FILE *
rpopen(char *host, int port, char *cmd)
{
	// if host = 0 or blank, use environemnt variable PHOST
	if (host == 0 || strlen(host) == 0) {

	}
	// if port = 0, use environment variable PPORT
	if (port == 0) {

	}
	if (port < 1024 || port > 65536) {
		//error, not in range
	}
	return 0;
}

