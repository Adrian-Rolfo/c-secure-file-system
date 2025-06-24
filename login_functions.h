#ifndef LOGIN_FUNCTIONS_H
#define LOGIN_FUNCTIONS_H
#include "definitions.h" /*list_t, user_t*/

/*prints the menu*/
void print_menu();

/*logs in the user*/
int user_login(const list_t* users, user_t* logged_in_user);

/*adds a new user to the list*/
void user_add(list_t* users);

/*checks if the inputted username is valid*/
int check_valid_username(char *str, int max_str_size, list_t* users);

/*checks if inputted username is valid*/
int check_valid_password(char str[], int max_str_size);

/*prints the about page*/
void print_about();

#endif
