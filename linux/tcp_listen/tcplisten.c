#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define ECHO_PORT          (5555)
#define MAX_LINE           (1000)
#define LISTENQ        (1024)   /*  Backlog for listen()   */


static ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;

    for ( n = 1; n < maxlen; n++ ) {
	
	rc = read(sockd, &c, 1);
    if (rc == 0 || rc == -1) return -1;
	if (rc == 1) {
	    *buffer++ = c;
	    if ( c == '\n' )
		break;
	}
	else if ( rc == 0 ) {
	    if ( n == 1 )
		return 0;
	    else
		break;
	}
	else {
	    if ( errno == EINTR )
		continue;
	    return -1;
	}
    }

    *buffer = 0;
    return n;
}


int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */


    /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */

    if ( argc == 2 ) {
	port = strtol(argv[1], &endptr, 0);
	if ( *endptr ) {
	    fprintf(stderr, "ECHOSERV: Invalid port number.\n");
	    exit(EXIT_FAILURE);
	}
    }
    else if ( argc < 2 ) {
	    port = ECHO_PORT;
    } else {
        fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling bind()\n");
	    exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling listen()\n");
        exit(EXIT_FAILURE);
    }

    while (1) {

        printf("waiting for connection at port %d\n", port);
        if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
            fprintf(stderr, "ECHOSERV: Error calling accept()\n");
            exit(EXIT_FAILURE);
        }
        printf("new connection\n");

        while (1) {
            int res = Readline(conn_s, buffer, MAX_LINE-1);
            if (res == -1) {
                close(conn_s);
                printf("closed connection\n");
                break;
            } else {
                printf("%s\n", buffer);
            }
        }
    }
    return 0;
}

