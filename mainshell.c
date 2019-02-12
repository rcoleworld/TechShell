/*******************************************************************************
* Tech Shell Project CSC 222
* Group: Reginald Thomas, Branson Hanzo
* Start Date: 2/9/19
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

/***********************
*        MACROS
***********************/
// Debugger
#define DEBUG 0
// Max size of user input
#define MAX_SIZE 256

/***********************
* PROTOTYPE FUNCTIONS
***********************/
// method to print the current directory default is home ~
void print_directory();
// function that gets the current directory
char* get_directory();
// function that returns user input
char* get_user_input();
// function to tokenize input
void token_input(char c[]);

/*******************
* STRUCTS
*******************/
struct command
{

};

/************
*   MAIN
************/
int main()
{
    char* input;

    printf("\n%s $ ", get_directory());
    input = get_user_input( );

    return 0;
}

/***********************************
* Function is used to receive and
* return the current directory. It
* must start at home.
************************************/
char* get_directory()
{
    char cwd[PATH_MAX];

    return getcwd(cwd, sizeof(cwd));;
}

/************************************
* Function gets and returns user
* input. Intended to be used later.
************************************/
char* get_user_input()
{
    static char input[MAX_SIZE];

    fgets(input, MAX_SIZE, stdin);  // stores user input from stdin to char input
    input[strcspn(input, "\n")] = '\0'; // removes null character at the end of input

    return input;
}

/************************************
* Function that tokenizes input to
* be handled by later functions
************************************/
void token_input(char c[])
{
    char *token;
    const char n[1] = " ";  // splits token by spaces
    token = strtok(c, n);

    // printf("Token(s):\n");

    while (token != NULL)
    {
        // printf(" %s\n", token);
        token = strtok(NULL, n);
    }

}
