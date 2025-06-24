/***************************************************************
COMPILATION INSTRUCTIONS. VERY IMPORTANT YOU COPY EXACTLY

'cd vFINAL' (gets you into the vFINAL folder)
'make main.out' or just 'make'
then run './main.out' with either '-d' for debugging and '-p' for production
to remove the .o and .out files 'make clean'

old compilation method
gcc -Wall -Werror -ansi -o main.out main.c generic_list.c login_functions.c user_functions.c sorting.c compression.c encryption.c user_input.c file_functions.c debug.c -lm
***************************************************************/

/***************************************************************
48430 Fundamentals of C Programming - Assignment 3
Students:
Adrian Rolfo 24818686
Lucy Nelson 25255555
Darshan Phuyal 24951941   
***************************************************************/

/***************************************************************
List header files
***************************************************************/

#include <stdio.h> /* printf() */
#include <string.h> /* strcmp() */
#include "definitions.h" /* user_t, list_t */
#include "login_functions.h"
#include "user_functions.h"
#include "encryption.h" /**/
#include "user_input.h" /* read_char_input() */
#include "file_functions.h"
#include "generic_list.h"
#include "debug.h"

/*************************************************************
Main
*************************************************************/

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Incorrect runtime flag.\n"
            "-d for debugging mode\n"
            "-p for production mode\n");
        return 0;
    }

    if(strcmp(argv[1], "-d") == 0) {
        debug_mode = 1;
        printf("\nDebugging mode activated\n");
    }

    else if(strcmp(argv[1], "-p") == 0) {
        printf("\nYou are in production mode\n");
    }
    else {
        printf("Incorrect runtime flag.\n"
            "-d for debugging mode\n"
            "-p for production mode\n");
        return 0;
    }

    user_t logged_in_user;
    list_t* users = list_create();
    file_load_users(users);

    char choice;
    int exit = 0;
    print_menu();
    read_char_input(&choice);

    while (choice != '4' && exit == 0) {
        switch (choice) {
            case '1': /* login */
                if(user_login(users, &logged_in_user) == 0) {
                    print_menu();
                    read_char_input(&choice);
                    break;
                }
                else {
                    exit = 1;
                    break;
                }
            case '2': /* add user */
                user_add(users);
                file_upload_users(users);
                print_menu();
                read_char_input(&choice);
                break;

            case '3': /* print about / how to use */
                print_about();
                print_menu();
                read_char_input(&choice);
                break;
            
            case '4': /* seems to be broken... did bandaid fix */
                printf("Exiting...\n");
                file_upload_users(users);
                return 0; 
            
        default:
            if (debug_mode == 1 && choice == '5') { /* debugging purposes */
                printf("\nDisplaying users...\n");
                list_display_users(users);
            } 
            else {
                printf("Invalid choice.\n");
            }
            print_menu();
            read_char_input(&choice);
            break;
        }
    }

    if(choice == '4') {
        printf("Exiting...\n");
        file_upload_users(users);
        return 0; 
    }
    
    list_t* file_list = list_create();
    logged_in_user.file_list = file_list;
    generate_key(logged_in_user.username, logged_in_user.key);

    file_load_user_files(&logged_in_user);

    choice = '0';
    print_login_menu();
    read_char_input(&choice);
    

    while (choice != '4') {
        switch (choice) {
            case '1': /* upload file */
                file_upload_file(&logged_in_user);
                file_upload_user_files(&logged_in_user);
                print_login_menu();
                read_char_input(&choice);
                break;
            case '2': /* display image list */
                files_display_menu(&logged_in_user);
                print_login_menu();
                read_char_input(&choice);
                break;
            case '3': /* deletes user and all data */
                user_remove(&logged_in_user, users);
                print_login_menu();
                read_char_input(&choice);
                break;
            case '4': /* seems to be broken... did bandaid fix */
                printf("Exiting...\n");
                return 0; 
			default:
				printf("Invalid choice.\n");
                print_login_menu();
				read_char_input(&choice);
				break;
        }
    }
    file_upload_user_files(&logged_in_user);
    return 0;
}
