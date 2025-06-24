#include <stdio.h> /*printf(), */
#include <stdlib.h> /* fopen(), fclose(), fseek(), ftell(), rewind(), fread(), fwrite, remove() */
#include <string.h> 
#include "definitions.h" /* node_t, list_t, user_t, file_t */
#include "file_functions.h" /*void file_load_users(), void file_upload_users(), void file_load_user_files(), void file_upload_user_files(), void file_upload_protected_file()*/
#include "generic_list.h"
#include "compression.h"
#include "encryption.h"
#include "debug.h"


void file_load_users(list_t* users) {
    DEBUG_PRINT("\nDEBUG: In function file_load_users()\n");

    FILE *fp = fopen("user_data", "rb");
    if(fp == NULL) {
        printf("No user data exists\n");
        return;
    }
    
    long file_size;

    /* gets size of file and resets file pointer to start of file*/
    fseek(fp, 0 , SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    /* gets number of saved users in memory */
    size_t num_user_t = file_size/(2*MAX_NAME_SIZE);

    user_t load_user;
    load_user.file_list = NULL;
    
    int i;
    for(i = 0; i < (int)num_user_t; i++) {
        fread(load_user.username, MAX_NAME_SIZE, 1, fp);
        fread(load_user.password, MAX_NAME_SIZE, 1, fp);
        list_push_back(users, &load_user, sizeof(user_t));
    }
    
    fclose(fp);
    return;
}

/*************
save user data in binary format

things to consider:
- what happens if a user/photos are deleted

user struct data
- username 
- password
- image list pointer

- username and password data essential to save
- image list pointer should not be saved as image list should 
be reconstructed in load method



**************/
void file_upload_users(list_t* users) {
    DEBUG_PRINT("\nDEBUG: In function file_upload_users\n");

    if(users->size == 0) {
        printf("Users list is empty, deleting user data\n");
        remove("user_data");
        return;
    }

    /* This block erases all data in file */
    FILE *fp = fopen("user_data", "wb");
    if(fp == NULL) {
        printf("Error: failure opening file in file_upload_users.\n");
        return;
    }
    fclose(fp);

    /*
    the actual file data saving stuff
    */
    fp = fopen("user_data", "wb");
    if(fp == NULL) {
        printf("Error: failure opening file in file_upload_users.\n");
        return;
    }
    
    node_t* current = users->head;
    int count = 1;
    while(current != NULL) {
        DEBUG_PRINT_INT("\nDEBUG: writing node data to file: %d\n", count++);
        user_t* curr_user = node_get_user(current);
        /*should write only the username and password*/
        fwrite(curr_user, MAX_NAME_SIZE, 2,fp);
        current = current->next;
    }

    fclose(fp);
    return;
}

void file_load_user_files(user_t* user) {
    DEBUG_PRINT("\nDEBUG: In function file_load_user_files\n");

    list_t* files = user->file_list;
    char file_name[MAX_NAME_SIZE];
    XOR_string(file_name, user->username, user->key);

    /* creating file_path string*/
    char directory[] = "file_data/";
    char file_path[sizeof(directory) + MAX_NAME_SIZE];
    strcpy(file_path, directory);
    strcat(file_path, file_name);

    FILE *fp = fopen(file_path, "rb");
    if(fp == NULL) {
        printf("User has no file data\n");
        return;
    }
    
    long file_size;

    /* gets size of file and resets file pointer to start of file*/
    fseek(fp, 0 , SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    size_t size_saved_file_t = 2*MAX_NAME_SIZE + sizeof(unsigned long);
    size_t num_file_t = file_size/size_saved_file_t;

    file_t load_file;
    
    int i;
    for(i = 0; i < (int)num_file_t; i++) {
        DEBUG_PRINT_INT("\nDEBUG: reading file data to list: %d\n", i + 1);

        fread(load_file.name, MAX_NAME_SIZE, 1, fp); /* Read name */
        fread(load_file.type, MAX_NAME_SIZE, 1, fp); /* Read type */
        fread(&load_file.file_size, sizeof(unsigned long), 1, fp); /* Read file_size */

        /* constructing encrypted name and adding to list */
        XOR_string(load_file.encrypted_name, load_file.name, user->key);
        list_push_back(files, &load_file, sizeof(file_t));
    }
    
    fclose(fp);
    return;
}

void file_upload_user_files(user_t* user) {
    DEBUG_PRINT("\nDEBUG: In function file_upload_user_files\n");

    list_t* files = user->file_list;

    char file_name[MAX_NAME_SIZE];
    XOR_string(file_name, user->username, user->key);

    /* creating file path directory */
    char directory[] = "file_data/";
    char file_path[sizeof(directory) + MAX_NAME_SIZE];
    strcpy(file_path, directory);
    strcat(file_path, file_name);

    if(files->size == 0) {
        printf("Files list is empty is empty, deleting file.\n");
        remove(file_path);
        return;
    }

    FILE *fp = fopen(file_path, "wb");
    if(fp == NULL) {
        printf("Error: failure opening file in file_upload_user_data.\n");
        return;
    }
    fclose(fp);

    fp = fopen(file_path, "wb");
    
    if(fp == NULL) {
        printf("Error: failure opening file in file_upload_user_data.\n");
        return;
    }

    node_t* current = files->head;

    int count = 1;
    while(current != NULL) {
        DEBUG_PRINT_INT("DEBUG: writing node data to file: %d\n", count++);
        file_t* curr_file = node_get_file(current);

        fwrite(curr_file->name, MAX_NAME_SIZE, 1, fp); /* Write name */
        fwrite(curr_file->type, MAX_NAME_SIZE, 1, fp); /* Write type */
        fwrite(&curr_file->file_size, sizeof(unsigned long), 1, fp); /* Write file_size */

        current = current->next;
    }

    fclose(fp);
    return;
}

void file_upload_protected_file(const char* dest_path, const char* source_path, const char* key) {
    DEBUG_PRINT("\nDEBUG: In function file_upload_protected_file\n");

    FILE* fsource;
    FILE* fdest;
    fsource = fopen(source_path, "rb"); 
    fdest = fopen(dest_path, "wb");
    if(fsource == NULL) {
        printf("Error: failure opening %s file in file_upload_protected_file.\n", source_path);
        return;
    }
    if(fdest == NULL) {
        printf("Error: failure opening %s file in file_upload_protected_file.\n", dest_path);
        return;
    }
    
    /* calculate file size */
    size_t file_size;
    fseek(fsource, 0 , SEEK_END);
    file_size = ftell(fsource);
    rewind(fsource);

    /* read source data into file */
    unsigned char data[file_size];
    fread(data, file_size, 1, fsource);

    fclose(fsource);
    fclose(fdest);
    
    /* compress, encrypt, and upload data to file */
    size_t compressed_size;
    unsigned char* compressed_data = file_RLE_compress(data, file_size, &compressed_size);
    XOR_file_encrypt(dest_path, compressed_data, compressed_size, key);

    free(compressed_data);
}
