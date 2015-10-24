#include "demo.h"

typedef enum { false, true } bool;
struct sym_list Head;   /* head of singly-linked list */


int main( argc, argv, env )
    int argc;
    char *argv[], *env[];
{
  int server_fd, create_service(), connection_fd;
  void service(), save();
  // restore();
  socklen_t len;
  struct sockaddr_in cliaddr;
  char buf[BUFSIZE];
  extern int close();

  server_fd = create_service();

  while( HELL_NOT_FROZEN )
    {
      len = sizeof( cliaddr );
      connection_fd = accept( server_fd, (SA *) &cliaddr, &len );

      if( connection_fd < 0 ){
      perror( "accept on server_fd" );
      exit( ERR_ACCEPT );
    }

    service(connection_fd);
    close( connection_fd );
    }
}

void service( int fd ) {
    void save(), restore();
    bool inChoosingOptions = false;
    bool inInsertUsername = false;
    bool inInsertPassword = false;
    bool inAuthUsername = true;
    bool inAuthPassword = false;
    bool inInsertNewPassword = false;
    bool userLoggedIn = false;

    char *ptr, *name, *pass, *newPass;

    FILE *client_request, *client_reply, *fdopen();
    char buf[BUFSIZE];
    extern  void fix_tcl(), insert();

    /* interface between socket and stdio */
    client_request = fdopen( fd, "r" );
    if( client_request == (FILE *) NULL ) {
        perror( "fdopen of client_request" );
        exit( 1 );
    }

    client_reply = fdopen( fd, "w" );
    if( client_reply == (FILE *) NULL ) {
        perror( "fdopen of client_reply" );
        exit( 1 );
    }

    fprintf(stderr,"%s","New Client Connected");
    
    fputs( "Enter Username to Start: \n", client_reply );
    fflush(client_reply);
    

    while( fgets( buf, BUFSIZE, client_request ) != NULL ){
    fix_tcl( buf ); /* hack to interface with tcl scripting language */
    fprintf(stderr,"%s",buf);

  
      if(inChoosingOptions) {
        if ((ptr = find_1( buf )) != (char *) NULL) {
          fputs( "Enter Username:\n", client_reply );
          fflush( client_reply );
          inChoosingOptions = false;
          inInsertUsername = true;
        }
        else if ((ptr = find_2( buf )) != (char *) NULL) {
          fputs( "Authenticate Username:\n", client_reply );
          fflush( client_reply );
          inChoosingOptions = false;
          inAuthUsername = true;
        }
        else{
              fputs( "Enter 1 to Insert New User or Enter 2 to Update Password for Existing User. \n", client_reply );
              fflush( client_reply );
            }
      } // inCHoosingOptions loop ends here

      else if(inInsertUsername){
        name = strsave(strtok(buf, "\n"));
        fputs( "Enter New Password:\n", client_reply );
        fflush( client_reply );
        inInsertUsername = false;
        inInsertPassword = true;
      } // inInsertUsername loop ends here

      else if (inInsertPassword) {
        pass = strsave(buf);
        fputs( "Username and Password Saved.  Press Enter.\n", client_reply );
        fflush( client_reply );
        insert( name, pass );
        save(DATABASE);
        inInsertPassword = false;
        inChoosingOptions = true;
      } // inInsertPassword loop ends here

      else if(inAuthUsername){
        name = strsave(strtok(buf, "\n"));
        fputs( "Enter Current Password:\n", client_reply );
        fflush( client_reply );
        inAuthUsername = false;
        inAuthPassword = true;
      } // inAuthUsername loop ends here

      else if (inAuthPassword) {
        pass = strsave(buf); 
        restore(DATABASE);
        if((lookup(name) != NULL) && (strcmp(lookup(name), pass) == 0)){ 
            fputs( "User Authenticated. Press Enter.\n", client_reply );
            fflush( client_reply );
            inAuthPassword = false;
            if(userLoggedIn == false) {
              inChoosingOptions = true;
              userLoggedIn = true;   
            }
            else {
              inInsertNewPassword= true;
            }
        }
        else if(userLoggedIn){
            fputs( "Incorrect User or Password. Returning to Options menu. Press Enter.\n", client_reply );
            fflush( client_reply );
            inInsertUsername = false;
            inInsertPassword = false;
            inAuthPassword = false;
            inAuthUsername = false;
            inChoosingOptions = true;       
        }
        else {
            fputs( "Incorrect User or Password. Enter Username Again:\n", client_reply );
            fflush( client_reply );
            inInsertUsername = false;
            inInsertPassword = false;
            inAuthPassword = false;
            inAuthUsername = true;
            inChoosingOptions = false;    
        }   
      } // inAuthUsername loop ends here
      
      else if(inInsertNewPassword){
        fputs( "Enter New Password:\n", client_reply );
        fflush( client_reply );
        inInsertUsername = false;
        inInsertPassword = true;
      } // inInsertPassword loop ends here

    } // end of while loop

    fprintf(stderr,"%s","Client Disconnected");
    return;
}

int create_service()
{
  int listenfd;
  struct sockaddr_in servaddr;

  listenfd = socket(AF_INET, SOCK_STREAM, 0 );
  if( listenfd < 0 )
    {
      perror( "creating socket for listenfd" );
      exit( ERR_SOCKET );
    }
  
  bzero( &servaddr, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
  servaddr.sin_port = htons( CIS551_PORT );

  if( bind( listenfd, (SA *) &servaddr, sizeof(servaddr) ) < 0 )
  {
    perror( "bind on listenfd");
    exit( ERR_BIND );
  }

  if( listen( listenfd, LISTENQ ) < 0 )
    {
      perror( "listen on listenfd" );
      exit( ERR_LISTEN );
    }

  return( listenfd );
}

void
fix_tcl( char *buf )
{
  char *ptr;

#define CARRIAGE_RETURN '\r'
  if( (ptr = strrchr( buf, CARRIAGE_RETURN )) != NULL )
    *ptr = EOS;
  return;

}
