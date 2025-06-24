#include <stdio.h> /*printf(), fopen(), fseeks(), ftell(), fclose()*/
#include <stdlib.h>
#include <string.h> /*strcmp(), strcopy(), strlen()*/
#include "definitions.h"
#include "login_functions.h"
#include "generic_list.h"
#include "user_input.h"
#include "debug.h"

/*************************************************************
Author/s:

Description:

Input/s:

Output/s:
*************************************************************/
void print_menu() {
    DEBUG_PRINT("\nDEBUG: In function print_menu\n");
    if(debug_mode == 1) {
        printf("\nWeclome to file storage program, what would you like to do?\n"
                "1. Log-in\n"
                "2. Create New User\n"
                "3. About\n"
                "4. Exit\n"
                "5. Display Users\n"
                "Enter your choice> ");    
    }
    else {
        printf("\nWeclome to file storage program, what would you like to do?\n"
                "1. Log-in\n"
                "2. Create New User\n"
                "3. About\n"
                "4. Exit\n"
                "Enter your choice> ");
    }
}


/*************************************************************
Author/s: 
Lucy Nelson 25255555
Adrian Rolfo 24818686

Description:

Input/s:

Output/s:
*************************************************************/
int user_login(const list_t* users, user_t* logged_in_user) {
    DEBUG_PRINT("\nDEBUG: In function user_login\n");
    char username_dummy[MAX_NAME_SIZE], 
         password_dummy[MAX_NAME_SIZE];

    printf("Enter Username> ");
    read_username_password_input(username_dummy, MAX_NAME_SIZE);

    printf("Enter Password> ");
    read_username_password_input(password_dummy, MAX_NAME_SIZE);

    node_t* current = users->head;
    if(current == NULL) {
        printf("Error: no users exist. Create user and try again.\n");
        return 0;
    }

    while(current != NULL) {
        user_t* current_data = node_get_user(current);

        /*checks if input username and password matches any data in existing nodes*/
        if(strcmp(current_data->username, username_dummy) == 0 &&
           strcmp(current_data->password, password_dummy) == 0) {
            
            strcpy(logged_in_user->username, current_data->username);
            strcpy(logged_in_user->password, current_data->password);

            printf("User login successful\n");
            return 1;
           }
        current = current->next;
    }

    printf("User login unsuccessful.\n");
    return 0;
}

/**************************************************************
Author/s:
Adrian Rolfo 24818686
Lucy Nelson 25255555

Description:
adds new users to the user struct blah blah fix later

inputs:
- none
outputs:
- none
**************************************************************/
void user_add(list_t* users) {
    DEBUG_PRINT("\nDEBUG: In function user_add\n");
    /*create user struct ON THE STACK to be added*/
    user_t new_user;
    new_user.file_list = NULL;
    

    /*initialized with MAX_SIZE + 1 to catch cases 
    where user and password is too long*/
    char username_dummy[MAX_NAME_SIZE + 1], 
         password_dummy[MAX_NAME_SIZE + 1];

    printf("\nREQUIREMENTS:\n");
    printf("Username must:\n"
           "- contain no white space characters\n"
           "- be longer than 5 characters\n"
           "- must be smaller than %d\n", MAX_NAME_SIZE - 1);
    printf("Password must:\n"
           "- contain at least 1 lower case letter\n"
           "- contain at least 1 upper case letter\n"
           "- contain at least 1 number\n"
           "- be longer than 5 characters\n"
           "- must be smaller than %d\n", MAX_NAME_SIZE - 1);

    printf("\nEnter Username> ");
    read_username_password_input(username_dummy, MAX_NAME_SIZE + 1);
    while(!check_valid_username(username_dummy, MAX_NAME_SIZE, users)) {
        printf("Invalid username. Username> ");
        read_username_password_input(username_dummy, MAX_NAME_SIZE + 1);
    }
    /*if input username is valid, add to new_user struct*/
    strcpy(new_user.username, username_dummy);

    printf("Enter Password> ");
    read_username_password_input(password_dummy, MAX_NAME_SIZE + 1);
    while(!check_valid_password(password_dummy, MAX_NAME_SIZE)) {
        printf("Invalid password. Password> ");
        read_username_password_input(password_dummy, MAX_NAME_SIZE + 1);
    }
    /*if input password is valid, add to new_user struct*/
    strcpy(new_user.password, password_dummy);

    list_push_front(users, &new_user, sizeof(user_t));
    printf("User added successfully.\n");
}


/**************************************************************
Author/s:
Adrian Rolfo 24818686

Description:
checks if the inputted username is valid

inputs:
- str: the string input to be checked 
- max_str_size: the max allowed size of the str
- users: pointer to the user_list_t struct
outputs:
- 1: if username is valid
- 0: if username is not valid
**************************************************************/
int check_valid_username(char *username, int max_str_size, list_t* users) {
    DEBUG_PRINT("\nDEBUG: In function check_valid_username\n");
    size_t str_len = strlen(username);

    /* checks if username is greater than max_str_size */
    if(str_len > (size_t)(max_str_size - 1)) {
        printf("Error: username exceeds max character limit (%d)."
        , MAX_NAME_SIZE);
        return 0;
    }
    
    /* checks if username has more than 5 characters */
    if(str_len < 5) {
        printf("Error: username is shorter than 5 characters\n");
        return 0;
    }
    
    /* checks if username already exists */
    if(list_find_username(users, username)) {
        printf("Error: username already exists\n");
        return 0;
    }

    /* checks if username contains whitespace characters */
    int i = 0;
    while(username[i] != '\0') {
        if(username[i] == ' ' || username[i] == '\t' || username[i] == '\n') {
            printf("Error: username contains white space input\n");
            return 0;
        }
        i++;
    }
    
    return 1;
}

int check_valid_password(char str[], int max_str_size) {
    DEBUG_PRINT("\nDEBUG: In function check_valid_password\n");
    size_t str_len = strlen(str);

    /* checks if password is longer than max_str_size */
    if(str_len > (size_t)(max_str_size - 1)) {
        printf("Error: password exceeds max character limit (%d).\n"
        , MAX_NAME_SIZE);
        return 0;
    }

    /* checks if password has more than 5 characters */
    if(str_len < 5) {
        printf("Error: username is shorter than 5 characters\n");
        return 0;
    }
    
    
    int upper_case_count = 0;
    int lower_case_count = 0;
    int number_count = 0;
    int i = 0;
    while(str[i] != '\0') {
        /* checks for captial letter */
        if(str[i] <= 90 && str[i] >= 65) {
            upper_case_count++;
        }
        /* checks for lowercase letter */
        if(str[i] <= 122 && str[i] >= 97) {
            lower_case_count++;
        }
        /* checks for number */
        if(str[i] <= 57 && str[i] >= 48) {
            number_count++;
        }
        i++;
    }
    if(upper_case_count > 0 && lower_case_count > 0 && number_count > 0) {
        return 1;
    }
    printf("Include at least 1 of each: capital letter, lowercase letter, number\n");
    return 0;
    
}

void print_about() {
    DEBUG_PRINT("\nDEBUG: In function print_about\n");
    printf("\nThis program is a simple user management system with image compression.\n"
           "It allows users to create an account, log in, and compress an image.\n"
           "These files are also automatically encrypted to provide security from digital attacks.\n");

    if(debug_mode == 1) {
        int test = 3;
        char test_string[5] = "test";
        DEBUG_PRINT("\nThis is a debug message test \n");
        DEBUG_PRINT_INT("integer debug mode test %d\n", test);
        DEBUG_PRINT_STRING("string debug: %s\n", test_string);
    }
}
