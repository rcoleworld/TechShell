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
    char *commands[MAX_SIZE];
    char *arguments[MAX_SIZE];
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
struct ShellCommand parse_input(char c[]);
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
    // struct ShellCommand c;

    chdir(getenv("HOME"));
    //printf("\n%s$ ", get_directory());
    while(1)
    {
        printf("\n%s$ ", get_directory());
        input = get_user_input();

        execute_command(parse_input(input));
    }

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
struct ShellCommand parse_input(char c[])
{
    int is_first_in_string = 1; // this to ensure that the first word is treated as a command

    struct ShellCommand line;  // struct that will be created with parsed input
    char *token; // = malloc(MAX_SIZE);
    const char n[1] = " ";  // splits token by spaces
    token = strtok(c, n);

    // commands[0] = malloc(sizeof(token)); // allocates memory
    line.commands[0] = malloc(sizeof(token));
    strcpy(line.commands[0], token);



    int i, j;   // counters to be used in loop
    i = 1; j = 0; // i is set at one because commands[0] is already assigned
    while (token != NULL)
    {
        line.commands[i] = malloc(sizeof(token)); // allocates memory
        line.arguments[j] = malloc(sizeof(token));   // allocates memory

        if(strchr(token, '-') != NULL)  // this is causing a segmentation fault
        {
            strcpy(line.commands[i], token);
            // printf("Token: %s contains -\n", line.commands[i]);
            i++;
        }
        if(strchr(token, '-') == NULL && !is_first_in_string)
        {
            strcpy(line.arguments[j], token);
            // printf("Token: %s does not have a dash.\n", line.arguments[j]);
            j++;
        }
        token = strtok(NULL, n);
        is_first_in_string = 0;
    }
    i++;
    line.commands[i] = NULL;
    //
    // printf("Commands: ");
    // for (int n = 0; n < i; n++)
    // {
    //     printf("%s, ", line.commands[n]);
    //
    // }
    // printf("\nArguments: ");
    // for (int k = 0; k < j; k++)
    // {
    //     printf("%s, ", line.arguments[k]);
    // }
    // printf("\n");

    return line;
}

/************************************
* Function that executes the commands
* that the user inputs.
************************************/
void execute_command(struct ShellCommand s)
{
    if(strcmp(s.commands[0], "cd") == 0)
    {
        chdir(s.arguments[0]);
    }
    if(strcmp(s.commands[0], "exit") == 0)
    {
        printf("Exited\n");
        exit(0);
    }
    if(strcmp(s.commands[0], "pwd") == 0)
    {
        printf("%s", get_directory());
    }
    if(strcmp(s.commands[0], "ls") == 0)
    {
        printf("YOU TRIED TO \n");

        s.commands[0] = "/bin/ls";
        pid_t pid = fork();
        if (pid == 0)
            execv(s.commands[0], s.commands);
    }
}

/***********************************
* This function will not be
* implemented. It is just used to
* test how strings can be split up.
*
* These tests will be used to
* implement the parse_input()
* function.
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
