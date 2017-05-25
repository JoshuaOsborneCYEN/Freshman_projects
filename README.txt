Joshua Osborne - Assignment 1 Tech Shelll READ ME
CSC 345 - Operating System
Dr. Box
25 April 2017

DESCRIPTION:
        This program emulates a simple shell. It will parse through entered commands, tokenize them, and run them as commands like a normal shell. It will handle the special characters: “<”, “>”, and “>>”, redirecting appropriately, and will also handle "|" and pipe commands together. It handles the built-in commands “pwd” “exit” and “cd”, and can also store and retrieve data in a hash table using the built in commands "set" and "list". It handles not-built-in commands by forking and executing. Space, tab, and vertical tab are the optional delimiters for tokens. The shell however cannot pipe and redirect at the same time, and it also cannot pipe more than once.

PROCESS:
        *TOKENIZING: Shell receives the entered commands, and will first parse through them and separate them by tokens. It does this by using space, tab, and vertical tab as delimiters. Non-special characters will be referred to as “normal tokens”. Special characters (i.e. redirection characters) will be referred to as “special tokens”. When special characters are entered, such as “<”, “>” and “>>”, it will store a number associated with the specific character in the variable scf. Once the normal tokens are created, it will store them in a character array arr. If scf is not equal to zero (meaning it holds a special token), it will then store that token in the array.
        *CATEGORIZING: Shell then passes arr (now the collection of tokens) to the checker function. The checker function will categorize the tokens based on whether the token is built-in or not-built-in. It checks the first token of arr for three possible conditions: “pwd”, “exit”, “cd”, "set", and "list". If the first token is “pwd”, it will then call the prwd function which will print the working directory. If the first token is “exit”, it will simply call an exit within the checker function and the program will exit. If the first token is “cd”, it will call the chd function and change the directory based on the path the user entered after. The "set" command, followed by two strings separated by delimiters, will store the key-value pair into the hash table using the "hashInsert" function. The "list" command will list all the key-value pairs in the hash table using its display() function. The hash table implementation can be found in HashTable.h. If the first token is none of these, it will then call the notbuilt function.
        *EXECUTING NON-BUILT: Once the notbuilt function is called, the shell will begin by spawning a child process. It will then execute the args entered if there are no special characters. If there are special characters, it will check which character is entered and use fopen() on the file depending on which character. Once fopen() has occurred, it will spawn the process afterwards. If there is a pipe, the program will fork twice. The first child will take the tokens from the start to the pipe as commands, redirect standard out to a pipe, and execute. The second child will read standard in from the pipe and execute commands from the token after the pipe to the end of the token set. Its output is then printed and the pipe is closed. 

EXECUTING:
	To compile and run the program, simply g++ the code TechShellFinal.c. The makefile is set up so using the command "make" will compile the file, producing an executable named "techShell".

ERROR CATCHING:
 Shell is equipped to handle the errors:
1)        If the command entered is not recognized
2)        If the child process fails to be created
3)        If the command fails to execute
4)        If in case of “cd” incorrect path entered
5)	  If the pipe forking or executions fail
6) 	  If dup() fails in pipe

LIMITATIONS:
Shell does not handle piping and redirects at the same time.
Shell does not pipe more than once
Shell can only parse a limited number of tokens, reasonably high, and only with a certain amount of characters.

SAMPLES TO ENTER:
        “ls -l”
        “ls > ls.out”
        “ls>ls.out”
        “wc < ls.out”
        “ls >> ls.out”
        “pwd”
        “cd ..”
	"ls -l | wc"
	"ls | more"
	"echo hello | wc"
	"set keyone valueone"
	"set keytwo valuetwo"
	"list"
        “exit”
