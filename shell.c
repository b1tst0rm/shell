#include <stdio.h>
#include <string.h>
#include <unistd.h> // for getopt

int main(int argc, char **argv) {
    // Get the custom prompt, if there is one:
    char prompt[25];

    if (argc > 1) {
        if (argv[1] == "-p") {
            strcpy(prompt, argv[2])
        }
        strcpy(prompt, argv[1]);
    } else {
        strcpy(prompt, "308sh> ");
    }

    printf("Prompt is: %s", prompt);
    return 0;
}
