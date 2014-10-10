/*
	rpopen: remote popen function
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int createSocket() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Cannot create socket");
		return 0;
	}
	return sock;
}

int createConnection(char *host, int port, int sock) {
	struct hostent *hp; /* host information */ 
	struct sockaddr_in servaddr; /* server address */ 
	memset((char*)&servaddr, 0, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(port); 
	
	/* look up the address of the server given its name */ 
	hp = gethostbyname(host); 
	if (!hp) { 
		fprintf(stderr, "could not obtain address of %s\n", host); 
		return 0; 
	} 
	
	/* put the host's address into the server address structure */ 
	memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);

	if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) { 
		perror("connect failed"); 
		return 0; 
	}

	return 1;
}

	FILE *
rpopen(char *host, int port, char *cmd)
{
	/* if host = 0 or blank, use environemnt variable PHOST*/
	if (host == 0 || strlen(host) == 0) {
		host = getenv("PHOST");
		if (host == 0) {
			fprintf(stderr, "Error, host isn't specified and PHOST environment variable not set. Exiting...\n");
			exit(1);
		}
	}
	/* if port = 0, use environment variable PPORT*/
	if (port == 0) {
		char * portString = getenv("PPORT");
		if (portString == 0) {
			fprintf(stderr, "Error, port isn't specified and PPORT environment variable not set. Exiting...\n");
			exit(1);
		}
		char* stopped;
		port = (int)strtol(portString, &stopped, 10);
		if (*stopped) {
			fprintf(stderr, "Error, port set as environmental variable is not an integer! Exiting...\n");
			exit(1);
		}
	}
	if (port < 1024 || port > 65536) {
		/*error, not in range*/
		fprintf(stderr, "Port is not in range. It needs to be >=1024 or <=65536.\n");
		return 0;
	}

	int sock = createSocket();
	if (!sock)
		return 0;
	int success = createConnection(host, port, sock);
	if (!success)
		return 0;

	uint16_t msgLen = htons(strlen(cmd));

	write(sock, cmd, strlen(cmd));
	shutdown(sock, SHUT_WR);
	return fdopen(sock, "r");
}

