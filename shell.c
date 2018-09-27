#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT_SIZE 25
#define IN_SIZE 128
#define CMD_SIZE 64
#define OUTPUT_SIZE 128
#define MAX_ARG_SIZE 64
#define MAX_ARGS 5

int run_command(char* input);

int main(int argc, char **argv) 
{
        char prompt[PROMPT_SIZE];
        char curr_input[IN_SIZE];
        char curr_input_bak[IN_SIZE];
        char cmd_output[OUTPUT_SIZE];
        char curr_cmd[CMD_SIZE];

        // Get the custom prompt, if there is one:
        if (argc > 1) {
                if (strcmp(argv[1], "-p") == 0) {
                        strncpy(prompt, argv[2], PROMPT_SIZE);
                } else {
                        printf("USAGE: ./shell {-p %cdesired prompt%c}"
                               " {-h/-help}\n", '"', '"');
                        exit(EXIT_FAILURE);
                }
        } else {
                strcpy(prompt, "308sh> ");
        }

        while(1) {
                printf("%s", prompt);
                fgets(curr_input, IN_SIZE, stdin);

                // Check to see if any background processes have exited
                int bg_status;
                int exited_pid = waitpid(-1, &bg_status, WNOHANG);
                if (exited_pid > 0) {
                        printf("Background process with PID of %d has "
                               "exited with code %d\n", exited_pid, bg_status);
                }

                // fgets() appends the newline character which is undesired
                if (strlen(curr_input) > 1) {
                        curr_input[strlen(curr_input) - 1] = '\0';
                }

                strcpy(curr_input_bak, curr_input);
                strcpy(curr_cmd, strtok(curr_input, " "));
 
                // No string switches in C, prepare for endless if-else ladder 
                if (strcmp(curr_cmd, "exit") == 0) {
                        exit(EXIT_SUCCESS);
                } else if (strcmp(curr_cmd, "pwd") == 0) {
                        getcwd(cmd_output, OUTPUT_SIZE);
                        printf("%s\n", cmd_output); 
                } else if (strcmp(curr_cmd, "pid") == 0) {
                        printf("%d\n", getpid());
                } else if (strcmp(curr_cmd, "ppid") == 0) {
                        printf("%d\n", getppid());
                } else if (strcmp(curr_cmd, "cd") == 0) {
                        char dir[MAX_ARG_SIZE];
                        char *tok = strtok(NULL, " ");
                        if (tok == NULL) {
                                // Change to user's home directory
                                strcpy(dir, getenv("HOME"));
                        } else {
                                strcpy(dir, tok);
                        }
                        int resp = chdir(dir);
                        if (resp < 0) {
                                perror("ERROR");
                        }
                } else if (strcmp(curr_cmd, "set") == 0) {
                        char *tok1 = strtok(NULL, " ");
                        char *tok2 = strtok(NULL, " ");
                        int resp;
                        if (tok1 == NULL) {
                                printf("USAGE: set <var>"
                                       " <value or empty to reset>\n");
                        } else if (tok2 == NULL) {
                                // Reset <var>
                                resp = unsetenv(tok1);
                                if (resp < 0) {
                                        perror("ERROR");
                                } else {
                                        printf("Var %s reset.\n", tok1);
                                }
                        } else {
                                // Set <value> to <var>
                                resp = setenv(tok1, tok2, 1);
                                if (resp < 0) {
                                        perror("ERROR");
                                } else {
                                        printf("Var %s set to %s.\n", 
                                               tok1, getenv(tok1));
                                }
                    
                        }
                } else if (strcmp(curr_cmd, "get") == 0) {
                        char *tok1 = strtok(NULL, " ");
                        if (tok1 == NULL) {
                                printf("Provide a variable to get.\n");
                        } else {
                                printf("%s\n", getenv(tok1));
                        }
                } else if (strcmp(curr_cmd, "\n") == 0) { 
                        // Do nothing for empty command
                } else {
                        // Not a built-in command, attempt to run executable
                        int resp = run_command(curr_input_bak);
                } 
        }
        
        return 0;
}

int run_command(char* input)
{
        int is_background = 0;

        // Determine number of arguments
        char *args[MAX_ARGS + 1]; // Argument list, plus 1 for the NULL

        // Build list of arguments for execvp
        char *tok = strtok(input, " ");

        int i = 0;
        while(tok != NULL && i < MAX_ARGS) {
                args[i] = malloc(MAX_ARG_SIZE);
                strcpy(args[i], tok);
                tok = strtok(NULL, " ");
                i++;
        }

        // Determines if process should be set to background and removes &
        if (strcmp(args[i - 1], "&") == 0) {
                is_background = 1;
                args[i - 1] = NULL;
                printf("BACKGROUND DETECTED\n");
        } else {
                args[i] = NULL;
        }

        i = 0;
        while (args[i] != NULL) {
                printf("ARG: |%s|\n", args[i]);
                i++;
        }
       
        pid_t pid;
        int status;

        if ((pid = fork()) < 0) {
                perror("FORK ERROR");
        } else if (pid == 0) {
                // Child process here
                printf("\nChild PID: %d\n", getpid());
                if (!is_background) {
                        printf("-------------------------\n");
                }
                if (execvp(args[0], args) < 0) {
                        perror("EXECVP ERROR");
                        printf("-------------------------\n");
                        printf("%d terminated.\n", getpid());
                }
        } else {
                // Parent process here
                if (!is_background) {
                        while (wait(&status) != pid) {
                                // Block until child process completes
                        }
                }    
                
                if (WIFEXITED(status)) {
                        printf("-------------------------\n");
                        int exit_status = WEXITSTATUS(status);
                        if (exit_status == EXIT_SUCCESS) {
                                printf("Exited successfully with code"
                                       " %d\n", exit_status);
                        } else {
                                printf("Exited with code %d\n",
                                       exit_status);
                        }
                } else if (WIFSIGNALED(status)) {
                        printf("-------------------------\n");
                        int term_status = WTERMSIG(status);
                        printf("Terminated with code %d\n", term_status);
                } else if (WIFSTOPPED(status)) {
                        printf("-------------------------\n");
                        int stop_status = WSTOPSIG(status);
                        printf("Stopped with code %d\n", stop_status);
                }
        }
        return 0;
}
