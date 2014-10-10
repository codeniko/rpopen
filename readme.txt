Nikolay Feldman
NETID: nf139

Distributed Systems, Assignment 2.

FILES SUBMITTED:
Makefile - the default makefile provided to us to build our binary files.

rpserver.c - the code for the server side aspect of the rpopen project. It listens on a port specified and defaults to port 12345 if not specified.

rpopen.c - the code for the rpopen client library to connect to the rpserver and return a FILE * to the output of the command executed remotely.

rpopen-demo.c - the client that uses the rpopen client library and sends an arbitrary predefined command to an rpserver.


HOW TO COMPILE AND RUN
1) Open a terminal
2) Download the tar archive file into the currently directory (use wget or curl)
3) Extract the files from this archive file by running "tar -xf a2.tar"
4) Run "make" to build the binary files
5) Find a machine to run the server, or on local, run "./rpserver". You can also specify the port to listen to by running "./rpserver -p [PORT]" where [PORT] is the port number that you want the server to listen on. Make sure your firewall doesn't block the port.
6) Edit rpopen-demo and change the command to whatever command you want to execute remotely. Also change the IP address to the server or "127.0.0.1" for localhost. Change port to the port that the server is listening on. If IP is null or empty string, the value under the environmental variable named PHOST is used. If port is 0, the value under the environmental variable named PPORT is used. Run "make" everytime you change the command.
7) On a client machine, or on local, run the demo file by running "./rpopen-demo"



TESTS/MODIFICATIONS TO RPOPEN-DEMO.C

1) rpopen(0, 0, "ls -lL /etc")
OUTPUT, success if PPORT and PHOST set

2) rpopen(0, 0, "")
OUTPUT, error

3) rpopen("127.0.0.1", 0, "ls -lL /etc")
OUTPUT, success if PPORT is set

4) rpopen("127.0.0.1", 12345, "ls -lL /etc")
OUTPUT, success if listening on port 12345 (default)

5) rpopen("127.0.0.1", 123, "ls -lL /etc")
OUTPUT, success if listening on port 123 (not default)

6) rpopen(0, 12345, "ls -lL /etc")
OUTPUT, success if PHOST set

7) rpopen("", 12345, "ls -lL /etc")
OUTPUT, success if PHOST set

8) rpopen("127.0.0.1", 12345, "touch test")
OUTPUT, success

9) rpopen("127.0.0.1", 0, "ls -lL /etc") BUT PPORT DOES NOT HOLD A NUMBER
OUTPUT, error
