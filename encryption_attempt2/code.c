#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *encryptText(char *src, int key) {
    int len = strlen(src);
    int i = 0;
    int ch = 0;

    /* Loop over each char in src */
    for (i = 0; i < len; i++) {
        ch = (int)src[i]; /* Convert the char to int to prevent many uneccecary casts */
        //if (ch >= 65 && ch <= 90) { /* If the char is uppercase */
        //    ch += key; /* add the key */
        //    if (ch > 90) ch -= 26; /* if the char is higher than the highest uppercase char, sub 26 */
        //    if (ch < 65) ch += 26; /* if the char is lower than the lowest uppercase char, add 26 */
        //    src[i] = (char)ch; /* set the current char in src to the char value of ch */
        //} else if (ch >= 97 && ch <= 122) { /* else if it's lowercase */
        //    ch += key; /* add the key */
        //    if (ch > 122) ch -= 26; /* if the char is higher than the highest lowercase char, sub 26 */
        //    if (ch < 97) ch += 26; /* if the char is lower than the lowest lowercase char, add 26 */
        //    src[i] = (char)ch; /* set the current char in src to the char value of ch */

        if (ch >= 33 && ch <= 125) {
            ch += key;
            if (ch > 125) ch -= 93;
            if (ch < 33) ch += 93;
            src[i] = (char)ch;
        } else if (ch == 32) {
            // this is the space
            // this is going to be encrypted as a space itself
            src[i] = (char)ch;
        }
        /* an else case is not needed, since we are modifying the original. */
    }
    /* Return a pointer to the char array passed to us */
    return src;
}

char *decryptText(char *src, int key) {
    /* Since getCipherText adds the key to each char, adding a negative key 
     * is equivalent to subtracting a positive key. Easier than re-implementing.
     */
    return encryptText(src, -key);
}

int main(int argc, char const *argv[])
{
	/* code */
	char s[1024];
	int key = 3;
	printf("Enter the text to encrypt\n");
	gets(s);
	encryptText(s,key);
	printf("The encrypted text is: %s\n", s);

	decryptText(s,key);
	printf("The decrypted text is: %s\n", s);
	return 0;
}


