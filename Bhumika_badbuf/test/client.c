#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "defs.h"

int main(int argc, char *argv[]){

  char *addressIP;
 
  if(argc > 2) {
	printf("Too many arguments supplied.\n");
  }
  else if (argc < 2) {
	printf("One argument expected.\n");
  }
  else if (argc == 2) {
	addressIP = argv[1];
  }
  printf("The server IP address is: %s\n", addressIP);

  int clientSocket;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  char buffer1[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char buffer3[BUFFER_SIZE];
  char buffer4[BUFFER_SIZE];

  /*---- Create the socket ----*/
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(CIS551_PORT);
  /* Set IP address to server IP address */
  serverAddr.sin_addr.s_addr = inet_addr(addressIP);
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  strcpy(buffer,"Welcome to badbuf communication over TCP\n");
  send(clientSocket,buffer,sizeof(buffer),0);
  
  // Receive the request for username from the server
  recv(clientSocket, buffer1, BUFFER_SIZE, 0);
  printf("%s",buffer1);
  putchar('>');
  if (fgets( buffer2, sizeof(buffer2), stdin ) != NULL ) 
  	 send(clientSocket,buffer2,sizeof(buffer2),0);

  // Receive the request for password from the server
  recv(clientSocket, buffer3, BUFFER_SIZE, 0);
  printf("%s",buffer3);
  putchar('>');
  if (fgets( buffer4, sizeof(buffer4), stdin ) != NULL )
         send(clientSocket,buffer4,sizeof(buffer4),0);
  
  return 0;
}

