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
/*******************
*     STRUCTS
*******************/
struct ShellCommand
{
    char* command;
    char* arguments;
};

/***********************
* PROTOTYPE FUNCTIONS
***********************/
// method to print the current directory default is home ~
void print_directory();
// function that gets the current directory
char* get_directory();
// function that returns user input
char* get_user_input();
// function to tokenizes input
void parse_input(char c[]);
// function to execute commands
void execute_command(struct ShellCommand s);
// function to debug token splits
void tokenizer(char c[]);

/************
*   MAIN
************/
int main()
{
    char* input;
    struct ShellCommand c;

    chdir(getenv("HOME"));

    printf("\n%s$ ", get_directory());
    input = get_user_input();
    c.command = input;
    execute_command(c);

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
* be handled by later functions.
* This needs to split it into
* arguments and the command.
* For example:
* $ ls -l -a -h ~/Documents.
*
* Command: ls -l -a -h
* Argument(s): ~/Documents
************************************/
void parse_input(char c[])
{


}

/************************************
* Function that executes the commands
* that the user inputs.
************************************/
void execute_command(struct ShellCommand s)
{
    if(strcmp(s.command, "cd") == 0)
    {
        chdir("s.arguments");
    }
    if(strcmp(s.command, "exit") == 0)
    {
        printf("Exited\n");
        exit(0);
    }
}

/***********************************
* This function will not be
* implemented. It is just used to
* test how strings can be split up.
*
* These tests will be used to
* implement the parse_input() function
***********************************/
void tokenizer(char c[])
{
    char *token;
    const char n[1] = " ";  // splits token by spaces
    token = strtok(c, n);
    int count = 0;  // counts the tokens

    printf("Token(s):\n");

    while (token != NULL)
    {
        printf(" %s\n", token);
        token = strtok(NULL, n);
        count++;    // adds one to token count
    }
    printf("%d token(s) read\n\n", count);
}
