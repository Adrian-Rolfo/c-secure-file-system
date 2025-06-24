#makefile for generating main.out
#Author Lucy Nelson 25255555

CC = gcc
CFLAGS = -Wall -Werror -ansi -lm

main.out: generic_list.o login_functions.o user_functions.o sorting.o compression.o encryption.o user_input.o file_functions.o debug.o
	$(CC) $(CFLAGS) -o main.out generic_list.o login_functions.o user_functions.o sorting.o compression.o encryption.o user_input.o file_functions.o debug.o main.c

generic_list.o: generic_list.c generic_list.h definitions.h debug.h
	$(CC) $(CFLAGS) -c -o generic_list.o generic_list.c

login_functions.o: login_functions.c login_functions.h generic_list.h user_input.h definitions.h debug.h
	$(CC) $(CFLAGS) -c -o login_functions.o login_functions.c

user_functions.o: user_functions.c definitions.h user_functions.h debug.h generic_list.h sorting.h encryption.h compression.h user_input.h file_functions.h
	$(CC) $(CFLAGS) -c -o user_functions.o user_functions.c

sorting.o: sorting.c sorting.h definitions.h debug.h
	$(CC) $(CFLAGS) -c -o sorting.o sorting.c

compression.o: compression.c compression.h debug.h
	$(CC) $(CFLAGS) -c -o compression.o compression.c

encryption.o: encryption.c definitions.h encryption.h debug.h
	$(CC) $(CFLAGS) -c -o encryption.o encryption.c

user_input.o: user_input.c user_input.h debug.h
	$(CC) $(CFLAGS) -c -o user_input.o user_input.c

file_functions.o: file_functions.c definitions.h file_functions.h generic_list.h compression.h encryption.h debug.h
	$(CC) $(CFLAGS) -c -o file_functions.o file_functions.c

debug.o: debug.c debug.h
	$(CC) $(CFLAGS) -c -o debug.o debug.c

clean:
	rm generic_list.o login_functions.o user_functions.o sorting.o compression.o encryption.o user_input.o file_functions.o debug.o main.out
