#include <stdio.h>
#include <ctype.h>

#define MAXSIZE 1024

void encrypt(char*);
void decrypt(char*);

int menu();

int
main(void)
{

char c,
     choice[2],
     s[MAXSIZE];

 while(1)
 {
 menu();

 gets(choice);

 if((choice[0]=='e')||(choice[0]=='E'))
 {
  puts("Input text to encrypt->");
  gets(s);
  encrypt(s);
 }
 else if((choice[0]=='d')||(choice[0]=='D'))
 {
  puts("Input text to decrypt->");
  gets(s);
  decrypt(s);
 }
 else
    break;
 }

 return 0;
}

void encrypt(char*str)
{
	int n=0;
	char *p=str,
		 q[MAXSIZE];

	while(*p)
	{
	 int ascii = (int)(*p);
	 
	 if(ascii <= 125 && ascii >= 33) {
	 	ascii = ascii + 3;

	 	//bounds check
	 	if(ascii > 125) {
	 		ascii = 33 + (ascii - 126);
	 	}
	 	q[n]= (char) ascii;
	 }
	 else {
	 	//error
	 }
	 n++;
	 p++;
	}
	q[n++]='\0';
	printf(q);
}

void decrypt(char*str)
{
	int n=0;
	char *p=str,
		 q[MAXSIZE];

	while(*p)
	{
	 int ascii = (int)(*p);
	 if(ascii <= 125 && ascii >= 33) {
	 	ascii = ascii - 3;

	 	//bounds check
	 	if(ascii < 33) {
	 		ascii = 126 - (33 - ascii);
	 	}
	 	q[n]= (char) ascii;
	 }
	 else {
	 	//error
	 }
	 n++;
	 p++;
	}
	q[n++]='\0';
	printf(q);
}

int menu()
{
 puts("To encrypt, input e or E\n");
 puts("To decrypt, input d or D\n");
 puts("To exit, input any other letter\n");
 puts("Your choice:->\n");
 return 0;
}
