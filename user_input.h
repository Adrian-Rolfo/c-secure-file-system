#ifndef USER_INPUT_H
#define USER_INPUT_H

/* reads character input and clears input buffer*/
void read_char_input(char* c);

/*reads general string input and clears input buffer */
void read_string_input(char str[], int str_max_size);

/*reads input for the username and password and clears input buffer*/
void read_username_password_input(char str[], int str_max_size);

/*clears input stream of any unwanted characters*/
void clear_input_buffer();

#endif
