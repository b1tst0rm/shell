#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define PROMPT_SIZE 25

int main(int argc, char **argv) {
        // Get the custom prompt, if there is one:
        char prompt[PROMPT_SIZE];

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

        printf("Prompt is: %s\n", prompt);
        return 0;
}
