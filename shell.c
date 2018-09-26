#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define PROMPT_SIZE 25
#define CMD_SIZE 128
#define OUTPUT_SIZE 128

int main(int argc, char **argv) {
        // Get the custom prompt, if there is one:
        char prompt[PROMPT_SIZE];
        char curr_cmd[CMD_SIZE];
        char cmd_output[OUTPUT_SIZE];

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
                fgets(curr_cmd, CMD_SIZE, stdin);

                // fgets appends the newline character which we do not want
                if (strlen(curr_cmd) > 1) {
                        curr_cmd[strlen(curr_cmd) - 1] = '\0';
                }
 
                // No string switches in C, prepare for endless if-else ladder 
                if (strcmp(curr_cmd, "exit") == 0) {
                        exit(EXIT_SUCCESS);
                } else if (strcmp(curr_cmd, "pwd") == 0) {
                        getcwd(cmd_output, OUTPUT_SIZE);
                        printf("%s\n", cmd_output); 
                } else {
                        printf("Command not supported or located\n");
                } 
        }
        
        return 0;
}
