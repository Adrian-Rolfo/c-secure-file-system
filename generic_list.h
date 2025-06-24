#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdlib.h> /* for size_t */
#include "definitions.h" /* list_t, node_t*/

/*creates a new list and returns a pointer to the list*/
list_t* list_create();

/*adds a node to the back of a list*/
void list_push_back(list_t* list, void* data, size_t data_size);

/*adds a node to the front of a list*/
void list_push_front(list_t* list, void* data, size_t data_size);

/* returns pointer to file struct stored in node */
file_t* node_get_file(node_t* node);

/* returns pointer to user struct stored in node*/
user_t* node_get_user(node_t* node);

/* deletes specified node in given list */
void node_delete(list_t* list, node_t* node);

/* frees all dynamically allocated memory related to given list */
void list_delete(list_t* list);

/* gets node at give index*/
node_t* node_get(list_t* list, int index);

/* returns node of file if file with given name already exists in list */
node_t* node_match_file_name(list_t* list, char* name);

/* returns success if username input exists in given list */
int list_find_username(list_t* users, char *input);

/* debugging purposes. Displays formatted usernames and passwords*/
void list_display_users(list_t* list);

/* displays formatted names of files in given list*/
void list_display_file_data(list_t* list);

#endif
