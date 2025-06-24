#include <stdio.h> /*printf(), fopen(), fseek(), rewind(), fclose()*/
#include <stdlib.h>
#include <string.h> /*strcpy(), strcat()*/
#include "definitions.h"
#include "user_functions.h"
#include "generic_list.h"
#include "sorting.h"
#include "encryption.h"
#include "compression.h"
#include "user_input.h"
#include "file_functions.h"
#include "debug.h"

/*#define DEBUG*/

/*
Author: Adrian Rolfo 24818686

Description: returns 1 if file already exists at given file_path
returns 0 if file at given file path doesn't exist;
*/
int check_file_exists(const char* file_path) {
    DEBUG_PRINT("\nDEBUG: in function check_file_exists\n");
    int exist_status = 0;
    FILE* fp = fopen(file_path, "rb");
    if(fp != NULL) {
        exist_status = 1;
    fclose(fp);
    }
    return exist_status;
}

void file_upload_file(user_t* logged_in_user) {
    DEBUG_PRINT("\nDEBUG: in function file_upload_file");
    printf("\nEnsure that your file is in file_upload directory.\n");

    char directory[] = "file_upload/";
    char file_name[MAX_NAME_SIZE];

    printf("Enter name of your file (MAX NAME SIZE = %d)\n", MAX_NAME_SIZE - 1);
    printf("Ensure your input is the form of <file_name>.<file_type>\n");
    printf("File name> ");
    read_string_input(file_name, MAX_NAME_SIZE);

    /* creates file_path which combines directory + name of file*/
    char file_path[MAX_NAME_SIZE + sizeof(directory)];
    strcpy(file_path, directory);
    strcat(file_path, file_name);

    /* checks if file actually exists */
    FILE *fp = fopen(file_path, "rb");
    if(fp == NULL) {
        printf("Can't find file! File path: %s\n", file_path);
        return;
    }

    printf("File open success! File path: %s\n", file_path);

    /* gets size of file*/
    long file_size;
    fseek(fp, 0 , SEEK_END);
    file_size = ftell(fp);

    fclose(fp);

    /* creates a file_t struct and fills in fields */
    file_t new_file;
    char* token = strtok(file_name, ".");
    strcpy(new_file.name, token);
    token = strtok(NULL, " .,");
    strcpy(new_file.type, token);
    new_file.file_size = file_size;
    XOR_string(new_file.encrypted_name, new_file.name, logged_in_user->key);

    /* creates string for dest path for file_upload_protected_file function */
    char dest_directory[] = "encrypted_files/";
    char dest_path[sizeof(dest_directory) + strlen(new_file.encrypted_name)];
    strcpy(dest_path, dest_directory);
    strcat(dest_path, new_file.encrypted_name);

    /* check if file has already been uploaded */;
    if(check_file_exists(dest_path) == 1) {
        printf("Error: File already uploaded.\n");
        return;
    }

    /* adds new file_t to file_list in logged_in_user*/
    file_upload_protected_file(dest_path, file_path, logged_in_user->key);
    list_push_front(logged_in_user->file_list, &new_file, sizeof(file_t));

    printf("File successfully downloaded, free to delete file at %s\n", file_path);

    DEBUG_PRINT("\nDEBUG: checking struct\n");
    DEBUG_PRINT_STRING("File Name: %s\n", new_file.name);
    DEBUG_PRINT_STRING("File Type: %s\n", new_file.type);
    DEBUG_PRINT_DOUBLE("File Size: %ld\n", new_file.file_size);
    DEBUG_PRINT_STRING("Encrypted Name: %s\n", new_file.encrypted_name);

    return;
}

void print_login_menu() {
    DEBUG_PRINT("\nDEBUG: in function print_login_menu\n");
    printf("\nUser menu\n"
           "1. Upload file\n" 
           "2. Display file list\n"
           "3. Delete user\n"
           "4. Exit program\n"
           "Enter choice> ");
}

