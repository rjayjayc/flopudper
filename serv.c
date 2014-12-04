/*********************************************************************\
* 			  RECV_UDP.C				      *
* Test of UDP/IP. Receive datagrams on internet socket bound to port  *
* 0x3333 on the local host.					      *
* 								      *
* To use:							      *
* 	1) On the appropriate host, invoke this code by		      *
* 	   "recv_udp&".						      *
* 	2) Invoke send_udp as many times as desired on the	      *
* 	   remote host.						      *
* 	3) When done, MAKE SURE TO KILL THIS BACKGROUND PROCESS!      *
* 								      *
* Phil Kearns							      *
* April 11, 1987						      *			      *
\*********************************************************************/

#include	<signal.h>
#include	<errno.h>
#include	<strings.h>
#include	<string.h>
#include 	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include        <arpa/inet.h>


#define PORT 5555
#define BUFSIZE 1024

int
main()
{
    int	csocket_fd, cc, newsocket_fd, clientsize;
    struct sockaddr_in	s_in, clientaddr;
    void printsin();    

    struct {
	char	head;
	u_long	body;
	char	tail;
	int cli_id;
	int ser_id;
    } msg;



char buf[BUFSIZE];


/*   Create the socket to be used for datagram reception. Initially,
     it has no name in the internet (or any other) domain. */
    
csocket_fd = socket (AF_INET, SOCK_DGRAM, 0);

    if (csocket_fd < 0) {
	perror ("Server :socket");
	exit (1);
    }

/*
   In order to attach a name to the socket created above, first fill
   in the appropriate blanks in an inet socket address data structure
   called "s_in". We blindly pick port number 5555. The second step
   is to BIND the address to the socket. If port 5555 is in use, the
   bind system call will fail detectably.
*/

    bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */

    s_in.sin_family = AF_INET;
    s_in.sin_addr.s_addr = (INADDR_ANY);    /* WILDCARD */
    s_in.sin_port = htons(PORT);
    printsin( &s_in, "Server's", "Local socket is:"); 
    fflush(stdout);

/*
   bind port 0x3333 on the current host to the socket accessed through
   csocket_fd. If port in use, die.
*/
    if (bind(csocket_fd, (struct sockaddr *)&s_in, sizeof(s_in)) < 0) {
	    perror("recv_udp:bind");
	    exit(1);
	}

//


listen(csocket_fd, 5);  

puts("Waiting for Connection...........\n");  

	for(;;) { //infinite loop
	
	clientsize = sizeof(clientaddr);

cc = recvfrom(csocket_fd,&msg ,sizeof(msg),0,(struct sockaddr *)&clientaddr,&clientsize);
	
         if (cc < 0){
	    perror(" Error orginating from Server-> :recvfrom() ");
	}
	printsin( &clientaddr, " Server : ", "Information from: Client");
	
	printf(" Client id # -> :: %d\n", msg.cli_id);
	printf(" Server id # -> :: %d ", msg.ser_id);



int clientid = msg.cli_id;
int serverid = msg.ser_id;


int s;

s = sendto(csocket_fd, msg, sizeof(msg), 0, (struct sockaddr *)&clientaddr, clientsize);

if (s < 0){
perror(" Error orginating from Server-> :sendto() ");
}
	fflush(stdout);
    }// End of for loop

    return(0);
}

void
printsin( sin, m1, m2 )
struct sockaddr_in *sin;
char *m1, *m2;
{

    printf ("%s %s:\n", m1, m2);
    printf ("Family %d, addr %s, port %d\n", sin -> sin_family,
	    inet_ntoa(sin -> sin_addr), ntohs((unsigned short)(sin -> sin_port)));
}

/*void
printids( sin, m1, m2 )
struct msg *sin;
int m1, m2;
{

    printf ("%d %d:\n", m1, m2);
    printf ("  family %d, client id %d, server id %d\n", sin -> msg,
	    inet_ntoa(sin -> sin_addr), ntohs((unsigned short)(sin -> sin_port)));
}*/

