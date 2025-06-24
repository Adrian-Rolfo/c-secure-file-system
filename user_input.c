#include <stdio.h> /*getchar(), scanf()*/
#include "user_input.h" /*void read_string_input(),
void read_username_password_input(), void clear_input_buffer()*/
#include "debug.h"

/*#define DEBUG*/

/****************
AUTHOR: Adrian Rolfo 24818686

Description:

****************/
void read_char_input(char* c) {
    DEBUG_PRINT("\nDEBUG: in function read_char_input\n");
    scanf(" %c", c);
    clear_input_buffer();
}

/****************
AUTHOR: Adrian Rolfo 24818686

Description:

****************/
void read_string_input(char str[], int str_max_size) {
    DEBUG_PRINT("\nDEBUG: in function read_string_input\n");
    int c;
    int index = 0;

    /* Ignore leading whitespace*/
    while ((c = getchar()) != EOF && (c == ' ' || c == '\t' || c == '\n'));

    /* Read characters until newline or end of string */
    while (c != '\n' && c != EOF && index < str_max_size - 1) {
        str[index++] = c;
        c = getchar();
    }
    str[index] = '\0';

    /* Check if there are characters remaining in the input buffer before 
    clearing */
    if (c != '\n' && c != EOF) {
        clear_input_buffer();
    }
}

/****************
AUTHOR: Adrian Rolfo 24818686

Description:

****************/
void read_username_password_input(char str[], int str_max_size) {
    DEBUG_PRINT("\nDEBUG: in function read_username_password_input\n");
    int c = getchar();
    int index = 0;

    /* Read characters until newline or end of string */
    while (c != '\n' && c != EOF && index < str_max_size - 1) {
        str[index++] = c;
        c = getchar();
    }
    str[index] = '\0';

    /* Check if there are characters remaining in the input buffer before 
    clearing */
    if (c != '\n' && c != EOF) {
        clear_input_buffer();
    }
}

/****************
AUTHOR: Adrian Rolfo 24818686

Description:

****************/
void clear_input_buffer() {
    DEBUG_PRINT("\nDEBUG: in function clear_input_buffer\n");
    int newline;
    while ((newline = getchar()) != '\n' && newline != EOF);
}
