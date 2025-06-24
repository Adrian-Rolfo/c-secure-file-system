#ifndef USER_FUNCTIONS_H
#define USER_FUNCTIONS_H
#include "definitions.h" /*user_t, list_t, node_t, file_t*/

/* prints menu after logging in */
void print_login_menu();

/* prints menu for options on how to display files */
void print_display_files_menu();

/* prints menu for option on what to do with selected file */
void print_file_options();

/* the menu for displaying files but also link to functions blah whatever*/
void file_upload_file(user_t* logged_in_user);

int check_file_exists(const char* file_path);

/* deletes an file from the file list */
void file_delete_encrypted_file(list_t* file_list, node_t* file);

/* downloads an file out of the system as .bmp file */
void file_download(node_t* file, const char* key);

/* options you can do to a single file node */
void file_options(user_t* logged_in_user, node_t* file);

/* prints file_t data stored in an array*/
void print_file_array(file_t files[], int size);

/* display user file data sorted by date */
void files_display_by_date(user_t* logged_in_user);

/* displays user files by (uncompressed) file size */
void files_display_by_size(user_t* logged_in_user);

/* displays user files by name alphabetically */
void files_display_by_name(user_t* logged_in_user);

/* the menu for displaying files but also link to functions that blah whatever*/
void files_display_menu(user_t* logged_in_user);

/* permanently deletes the users account and all related files */
void user_remove(user_t* logged_in_user, list_t* users);

#endif
