/*
	rpserver: remote popen server
*/

#define DEFAULT_PORT 12345


main(int argc, char **argv)
{
	int port = DEFAULT_PORT;
	int flags, opt;
	while ((opt = getopt(argc, argv, "p")) != 1) {
		switch(opt) {
			case 'p': port = atoi(optarg); break;
			case '?':
			default:	
		}
	}
}
