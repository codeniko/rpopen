/*
	rpserver: remote popen server
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>

#define DEFAULT_PORT 12345
#define MAX_CONNECTIONS 50
#define BUFFER_SIZE 1024

int createSocket() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Cannot create socket");
		return 0;
	}
	return sock;
}

int nameSocket(int port, int sock) {
	struct hostent *hp = NULL; /* remote host info from gethostbyname() */

	struct sockaddr_in myaddr;
	memset((char *)&myaddr, '\0', sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(port);

	int optval = 1;
	/* avoid EADDRINUSE error on bind() */
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0) {
		perror("Failed to set socket options.\n");
		return 0;
	}

	if (bind(sock, (struct sockaddr *) (&myaddr), sizeof(myaddr)) < 0) {
		perror("Failed to set socket options.\n");
		return 0;
	}

		return 1;
}

void rpopen(int sock)
{
	char buf[BUFFER_SIZE];
	int bytesRead = 0;
	int bufIndex = 0; /*contains the index of next empty byte. if 5, then 5th index is empty in buffer*/

	memset(buf, '\0', BUFFER_SIZE);

	while((bytesRead = read(sock, buf + bufIndex, BUFFER_SIZE-bufIndex)) != 0) {
		if (bytesRead < 0) { /*error*/
			perror("Error reading message from socket\n");
			exit(1); /*exit child*/
		}
		bufIndex += bytesRead;
	}

	/*printf("bytesread, %d   val='%s'\n", bufIndex, buf);*/

	/* full message received, process it.*/
	shutdown(sock, SHUT_RD);

	close(0);
	close(1);
	int res = dup2(sock, 1);
	if (res == -1) {
		perror("Failed to duplicate sock to output fd\n");
		exit(1);
	}

	res = execlp("/bin/sh", "/bin/sh", "-c", buf, (char *)0);
	if (res == -1) {
		perror("Failed to execute command\n");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	int port = DEFAULT_PORT;
	int flags, opt;
	extern char *optarg;
	
	while ((opt = getopt(argc, argv, "p:")) != -1) {
		switch(opt) {
			case 'p': port = atoi(optarg); break;
			case '?':
				fprintf(stderr, "Invalid option specified.\n");
				return 1;
		}
	}

	int sock = createSocket();
	if (!sock)
		return 1;

	int bindRes = nameSocket(port, sock);
	if (!bindRes)
		return 1;

	/*Set the number of allowed connections in the queue*/
	if (listen(sock, MAX_CONNECTIONS) < 0)
	{
		perror("Server listening failed. \n");
		return 1;
	}
	/*Accept connection if server*/
	int clientSock = -1;
	while (clientSock = accept(sock, NULL, NULL)) {
		if (clientSock < 0)
		{
			/* we may break out of accept if the system call was interrupted. In this case,
			 * loop back and  try again */ 
			if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
				perror("Server unable to accept connection. Exiting program\n");
				exit(1);
			}
			perror("Server unable to accept connection. \n");
			continue;
		}

		int pid = fork();
		if (pid == -1)
			perror("Error spawning child for new connection");
		if (pid == 0)
			rpopen(clientSock);

		close(clientSock);
	}


}
