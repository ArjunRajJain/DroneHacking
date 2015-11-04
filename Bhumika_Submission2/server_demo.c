#include "demo.h"

char* encrypt(char*str)
{
  int n=0;
  char *p=str,
     q[MAXSIZE];

  while(*p)
  {
   if(islower(*p))
   {
     if((*p>='a')&&(*p<'x'))
       q[n]=toupper(*p + (char)3);
     else if(*p == ' ')
        q[n] = ' ';
     else if(*p=='x')
       q[n]='A';
     else if(*p=='y')
       q[n]='B';
     else
       q[n]='C';
   }
   else
   {
     q[n]=*p;
   }
   n++; p++;
  }
  q[n++]='\0';
  return q;
}

char* decrypt(char*str)
{
  int   n=0;
  char *p=str,
     q[MAXSIZE];

  while(*p)
  {
   if(isupper(*p))
   {
     if((*p>='D')&&(*p<='Z'))
       q[n]=tolower(*p - (char)3);
     else if(*p == ' ')
        q[n] = ' ';
     else if(*p=='A')
       q[n]='x';
     else if(*p=='B')
       q[n]='y';
     else
       q[n]='z';
   }
   else
   {
     q[n]=*p;
   }
   n++; p++;
  }
  q[n++]='\0';
  return q;
}

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
    bool inUpdatePassword = false;
    bool inInsertNewPassword = false;
    bool userLoggedIn = false;
    bool inUpdate = false;

    char *ptr, *name, *pass, *newPass, *username;

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

    fprintf(stderr,"%s","New Client Connected \n");
    
    fputs( encrypt("enter username to start:\n"), client_reply );
    fflush(client_reply);
    

    while( fgets( buf, BUFSIZE, client_request ) != NULL ){
    fix_tcl( buf ); /* hack to interface with tcl scripting language */
    fprintf(stderr,"%s",buf);
    char *result = decrypt(buf);
    fprintf(stderr,"%s",result);

  
      if(inChoosingOptions) {
        if ((ptr = find_1( buf )) != (char *) NULL) {
          fputs( encrypt("enter username:\n"), client_reply );
          fflush( client_reply );
          inChoosingOptions = false;
          inInsertUsername = true;
        }
        else if ((ptr = find_2( buf )) != (char *) NULL) {
          fputs( encrypt("enter current password:\n"), client_reply );
          fflush( client_reply );
          inChoosingOptions = false;
          inUpdate = true;
        }
        else{
              fputs( encrypt("enter 1 to insert new user or enter 2 to update password for existing user. \n"), client_reply );
              fflush( client_reply );
            }
      } // inCHoosingOptions loop ends here

      else if(inInsertUsername){
        name = strsave(strtok(buf, "\n"));
        fputs( encrypt("enter new password:\n"), client_reply );
        fflush( client_reply );
        inInsertUsername = false;
        inInsertPassword = true;
      } // inInsertUsername loop ends here

      else if (inInsertPassword) {
        pass = strsave(buf);
        fputs( encrypt("username and password saved. press enter.\n"), client_reply );
        fflush( client_reply );
        insert( name, pass );
        save(DATABASE);
        inInsertPassword = false;
        inChoosingOptions = true;
      } // inInsertPassword loop ends here

      else if (inUpdatePassword) {
        pass = strsave(buf);
        fputs( encrypt("password updated. press enter.\n"), client_reply );
        fflush( client_reply );
        insert(username, pass );
        save(DATABASE);
        inUpdatePassword = false;
        inChoosingOptions = true;
      } // inInsertPassword loop ends here

      else if(inUpdate){
        pass = strsave(buf);
        restore(DATABASE);
        if((lookup(username) != NULL) && (strcmp(lookup(username), pass) == 0)){ 
          fputs( encrypt("user authenticated. enter new password.\n"), client_reply );
          fflush( client_reply );
          inUpdate = false;
          inUpdatePassword = true;
        }
        else {
          fputs( encrypt("incorrect password, enter current password again: \n"), client_reply );
          fflush( client_reply );
        }
      } // inAuth

      else if(inAuthUsername){
        username = strsave(strtok(buf, "\n"));
        fputs( encrypt("enter current password:\n"), client_reply );
        fflush( client_reply );
        inAuthUsername = false;
        inAuthPassword = true;
      } // inAuthUsername loop ends here

      else if (inAuthPassword) {
        pass = strsave(buf); 
        restore(DATABASE);
        if((lookup(username) != NULL) && (strcmp(lookup(username), pass) == 0)){ 
            fputs( encrypt("user authenticated. press enter.\n"), client_reply );
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
            fputs( encrypt("incorrect user or password. returning to options menu. press enter.\n"), client_reply );
            fflush( client_reply );
            inInsertUsername = false;
            inInsertPassword = false;
            inAuthPassword = false;
            inAuthUsername = false;
            inChoosingOptions = true;       
        }
        else {
            fputs( encrypt("incorrect user or password. enter username again:\n"), client_reply );
            fflush( client_reply );
            inInsertUsername = false;
            inInsertPassword = false;
            inAuthPassword = false;
            inAuthUsername = true;
            inChoosingOptions = false;    
        }   
      } // inAuthUsername loop ends here
      
      else if(inInsertNewPassword){
        fputs( encrypt("enter new password:\n"), client_reply );
        fflush( client_reply );
        inInsertUsername = false;
        inInsertPassword = true;
      } // inInsertPassword loop ends here

    } // end of while loop

    fprintf(stderr,"%s","client disconnected \n");
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
