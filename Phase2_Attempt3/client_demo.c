#include "demo.h"

char *encrypt(char *src) {
    //printf("You are in encrypt\n");
    int len = strlen(src);
    int i = 0;
    int ch = 0;
    int key = 3;
    char q[1024] = " ";

    /* Loop over each char in src */
    for (i = 0; i < len; i++) {
        ch = (int)src[i]; /* Convert the char to int to prevent many uneccecary casts */
        if (ch >= 33 && ch <= 125) {
            ch += key;
            if (ch > 125) ch -= 93;
            if (ch < 33) ch += 93;
            q[i] = (char)ch;
        } else if (ch == 32) {
            // this is the space
            // this is going to be encrypted as a space itself
            q[i] = (char)ch;
        } else if (ch == 10) {
            q[i] = (char)ch;
        }
        
    }

    q[i] = '\0';
    return q;
}

char *decrypt(char *src) {
    
    int len = strlen(src);
    int i = 0;
    int ch = 0;
    int key = 3;
    char q[1024] = " ";

    /* Loop over each char in src */
    for (i = 0; i < len; i++) {
        ch = (int)src[i]; /* Convert the char to int to prevent many uneccecary casts */
       

        if (ch >= 33 && ch <= 125) {
            ch -= key;
            if (ch > 125) ch -= 93;
            if (ch < 33) ch += 93;
            q[i] = (char)ch;
        } else if (ch == 32) {
            // this is the space
            // this is going to be encrypted as a space itself
            q[i] = (char)ch;
        } else if (ch == 10) {
            q[i] = (char)ch;
        }
        
    }
    /* Return a pointer to the char array passed to us */
    q[i] = '\0';
    return q;
}

int main( int argc, char *argv[] )
{
  char buf[BUFSIZE];
  char *result;
  int sockfd;
  struct sockaddr_in servaddr;
  FILE *server_request, *server_reply;
  extern int close();

  /* Check if invoked correctly */
  if( argc != 2 )
    {
      fprintf(stderr, "Usage: %s <IP address>\n", argv[0] );
      exit ( 1 );
    }
  
  /* Open up a TCP/IP socket */
  if( (sockfd = socket(AF_INET, SOCK_STREAM, 0 )) < 0 )
    {
      perror( "socket open" );
      exit( ERR_SOCKET );
    }
  
  /* initialize and prepare for connections to a server */
  bzero( &servaddr, sizeof(servaddr ));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons( CIS551_PORT );

  /* convert address, e.g., 127.0.0.1, to the right format */
  if( inet_pton( AF_INET, argv[1], &servaddr.sin_addr ) <= 0 )
    {
      perror( "inet_pton for address" );
      exit( 99 );
    }

  /* attempt to make the actual connection */
  if( connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0 )
    {
      perror( "connect to associative memory at server" );
      exit( 100 );
    }

  /* setup the interfaces between the new socket and stdio system */
  server_request = fdopen( sockfd, "w" );
  if( server_request == (FILE *) NULL )
    {
      perror( "fdopen of stream for server requests" );
      exit( 2 );
    }
  server_reply = fdopen( sockfd, "r" );
  if( server_reply == (FILE *) NULL )
    {
      perror( "fdopen of stream for server replies" );
      exit( 3 );
    }

  /* The main interactive loop, getting input from the user and 
   * passing to the server, and presenting replies from the server to
   * the user, as appropriate. Lots of opportunity to generalize
   * this primitive user interface...
   */
  fprintf(stderr,"%s","Connected to Drone!\n");
  if( fgets( buf, BUFSIZE, server_reply ) == NULL ) {
        perror( "read failure from associative memory at server");
        exit(0);
  }
  result = decrypt(buf);
  fprintf(stderr, "%s", result);
  
  for( putchar('>'); (fgets( buf, BUFSIZE, stdin ) != NULL ); putchar('>')) {

    
    if( fputs( encrypt(buf), server_request ) == EOF ) {
      perror( "write failure to associative memory at server" );
    }
    
    fflush( server_request );  /* buffering everywhere.... */
    
    if( fgets( buf, BUFSIZE, server_reply ) == NULL ) {
        perror( "read failure from associative memory at server");
        exit(0);
    }
    
    result = decrypt(buf);
    fprintf(stderr,"%s", result );
    
  }
  /* shut things down */
  fclose( server_request );
  fclose( server_reply );
  close( sockfd); 

  exit( 0 );
}