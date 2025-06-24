#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MAX_NAME_SIZE 25

typedef struct node {
    void* data;
    struct node* next;
} node_t;


typedef struct list {
    int size;
    node_t* head;
    node_t* tail;
} list_t;

typedef struct user {
    char username[MAX_NAME_SIZE];
    char password[MAX_NAME_SIZE];
    list_t* file_list; /* not saved to file*/
    char key[MAX_NAME_SIZE]; /* not saved to file */
} user_t;

typedef struct file_data {
    char name[MAX_NAME_SIZE];
    char type[MAX_NAME_SIZE];
    unsigned long file_size;
    char encrypted_name[MAX_NAME_SIZE]; /* not saved to file */
} file_t;

#endif
