/*
 * Joshua Osborne ~ CSC 345: Assignment 1: Tech Shell
 * 4-22-2017
 * techShell -- Assignment1TechShell.c
 * This code acts as a basic shell. It handles simple Linux commands and redirects. It handles piping. Will return errors if invalid Linux commands are used.
 * Piping assistance from https://www.quora.com/Unix-How-can-I-write-a-code-for-PIPE-in-C-shell-script-python
 * Hash table assistance from https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "HashTable.h"

#define MAXLINE 80
#define CHUNK 10
#define WHITE "\t \n"
#define MAXARG 20

char str[128];
static char *ptr; //for parsing tokens
static char *arr[20]; // Token storage
static int j; //argument counter
static int scf = 0; // Flag for handling special characters
static char q = 0; // Assists in handling special characters during execution

int notbuilt(char *args[]){ // Not built-in command handler

	pid_t pid;
	pid = fork();
	int execstat; //stores info about execution status

	if(pid < 0)
	{ //fork failure
		fprintf(stderr, "Fork failed");
		exit(-1);
	}

	else if (pid == 0) //child process
	{
		//Check for special character tokens, do redirections if necessary
		for(q = 0; args[q] != '\0'; q++)
		{    
			if(args[q] ==  "<" || args[q] == ">" || args[q] == ">>")
			{ // Special token execution
				if(args[q] == ">"){ // > execution
					FILE *file;
					q++; // file name
					//write from stdout to new file
					file = freopen(args[q], "w+", stdout);
					q--; // back to special char
					args[q] = '\0';
					execstat = execvp(args[0], args);
					if(execstat != 0){
						perror("Error:");
						_exit(0);
					}

					fclose(file);
					q--;

				}

				if(args[q] == "<"){ // < execution
					q++;
					//read from file as stdin
					freopen(args[q], "r", stdin);
					q--;
					args[q] = '\0';
					execstat = execvp(args[0], args);
					if(execstat != 0){
						perror("Error:");
						_exit(0);
					}


				}

				if(args[q] == ">>"){ // >> execution
					q++;
					//append to file from stdout
					freopen(args[q], "a", stdout);
					q--;
					args[q] = '\0';
					execstat = execvp(args[0], args);
					if(execstat != 0){
						perror("Error:");
						_exit(0);
					}
				}
			}

			//piping
			if(args[q] == "|")
			{
				int i, status; //loop index, status of child execution
				int pipeVar[2]; //for pipe
				//holds argument list for children
				char* argl[MAXARG];
				if(pipe(pipeVar) != 0)
				{
					perror("Pipe error");
					exit(-1);
				} //pipe creates two file decriptors: one for read, one for write end

				int pipe_pid1 = fork();
				if (pipe_pid1 < 0) //failed fork
				{
					fprintf(stderr, "Fork failed");
					exit(-1);
				}
				else if (pipe_pid1 == 0) //child process
				{
					//pipe[0] is read end, pipe[1] is write end
					i = 0;
					//pack arguments into new list
					for (i = 0; i < q && i < MAXARG; i++)
					{
						argl[i] = args[i];
					}
					//append with NULL
					argl[i] = NULL;
					//close read end
					close(pipeVar[0]);
					//write from stdout into the pipe
					if(dup2(pipeVar[1], 1) == -1)
					{
						perror("Child 1 dup error");
					}
					//close write end
					close(pipeVar[1]);
					execstat = execvp(argl[0], argl);
					if (execstat == -1)
					{
						perror("Pipe child 1 (first) execution failed");
						_exit(0);
					}
					_exit(1);
				}
				int pipe_pid2 = fork();
				if (pipe_pid1 < 0) //failed fork
				{
					fprintf(stderr, "Fork failed");
					exit(-1);
				}
				else if (pipe_pid2 == 0) //other child process
				{	
					//start after pipe
					i = q + 1;
					//put arguments into arg list
					for (int k = 0; i <= j && k < MAXARG; i++, k++)
					{
						argl[k] = args[i];
					}
					//close write end
					close(pipeVar[1]);
					//read output from child, mapped as input
					if(dup2(pipeVar[0], 0) == -1)
					{
						perror("Child 2 dup error");
					}
					//close read end of pipeVar
					close(pipeVar[0]);
					//after all pipes completed, execute final argument
					execstat = execvp(argl[0], argl);
					if (execstat == -1)
					{
						perror ("Pipe child 2 (second) execution failed");
						_exit(0);
					}
					_exit(1);
				}
				close(pipeVar[0]);
				close(pipeVar[1]);
				while (wait(&status) != pipe_pid1);
				_exit(0);
				break; //quit loop; we're done here after executing pipe
			}
		}
		if(args[0] != "<" || args[0] != ">" || args[0] != ">>")
		{ 
			//normal execution
			execstat = execvp(args[0], args);
			if(execstat == -1)
			{
				perror("Error");
			}
		}
		_exit(1);

	}

	else
	{
		//parent waits for child completion
		wait(NULL);
	}

}

int chd(char *args[]){ // CD - Change directory 
	char cwd[1024];
	if(chdir(args[1]) != 0)
	{ 
		perror("\n chdir() error()");
	}
	else
	{
		if(getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("\n getcwd() error");
		}
		else
		{
			printf("\nAfter changing, current working directory is: %s\n", cwd);
		}
	}
}

int prwd(char *args[])
{ // PWD - Print working directory
	char pwd[1024];
	if(getcwd(pwd, sizeof(pwd)) == NULL)
	{
		perror("\ngetpwd() error");
	}
	else
	{
		printf("\nCurrent working directory is: %s\n", pwd);
	}
}

void checker(char *args[])
{ //Built-in vs Not built-in detector

	//Built-in

	if(strcmp(args[0], "cd") == 0)
	{ // cd command
		chd(args);
	}

	else if(strcmp(args[0], "pwd") == 0)
	{ // pwd command
		prwd(args);
	}

	else if(strcmp(args[0], "exit") == 0)
	{ // exit command
		exit(3);
	}

	else if(strcmp(args[0], "set") == 0)
	{ // set command
		hashInsert(args[1], args[2]);
	}

	else if(strcmp(args[0], "list") == 0)
	{ // list command
		hashDisplay();
	}

	//Not built-in

	else
	{
		notbuilt(args);
	}    
}



char *tokenizer(char* string) // Token parser 
{
	char *p;
	if(string != NULL) // Beginning of token & if content still to parse
	{
		ptr=string;
		p=string;
	}
	else
	{
		if(*ptr == '\0') // No content to parse
			return NULL;

		p=ptr;
	}

	while(*ptr != '\0')
	{
		//take care of quotation marks as a single token
		if(*ptr == '"')
		{
			ptr++;
			while(*ptr != '"')
			{
				ptr++; //get to the next quotation mark
			}
			ptr++; //get past quotation mark
			*ptr = '\0'; //End of token
			ptr++;
			return p; //Done with token, return the string
		}


		if(*ptr == ' ' || *ptr == '\t' || *ptr == '\v' || *ptr == '<' || *ptr == '>' || *ptr ==  '|') // Delimiters & special characters
		{
			if(ptr == p && (*ptr == ' ' || *ptr == '\t' || *ptr == '\v')) // Catch multiple spaces, tabs, vertical tabs
			{
				p++;

			} 
			else
			{
				if(*ptr == '|'){ // Flag | token
					scf = 1;
				}

				if(*ptr == '<'){ // Flag < token
					scf = 2;
				}

				if(*ptr == '>'){ // Flag >, >> token
					*ptr='\0';
					ptr++;
					scf = 3;
					if(*ptr == '>'){
						scf=4;
					}
					else{
						ptr--;
					}
				}

				*ptr='\0'; // End of token
				ptr++;
				return p;
			}
		}

		ptr++;
	}
	return p; // Beginning of token to null 
}

int main()
{
	int i;
	j = 0;
	char *p_str, *token;
	int temp = 0;

	printf("\nEnter string to tokenize:  ");
	while(1)
	{
		j = 0;
		temp = 0;
		printf("%% ");
		scanf(" %[^\n]", str);

		for(i=1, p_str = str; ; i++, p_str = NULL)
		{
			//get the next token
			token = tokenizer(p_str);
			if(token == NULL)
				break;

			if(*token != '\0')
			{ // Print token & token number
				//put token into args array
				arr[j] = token;
				j++;
			}

			if(scf != 0){// Special character flags printed
				//scf is flagged in tokenizer function
				if(scf == 1){
					arr[j] = "|";
				}
				if(scf==2){
					arr[j] = "<";
				}
				if(scf==3){
					arr[j] = ">";
				}
				if(scf==4){
					arr[j] = ">>";
				}
				j++;
				scf = 0;
			}
		}
		arr[j] = '\0'; //NULL
		//check for built in vs non built in args then exec
		checker(arr);
		printf("\n");
	}
}
