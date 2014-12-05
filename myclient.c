//* Names: Robert Clarke, Corinne, Derrick

/*CSU ids: 2565537, () , ()

CIS 340: Project #4 

Description: Make udp client server aplication 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port> */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <math.h>

#define BUFSIZE 1024
#define PORT 5555

/* error - wrapper for perror */

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    
    int sockfd, portno, ccheck, n;
    int serverlen;
    struct sockaddr_in serveraddr; //define in socket.h
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    char *input;
    size_t buffer;
    buffer = BUFSIZE;
    	
	    struct {
	char	head;
	u_long	body;
	char	tail;
	int cli_id;
	int ser_id;
	char buffer[BUFSIZE];
    } msg;




    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1]; // localhost
    portno = atoi(argv[2]); // portnumber

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
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

srand(time(NULL)); 
msg.cli_id = rand();
msg.ser_id = rand();

  /*    input the command to send to the server    */




input = (char *)malloc(sizeof(char) * buffer);
//    bzero(buf, BUFSIZE);
printf("Please enter msg: ");
getline(&input, &buffer, stdin);
sprintf(msg.buffer, "%s", input); 


    /* send the message to the server */
    serverlen = sizeof(serveraddr);

   n = sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, (socklen_t)serverlen);

    if (n < 0) 
      error("ERROR in sendto ");
    
    /* print the server's reply */
    n = recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, (socklen_t *)&serverlen);
    

if (n < 0) {
      error("ERROR in recvfrom");
    printf("Message from server: %s", msg.buffer);
}

printf(" Data recieved :: -> %s\n", msg.buffer);
/*if (ccheck < 0){
      error("ERROR in recvfrom");
    printf("Message from server: %d", crandom);
}*/
    


return 0;
}
