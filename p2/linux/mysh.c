#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

char *prompt_message = "mysh # ";
char error_message[30] = "An error has occurred\n";
// tokens for the real command without redirection
char *tokens[512];
// outputs for the redirection
char *outputs[512];
// 1, if it is batch mode; otherwise, 0
int isBatch = 0;
// 1, if redirection; otherwise, 0
int isRedir = 0;
// counter recorded for the command in the history
int counter = 0;

// store the command into history with an unique id
struct record {
	int id;
	char *command;
} cmdRec[20];

// point to the first command in the history array
struct record *front = NULL;
// point to the last command in the history array
struct record *rear = NULL;

void printPrompt() {
	if (!isBatch) {
		write(STDOUT_FILENO, prompt_message, strlen(prompt_message)); 
	}
}

void printError() {
	write(STDERR_FILENO, error_message, strlen(error_message));	
}

// Split the line into tokens based on the delimiter ' ' and return the number 
// of tokens.
int splitLine(char *line, char *words[]) {
	int numWords = 0;
	words[0] = strtok(line, " ");

	while (words[numWords] != NULL) {
		words[++numWords] = strtok(NULL, " ");
	}
	return numWords;
}

void runCommand(char *cmd) {
		isRedir = 0;
		char *cmdDup = strdup(cmd);
		char *line = strcat(cmdDup, "\n");
		char *special = strchr(line, '>');
		char *preSpecial, *postSpecial;
		int numTokens;
		if (special != NULL) {
			// more than one '>' in the command
			if (special != strrchr(line, '>')) {
				printError();
				return;
			}
			preSpecial = strtok(line, ">");
			postSpecial = strtok(NULL, "\n");
			// error command of redirection
			if (splitLine(postSpecial, outputs) != 1) {
				printError();
				return;
			}
			numTokens = splitLine(preSpecial, tokens);
			// command started with '>'
			if (numTokens == 0) {
				printError();
				return;
			}
			// '!' cannot use with '>'
			if (tokens[0][0] == '!') {
				printError();
				return;
			}
			close(STDOUT_FILENO);
			int outFD = open(outputs[0], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
			// cannot open the output file
			if (outFD < 0) {
				printError();
				return;
			}
			isRedir = 1;
		} else {
			numTokens = splitLine(strtok(line, "\n"), tokens); 
			// an empty command
			if (numTokens == 0) {
				return;
			}
		}

		// internal commands
		// ! and !(#)
		// assume this kind of command don't go into the history

		if (tokens[0][0] == '!') {
			if (numTokens > 2) {
				printError();
				return;
			}
                        // id of the command as a string
			char *countStr;
                        // id of the command as a integer
			int countInt;
			struct record *tmp = front;
			char *pEnd;
			// no records in the history
			if (rear == NULL || front == NULL || (strlen(tokens[0]) > 1 &&
						numTokens == 2)) {
				printError();
				return;
			} 
			if (numTokens == 1) {
				countStr = strtok(strdup(tokens[0]), "!");
				if (countStr == NULL) {
					runCommand(rear->command);
					return;
				} else {
					countInt = strtol(countStr, &pEnd, 10);
					if (errno == ERANGE || *pEnd != '\0') {
						printError();
						return;
					}
				}
			} else {
				countInt = strtol(tokens[1], &pEnd, 10);
				if (errno == ERANGE || *pEnd != '\0') {
					printError();
					return;
				}
			} 
			// operand of ! out of bound
			if (countInt < front->id || countInt > rear->id) {
				printError();
				return;
			}
			tmp += (countInt - front->id);
			runCommand(tmp->command);
			return;
		}
		
		// similar to circular array implementation of queue
		if (rear == NULL) {
			rear = cmdRec;
		} else {
			if (rear == &cmdRec[19]) {
				rear = cmdRec;
			} else {
				++rear;
			}
		}
		rear->id = ++counter;
		rear->command = strdup(cmd);
		if (rear == front) {
			if (front == &cmdRec[19]) {
				front = cmdRec;
			} else {
				++front;
			}
		}
		if (front == NULL) {
			front = cmdRec;
		}

	
		// internal commands
		// exit
		if (strcmp(tokens[0], "exit") == 0) {
			if (numTokens > 1) {
				printError();
				return;
			} else {
				exit(0);
			}
		}

		// history
		if (strcmp(tokens[0], "history") == 0) {
			if (numTokens > 1) {
				printError();
				return;
			} 
			if (rear != NULL) {
				struct record *tmpFront = front;
				struct record *tmpRear = rear;
				while (tmpFront != tmpRear) {
					printf("%d %s\n", tmpFront->id, tmpFront->command);
					if (tmpFront == &cmdRec[19]) {
						tmpFront = cmdRec;
					} else {
						++tmpFront;
					}
				}
					printf("%d %s\n", tmpFront->id, tmpFront->command);
	
			}
			return;
		}	

		int rc = fork();

		if (rc == 0) {
			// child
			execvp(tokens[0], tokens);
			printError();
			exit(1);
		} else if (rc > 0) {
			// parent
			wait(NULL);
		} else {
			printError();
		}
}	

int main(int argc, char *argv[]) {
	FILE *inFile = NULL;
	char line[1024];
	
	// an incorrect number of command line arguments to the shell program
	if (argc == 2) {	
		isBatch = 1;
		inFile = fopen(argv[1], "r");
		if (inFile == NULL) {
			printError();
			exit(1);
		}
	} else if (argc == 1) {
		inFile = stdin;
	} else {
		printError();
		exit(1);
	}

	printPrompt();
	
	while (fgets(line, 1024, inFile)) {

		// a very long command line (over 512 characters)
		if (strlen(line) > 513) {
			if (isBatch) {
				write(STDOUT_FILENO, line, 512);
				write(STDOUT_FILENO, "\n", 1);
			}
			printError();
			printPrompt();
			continue;
		}

		if (isBatch) {
			write(STDOUT_FILENO, line, strlen(line));
		}
	
		int stdoutCopy = dup(STDOUT_FILENO);
		char *cmd = strtok(line, "\n");
		runCommand(cmd);
		if (isRedir) {
			dup2(stdoutCopy, STDOUT_FILENO);
		}
		close(stdoutCopy);
		printPrompt();
	}		
	return 0;
}
