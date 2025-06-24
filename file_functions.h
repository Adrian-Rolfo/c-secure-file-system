#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H
#include "definitions.h" /*list_t, user_t*/

/*adds users to users list from file*/
void file_load_users(list_t* users);

/*replaces user data file with existing users (word better)*/
void file_upload_users(list_t* users);

/* fills relevant data structure with information about users files read from a stored file */
void file_load_user_files(user_t* user);

/* uploads user file information to system */
void file_upload_user_files(user_t* user);

/* uploads encrypted and compressed file to from source path to destination path */
void file_upload_protected_file(const char* dest_path, const char* source_path, const char* key);

#endif
