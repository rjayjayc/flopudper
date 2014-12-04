/* Names: Robert Clarke, Corinne, Derrick

CSU ids: 2565537, () , ()

CIS 340: Project #4

Description: Make udp client server aplication 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024
#define PORT 5555

/* error - wrapper for perror */

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    
    int ssockfd, portno, ccheck, r,  n;
    int serverlen;
    struct sockaddr_in serveraddr; //define in socket.h
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];

	    struct {
	char	head;
	u_long	body;
	char	tail;
	int cli_id;
	int ser_id;
    } msg;


//Generate random key number number 

    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1]; // localhost
    portno = atoi(argv[2]); // portnumber

    /* socket: create the socket */
    ssockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (ssockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

/* Generates random id numbers*/
srand(time(NULL)); 
msg.cli_id = rand();
msg.ser_id = rand();




    /* get a message from the user
    bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFSIZE, stdin); */
	
// strcpy(buf , " Testing ");

    /* send the message to the server */
    
	serverlen = sizeof(serveraddr);

   n = sendto(ssockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serveraddr, (socklen_t)serverlen);

    if (n < 0) 

      error("ERROR in sendto ");
    
    /* print the server's reply */

    r = recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr *)&serveraddr, (socklen_t *)&serverlen);

printf("%d",n);
printf(" Data recieved :: -> %s\n", msg);

if (n < 0) {

      error("ERROR in recvfrom");

    printf("Message from server: %s", msg);
}

if (n != 0 ){
printf(" Id recieved !");
}
/*if (ccheck < 0){
      error("ERROR in recvfrom");
    printf("Message from server: %d", crandom);
}*/
    

return 0;
}
