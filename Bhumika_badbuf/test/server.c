#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int match(char *s1, char *s2) {


        while( *s1 != '\0' && *s2 != 0 && *s1 == *s2 ){
                s1++;
                s2++;
        }
        return( *s1 - *s2 );
}

void welcome(char *str) {
        printf(str);
 }

void goodbye(char *str) {
        void exit();
        printf(str);
        exit(1);
}


int main(){
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  char *good ="Welcome to The Machine!\n";
  char *evil = "Invalid identity, exiting!\n";


  char buffer1[1024];
  char buffer2[1024];
  char buffer3[1024];
  char buffer4[1024];

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(10551);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("158.130.109.233");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  /*---- Send message to the socket of the incoming connection ----*/

  // Receive the start of the conversation message from client
  recv(newSocket, buffer, 1024, 0);
  printf("Data received: %s",buffer);

  // Ask the client to send the username
  strcpy(buffer1, "Enter the username: \n");
  send(newSocket,buffer1,sizeof(buffer1),0);
  printf("Asked the client to send the username \n");

  // Receive the username from the client 
  recv(newSocket, buffer2, 1024, 0);
  printf("The username received from the client is: %s", buffer2);

  // Ask the client to send the password
  strcpy(buffer3, "Enter the password: \n");
  send(newSocket,buffer3,sizeof(buffer3),0);
  printf("Asked the client to send the password \n");

  // Receive the password from the client
  recv(newSocket, buffer4, 1024, 0);
  printf("The password received from the client is: %s", buffer4);

  // Username -> buffer2 and Password -> buffer4
  if( match(buffer2,buffer4) == 0 )
                welcome( good );
        else
                goodbye(evil );


   return 0;
}
