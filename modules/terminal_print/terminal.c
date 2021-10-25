/******************************************************************************************************
*   Program name: Terminal Print in C,
*   Author name: NiyojOli (https://www.github.com/niyoj),
*   Created Path: .modules/terminal_print/terminal.c,
*   Created Date: 07 Oct 2021, 12:01:10 (DD MON YYYY, HH:MM:SS),
*   Description: This  program can be used to implement printing with characters in terminal.
*   Remarks: This project is a modified open source project that can be found here (https://github.com/niyoj/terminal_print_C).
*******************************************************************************************************/

#include <stdio.h>              //is used for basic I/O operations
#include <string.h>             //is used for executing some operations with strings

#include "./fonts/default.c"

//function `print_row()` is used by the function `terminal_print()` to print the character row by row
void print_row(char c, int n) {
    if(c == 'A') {
        printf("%s  ", A[n]);
    } else if ( c == 'B') {
        printf("%s  ", B[n]);
    } else if ( c == 'C') {
        printf("%s  ", C[n]);
    } else if ( c == 'D') {
        printf("%s  ", D[n]);
    } else if ( c == 'E') {
        printf("%s  ", E[n]);
    } else if ( c == 'F') {
        printf("%s  ", F[n]);
    } else if ( c == 'G') {
        printf("%s  ", G[n]);
    } else if ( c == 'H') {
        printf("%s  ", H[n]);
    } else if ( c == 'I') {
        printf("%s  ", I[n]);
    } else if ( c == 'J') {
        printf("%s  ", J[n]);
    } else if ( c == 'K') {
        printf("%s  ", K[n]);
    } else if ( c == 'L') {
        printf("%s  ", L[n]);
    } else if ( c == 'M') {
        printf("%s  ", M[n]);
    } else if ( c == 'N') {
        printf("%s  ", N[n]);
    } else if ( c == 'O') {
        printf("%s  ", O[n]);
    } else if ( c == 'P') {
        printf("%s  ", P[n]);
    } else if ( c == 'Q') {
        printf("%s  ", Q[n]);
    } else if ( c == 'R') {
        printf("%s  ", R[n]);
    } else if ( c == 'S') {
        printf("%s  ", S[n]);
    } else if ( c == 'T') {
        printf("%s  ", T[n]);
    } else if ( c == 'U') {
        printf("%s  ", U[n]);
    } else if ( c == 'V') {
        printf("%s  ", V[n]);
    } else if ( c == 'W') {
        printf("%s  ", W[n]);
    } else if ( c == 'X') {
        printf("%s  ", X[n]);
    } else if ( c == 'Y') {
        printf("%s  ", Y[n]);
    } else if ( c == 'Z') {
        printf("%s  ", Z[n]);
    } else if( c == ' ') {
        printf("%s", SPACE);  
    } else {
        printf("\nAn unexpected error occured. Please check whether the string consists of legal set of characters or not i.e. from A to Z only.\n");
    }
}

//function `terminal_print()` is the main function used to print the characters in terminal we want
//Use of this function can be seen inside `./docs/` folder
int terminal_print(char str[]) {
    char len = strlen(str);                 //determining the length of characters

    for (int i=0; i<MAX_ROW; i++) {         //start looping for 1 to 9th row of the array
        for(int j=0; j<len; j++) {          //start looping for each character given
            char c = str[j];
            print_row(c, i);                //prints the nth row of the character                
        }
        printf("\n");                       //prints the newline after nth row is printed
    }
}