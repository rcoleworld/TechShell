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
#include <wait.h>
#include <errno.h>

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
    char *commands[MAX_SIZE];   //needs to be a string not array of string
    char *arguments[MAX_SIZE];
    char *input;
    char *output;
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
    char* home_directory = getenv("HOME");

    chdir(getenv("HOME"));

    while(1)
    {
        // this shorthands home directory
        if (strcmp(get_directory(),home_directory) == 0)
            printf("\n~$ ");
        else
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
    char* input = malloc(MAX_SIZE);

    fgets(input, MAX_SIZE, stdin);  // stores user input from stdin to char input
    input = strtok(input,"\n");

    fflush(stdin);

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
    struct ShellCommand line;  // struct that will be created with parsed input
    char *token; // = malloc(MAX_SIZE);
    const char n[1] = " ";  // splits token by spaces

    token = strtok(c, n);
    line.input = NULL;
    line.output = NULL;
    // handles the case of no input
    if (token == NULL)
    {
        line.commands[0] = "empty";
        return line;
    }

    line.commands[0] = malloc(sizeof(token));
    strcpy(line.commands[0], token);
    int i, j;   // counters to be used in loop
    i = 1; j = 0; // i is set at one because commands[0] is already assigned

    while (token != NULL)
    {
        line.commands[i] = malloc(sizeof(token)); // allocates memory
        line.arguments[j] = malloc(sizeof(token));   // allocates memory

        if (strchr(token, '-') != NULL)
        {
            strcpy(line.commands[i], token);
            i++;
        }

        // adds everything to arguments
        strcpy(line.arguments[j], token);
        j++;
        token = strtok(NULL, n);
    }

    // THIS IS WHAT FIXED EVERYTHING
    line.arguments[j] = NULL;

    // This handles a input and out put file parsing
    for (int n = 0; n < j; n++)
    {
        // output file will come after ">"
        if (strcmp(line.arguments[n], ">") == 0)
        {
            line.output = line.arguments[n+1];
            // printf("outputfile = %s\n", line.output);
        }
        // input file will come after "<"
        else if(strcmp(line.arguments[n], "<") == 0)
        {
            line.input = line.arguments[n+1];
            // printf("inputfile = %s\n", line.input);
        }
    }

    return line;
}

/************************************
* Function that executes the commands
* that the user inputs.
************************************/
void execute_command(struct ShellCommand s)
{

    if (strcmp(s.commands[0], "\n") == 0)
    {
        printf("\n");
    }

    else if (strcmp(s.commands[0], "cd") == 0)
    {
        if (chdir(s.arguments[1]) == -1)
            printf("error: %d (%s)\n", errno, strerror(errno)); // handles errors
    }


    else if (strcmp(s.commands[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(s.commands[0], "pwd") == 0)
    {
        printf("%s", get_directory());
    }

    /********************************************
    * Figuring out how to get this part to work
    * caused me to rethink my entire existence.
    * Null characters are the devil.
    ********************************************/
    else
    {

        // fork
        pid_t pid = fork();
        // new argument variable to handle redirection
        char **inoutargs = malloc(MAX_SIZE);

        // in child process
        if (pid == 0)
        {
            // to determine if no input or output
            if (s.input == NULL && s.output == NULL)
            {
                if (execvp(s.commands[0], s.arguments) == -1)
                    printf("error: %d (%s)\n", errno, strerror(errno));
                exit(0);
            }
            else
            {
                // loops through arguments
                for (int i = 0; i < sizeof(s.arguments); i++)
                {
                    // reads files
                    if (strcmp(s.arguments[i], "<") == 0)
                    {
                        FILE* infile = fopen(s.input, "r");
                        dup2(fileno(infile), 0);
                        fclose(infile);
                    }
                    // writes to files
                    else if(strcmp(s.arguments[i], ">") == 0)
                    {
                        FILE* outfile = fopen(s.output, "w");
                        dup2(fileno(outfile), 1);
                        fclose(outfile);
                        execvp(s.commands[0], inoutargs);
                    }
                    else
                        inoutargs[i] = s.arguments[i];
                }
                execvp(s.commands[0], inoutargs);
            }
            exit(0);
        }
        // parent process
        else
        {
            wait(0);
        }
    }
}