void print_display_files_menu() {
    DEBUG_PRINT("\nDEBUG: in function print_display_files_menu\n");
    printf("\nDisplay files menu\n"
           "1. Display by last added\n"
           "2. Display by file size\n" 
           "3. Display alphabetically\n" 
           "4. Back\n"
           "Enter choice> ");
}

void print_file_options() {
    DEBUG_PRINT("\nDEBUG: in function print_file_options\n");
    printf("\nWhat would you like to do with the file?\n"
           "1. Delete file\n"
           "2. Download file\n"
           "3. Back\n"
           "Enter choice> ");
}

/* deletes a file from the file list */
void file_delete_encrypted_file(list_t* file_list, node_t* file) {
    DEBUG_PRINT("\nDEBUG: in function file_delete_encrypted_file\n");
    file_t* curr_file = node_get_file(file);
    char directory[] = "encrypted_files/";
    char file_path[sizeof(directory) + MAX_NAME_SIZE];
    strcpy(file_path, directory);
    strcat(file_path, curr_file->encrypted_name);

    remove(file_path);
    node_delete(file_list, file);  
    return;
}

/* downloads an file out of the system as .bmp file */
void file_download(node_t* file, const char* key) {
    DEBUG_PRINT("\nDEBUG: in function file_download\n");
    printf("IN FILE DOWNLOAD\n");
    char source_directory[] = "encrypted_files/";
    char source_path[sizeof(source_directory) + MAX_NAME_SIZE];
    char dest_directory[] = "file_out/";
    char dest_path[sizeof(dest_directory) + MAX_NAME_SIZE];

    file_t* curr_file = node_get_file(file);
    printf("ENCRYPTED FILE NAME: %s\n", curr_file->encrypted_name);
    printf("FILE SIZE: %d\n", (int)curr_file->file_size);
    /*creates file_name from curr_file fields*/
    char file_name[MAX_NAME_SIZE];
    strcpy(file_name, curr_file->name);
    strcat(file_name, ".");
    strcat(file_name, curr_file->type);
    /*creates destination path*/
    strcpy(dest_path, dest_directory);
    strcat(dest_path, file_name);
    /*creates source path (data in)*/
    strcpy(source_path, source_directory);
    strcat(source_path, curr_file->encrypted_name);

    FILE* fsource;
    FILE* fdest;
    fsource = fopen(source_path, "rb"); 
    fdest = fopen(dest_path, "wb");
    if(fsource == NULL) {
        printf("Error: failure opening %s file in file_download.\n", source_path);
        return;
    }
    if(fdest == NULL) {
        printf("Error: failure opening %s file in file_download.\n", dest_path);
        return;
    }

    size_t file_size;
    fseek(fsource, 0 , SEEK_END);
    file_size = ftell(fsource);
    rewind(fsource);

    /*unsigned char* data = (unsigned char*)malloc(file_size);*/
    unsigned char data[file_size];
    fread(data, file_size, 1, fsource);

    fclose(fsource);
    fclose(fdest);

    size_t decompressed_size = curr_file->file_size;
    unsigned char* decrypted_data = XOR_file_decrypt(data, file_size, key);
    file_RLE_decompress(dest_path, decrypted_data, file_size, decompressed_size);

    free(decrypted_data);

    printf("\nDownload success. File available at %s\n", dest_path);
}

