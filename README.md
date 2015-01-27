## rpopen
Remote popen module used within a client/server modeled application. This module provides a function (rpopen) which allows you to send an arbitrary command from the client to the server. The server executes the command and returns a FILE * for the client to read the result output from.

## files
rpserver - Daemon running on the server that accepts and executes a remote command. Default PORT=12345.

rpopen.o - Module providing the rpopen function to send an arbitrary command to a listening rpserver.

rpopen-demo - An example client that uses the rpopen client module and sends a predefined command to an rpserver by calling the rpopen function.


## building and running
To build the binary files, simple run:

	Make

To run:

	1) Find a machine to run the server, or on local, run "./rpserver". You can also specify the port to listen to by running "./rpserver -p [PORT]" where [PORT] is the port number that you want the server to listen on. Make sure your firewall doesn't block the port.
	2) Include the rpopen.h header file in your client and call/read from the rpopen function call.

For an example client program that utilizes rpopen, take a look at rpopen-demo.c. Edit rpopen-demo and change the command to whatever command you want to execute remotely. Also change the IP address to the server or "127.0.0.1" for localhost. Change port to the port that the server is listening on. If IP is null or empty string, the value under the environmental variable named PHOST is used. If port is 0, the value under the environmental variable named PPORT is used.
4) On a client machine, or on local, run the demo file by running "./rpopen-demo"

