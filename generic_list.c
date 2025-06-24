#include <stdlib.h>  /* malloc(), free() */
#include <string.h> /* strcmp(), strcat(), strcpy() */
#include <stdio.h> /* printf() */
#include "definitions.h"
#include "debug.h"
#include "generic_list.h" /* list_t* list_create(), void list_push_back(), 
void list_push_front(), void* node_getdata(), file_t* node_get_file(), 
user_t* node_get_user(), void node_delete(), void list_delete(), 
node_t* node_get(), node_t* node_match_file_name(), int list_find_username(), 
void list_display_users(), void list_display_file_data() */

/*#define DEBUG*/


list_t* list_create() {
    DEBUG_PRINT("\nDEBUG: In function list_create\n");
    DEBUG_PRINT("\nDEBUG: allocating memory\n");

    list_t* list = (list_t*)malloc(sizeof(list_t));
    list->size = 0;
    list->head = list->tail = NULL;
    return list;
}

void list_push_back(list_t* list, void* data, size_t data_size) {
    DEBUG_PRINT("\nDEBUG: In function list_push_back\n");
    DEBUG_PRINT("\nDEBUG: allocating memory\n");

    node_t* new_node = (node_t*)malloc(sizeof(node_t));

    void* datap = new_node->data = malloc(data_size);

    /*copies input stack data into the heap*/
    int i;
    for(i = 0; i < (int)data_size; i++) {
        /*cast to char* to allow copying data byte by byte*/
        *(char*)(datap + i) = *(char*)(data + i);
    }

    if(list->size == 0) {
        list->head = list->tail = new_node;
    }
    list->tail->next = new_node;
    list->tail = new_node;
    new_node->next = NULL;

    list->size++;
}

/**
ASSUMED
- data is a pointer to STACK memory
**/
void list_push_front(list_t* list, void* data, size_t data_size) {
    DEBUG_PRINT("\nDEBUG: In function list_push_front\n");
    DEBUG_PRINT("\nDEBUG: allocating memory\n");

    node_t* new_node = (node_t*)malloc(sizeof(node_t));

    void* datap = new_node->data = malloc(data_size);

    /*copies input stack data into the heap*/
    int i;
    for(i = 0; i < (int)data_size; i++) {
        /*cast to char* to allow copying data byte by byte*/
        *(char*)(datap + i) = *(char*)(data + i);
    }

    /*****
    this works for any size list
    *****/
    if(list->size == 0) {
        list->tail = new_node;
    }
    new_node->next = list->head;
    list->head = new_node;   

    list->size++;
}

file_t* node_get_file(node_t* node) {
    DEBUG_PRINT("\nDEBUG: In function node_get_file\n");

    return (file_t*)(node->data);
}

user_t* node_get_user(node_t* node) {
    DEBUG_PRINT("\nDEBUG: In function node_get_user\n");

    return (user_t*)(node->data);
}

/*

*/

void node_delete(list_t* list, node_t* node) {   
    DEBUG_PRINT("\nDEBUG: In function node_delete\n");

    node_t* temp = list->head;
    node_t* prev_node = NULL;
    
    /* case where node = head*/
    if (temp != NULL && temp == node) { 
        list->head = temp->next; 
        if(list->head == NULL) list->tail = NULL;
        DEBUG_PRINT("\nDEBUG: freeing memory\n");
        free(temp->data);
        free(temp);
        list->size--;
        return; 
    } 
    
    /* loops through list searching for node match*/
    while (temp != NULL && temp != node) { 
        prev_node = temp; 
        temp = temp->next; 
    } 

    /* not in list */
    if (temp == NULL) return; 

    /* temp is at the tail */
    if(temp->next == NULL) {
        list->tail = prev_node;
        prev_node->next = NULL;
        DEBUG_PRINT("\nDEBUG: freeing memory\n")
        free(temp->data);
        free(temp);
        list->size--;
        return;
    }
  
    prev_node->next = temp->next; 
    DEBUG_PRINT("\nDEBUG: freeing memory\n")
    free(temp->data);
    free(temp);
    list->size--;
} 

void list_delete(list_t* list) {
    DEBUG_PRINT("\nDEBUG: list_delete\n");
    if(list == NULL) {
        printf("Error: list is NULL in list_delete.\n");
        return;
    }
    
    node_t* current = list->head;
    int count = 1;
    while(current != NULL) {
        DEBUG_PRINT_INT("\nDEBUG: deleting node: %d\n", count++);
        node_t* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    DEBUG_PRINT("\nDEBUG: freeing list memory\n");
    free(list);
}

node_t* node_get(list_t* list, int index) {
    DEBUG_PRINT("\nDEBUG: in function node_get\n");
    if(list->size == 0) {
        printf("Error: List empty in node_get\n");
        return NULL;
    }

    node_t* current = list->head;

    int i; 
    for(i = 1; i < index; i++) current = current->next;

    return current;
}

node_t* node_match_file_name(list_t* list, char* name) {
    DEBUG_PRINT("\nDEBUG: in function node_match_file_name\n");
    if(list->size == 0) {
        printf("Error: list empty in node_match_file_name\n");
        return NULL;
    }

    node_t* current = list->head;
    while(current != NULL) {
        file_t* curr_file = node_get_file(current);
        if(strcmp(curr_file->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    printf("Error: no name match in node_match_file_name\n");
    return NULL;
}


/****
return 0 if username not found in list
return 1 if username found in list
****/
int list_find_username(list_t* users, char *input) {
    DEBUG_PRINT("\nDEBUG: in function list_find_username\n");
    
    if(users->size == 0) return 0;

    node_t* current = users->head;

    while(current != NULL) {
        /*gets username in current node */
        char *node_username = node_get_user(current)->username;
        
        /* returns true if input and node username match*/
        if(strcmp(node_username, input) == 0) return 1;

        /* moves current node to next node*/
        current = current->next;
    }

    return 0;

}

void list_display_users(list_t* list) {
   DEBUG_PRINT("\nDEBUG: in function list_display_users\n"); 
    node_t* current = list->head;

    int count = 1;
    while(current != NULL) {
        user_t* curr_user = node_get_user(current);
        printf("%d. \nUsername: %s\nPassword: %s\n"
        , count++, curr_user->username, curr_user->password);

        current = current->next;
    }
}

void list_display_file_data(list_t* list) {
    DEBUG_PRINT("\nDEBUG: in function list_display_file_data\n");
    node_t* current = list->head;

    int count = 1;
    while(current != NULL) {
        file_t* curr_file = node_get_file(current);
        char file_name[MAX_NAME_SIZE];
        strcpy(file_name, curr_file->name);
        strcat(file_name, ".");
        strcat(file_name, curr_file->type);
        printf("%d.  File Name: %s\n"
        , count++, file_name);

        current = current->next;
    }
}