void file_options(user_t* logged_in_user, node_t* file) {
    DEBUG_PRINT("\nDEBUG: in function file_options\n");
    DEBUG_PRINT("ENTERED FILE_OPTIONS\n");
    if(file == NULL) {
        DEBUG_PRINT("FILE IS NULL\n");
        return;
    }
    file_t* curr_file = node_get_file(file);
    char file_name[MAX_NAME_SIZE];
    strcpy(file_name, curr_file->name);
    strcat(file_name, ".");
    strcat(file_name, curr_file->type);
    printf("\nFile chosen: %s", file_name);

    char choice, choice2;
    print_file_options();
    read_char_input(&choice);

    int exit = 0;
    while (choice != 3 && exit == 0) {
    switch (choice) {
        /*if scanf reads non int input 0 is returned*/
        case '0': /* bad input */
            printf("Invalid choice.\n");
            print_file_options();
            read_char_input(&choice);
            break;
        case '1': /* Delete file */
            printf("\nWARNING: deleting file is irreversible\n");
            printf("Do you wish to delete %s (Y/N)> ", file_name);

            read_char_input(&choice2);
            while(choice2 != 'Y' && choice2 != 'N') {
                printf("Invalid choice.\n");
                printf("Do you wish to delete %s (Y/N)> ", file_name);
                read_char_input(&choice2);
            }
            if(choice2 == 'Y') {
                file_delete_encrypted_file(logged_in_user->file_list, file);
                DEBUG_PRINT("SUCCEFFULLY DELETED ENCRYPTED FILE\n");
                file_upload_user_files(logged_in_user);
                DEBUG_PRINT("SUCCESSFULLY UPLOADED USER FILES\n");
                DEBUG_PRINT_STRING("%s deleted\n", file_name);
                exit = 20;
            }
            else {
                print_file_options();
                read_char_input(&choice);
            }
            break;
        case '2': /* Download file */
            file_download(file, logged_in_user->key);
            return;
        case '3': /* back seems to be broken... did bandaid fix */
            return; 
        default:
            printf("Invalid choice.\n");
            print_file_options();
            read_char_input(&choice);
            break;
        }
    }

}

void files_display_by_date(user_t* logged_in_user) {
    DEBUG_PRINT("\nDEBUG: in function files_display_by_date\n");
    list_t* file_list = logged_in_user->file_list;
    printf("\nDisplay by Last Added\n");

    if(file_list->size == 0) {
        printf("No files to display.\n");
        return;
    }
    
    list_display_file_data(file_list);
    
    int choice = 0;
    printf("Select file (Enter 0 to exit)> ");
    scanf("%d", &choice);

    while(choice < 0 || choice > file_list->size) {
        printf("File not in list, try again> ");
        scanf("%d", &choice);
    }
    if(choice == 0) return;

    file_options(logged_in_user, node_get(file_list, choice));
}

void print_file_array(file_t files[], int size) {
    DEBUG_PRINT("\nDEBUG: in function print_file_array\n");
    int i;
    for(i = 0; i < size; i++) {
        char name[MAX_NAME_SIZE];
        strcpy(name, files[i].name);
        strcat(name, ".");
        strcat(name, files[i].type);

        printf("%d. File Name: %s\n"
        , i + 1, name);
    }
}

void files_display_by_size(user_t* logged_in_user) {
    DEBUG_PRINT("\nDEBUG: in function files_display_by_size\n");
    list_t* file_list = logged_in_user->file_list;
    printf("\nDisplay by File Size (largest to smallest)\n");

    if(file_list->size == 0) {
        printf("No files to display.\n");
        return;
    }

    /*** creates an array of with file_list size ***/
    file_t copied_list[file_list->size];
    
    /* fills in each position of array with filt_t structs */
    node_t* current = file_list->head;
    int count = 0;
    while(current != NULL) {
        copied_list[count] = *node_get_file(current);
        count++;
        current = current->next;
    }

    sort(copied_list, file_list->size, sizeof(file_t), compare_file_size);

    print_file_array(copied_list, file_list->size);

    int choice = 0;
    printf("Select file (Enter 0 to exit)> ");
    scanf("%d", &choice);

    while(choice < 0 || choice > file_list->size) {
        printf("File not in list, try again> ");
        scanf("%d", &choice);
    }
    if(choice == 0) return;

    file_options(logged_in_user, node_match_file_name(file_list, copied_list[choice - 1].name));
}

