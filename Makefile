CC = gcc

all: shell.c
	gcc -o shell shell.c

clean:
	$(RM) shell
