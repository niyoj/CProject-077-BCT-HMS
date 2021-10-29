/******************************************************************************************************
*   Program name: Security Module for HIMS,
*   Author name: NiyojOli(THA077BCT029), 
*   Created Path: /modules/security.c,
*   Created Date: 19 Oct 2021, 08:08:31 (DD MON YYYY, HH:MM:SS),
*   Description: This  program file is responsible for encryption, decryption and generating tokens.
*******************************************************************************************************/

//function hash() is a simple hashing algorithm in C
void hash(unsigned char *str, char retrn[]) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        
    for(int i=19; i>0; i--) {
        int print = (int)(hash%10);
        retrn[19-i] = print+65;
        hash = hash/10; 
    }
}

//function create_token() is used to generate a random number in form of integer
void create_token(char retrn[]) {
    int max = 1, min = 0;
    int l = 6;
    while(l > 0){
        max *= 10;
        l--;
    }
    min = max/10;
    int generated = min + rand() % (max - min);
    
    sprintf(retrn, "%d", generated);
}