void files_display_by_name(user_t* logged_in_user) {
    DEBUG_PRINT("\nDEBUG: in function files_display_by_name\n");
    list_t* file_list = logged_in_user->file_list;
    printf("\nDisplay by File Name (alphabetically)\n");

    if(file_list->size == 0) {
        printf("No files to display.\n");
        return;
    }

    /*** creates an array of with file_list size ***/
    file_t copied_list[file_list->size];
    
    /* fills in each position of array with filt_t structs */
    node_t* current = file_list->head;
    int count = 0;
    while(current != NULL) {
        copied_list[count] = *node_get_file(current);
        count++;
        current = current->next;
    }

    sort(copied_list, file_list->size, sizeof(file_t), compare_file_name);

    print_file_array(copied_list, file_list->size);

    /****
    MAY HAVE TO CHANGE THIS IMPLEMENTATION
    ****/

    int choice = 0;
    printf("Select file (Enter 0 to exit)> ");
    scanf("%d", &choice);

    while(choice < 0 || choice > file_list->size) {
        printf("File not in list, try again> ");
        scanf("%d", &choice);
    }
    if(choice == 0) return;

    file_options(logged_in_user, node_match_file_name(file_list, copied_list[choice - 1].name));
}

/* the menu for displaying files but also link to functions that blah whatever*/
void files_display_menu(user_t* logged_in_user) {
    DEBUG_PRINT("\nDEBUG: in function files_display_menu\n");
        int choice;
        print_display_files_menu();
        scanf("%d", &choice);

        while (choice != 4) {
        switch (choice) {
			/*if scanf reads non int input 0 is returned*/
			case 0: /* bad input */
				clear_input_buffer();
				printf("Invalid choice.\n");
				print_display_files_menu();
				scanf("%d", &choice);
				break;
            case 1: /* Display by last added PROBABLY WILL NOT IMPLEMENT */
                files_display_by_date(logged_in_user);
                print_display_files_menu();
                scanf("%d", &choice);
                break;
            case 2: /* display by file size */
                files_display_by_size(logged_in_user);
                print_display_files_menu();
                scanf("%d", &choice);
                break;
            case 3: /* display alphabetically */
                files_display_by_name(logged_in_user);
                print_display_files_menu();
                scanf("%d", &choice);
                break;
            case 4: /* seems to be broken... did bandaid fix */
                return; 
			default:
                clear_input_buffer();
				printf("Invalid choice.\n");
                print_display_files_menu();
				scanf("%d", &choice);
				break;
        }
    }
}

/* deletes the users account */
void user_remove(user_t* logged_in_user, list_t* users) {
    DEBUG_PRINT("\nDEBUG: in function user_remove\n");
    char choice;

    printf("\nWARNING: deleting user removes all user data\n");
    printf("This includes all stored files as well as deletion of account\n");
    printf("\nDo you still wish to proceed (Y/N)> ");

    read_char_input(&choice);
    while(choice != 'Y' && choice != 'N') {
        printf("Invalid choice.\n");
        printf("Do you still wish to proceed (Y/N)> ");
        read_char_input(&choice);
    }
    if(choice == 'Y') {
        printf("Deleting all user data...\n");
    }
    else return;

    list_t* file_list = logged_in_user->file_list;

    while(file_list->size != 0) {
        file_delete_encrypted_file(file_list, file_list->head);
    }

    list_delete(file_list);
    file_list = logged_in_user->file_list = NULL;

    char file_name[MAX_NAME_SIZE];
    XOR_string(file_name, logged_in_user->username, logged_in_user->key);

    char directory[] = "file_data/";
    char file_path[sizeof(directory) + MAX_NAME_SIZE];
    strcpy(file_path, directory);
    strcat(file_path, file_name);
    remove(file_path);
    
    node_t* current = users->head;
    while(current != NULL) {
        user_t* curr_user = node_get_user(current);
        if(strcmp(curr_user->username, logged_in_user->username) == 0) break;
        current = current->next;
    }
    node_delete(users, current);
    file_upload_users(users);
    printf("All user data deleted. Exiting program.\n");
    exit(EXIT_SUCCESS);
}
