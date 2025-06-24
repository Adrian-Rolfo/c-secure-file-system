#include <stdlib.h> /*malloc(), memcpy(), free()*/
#include <string.h> /*strcpy(), strcmp()*/
#include <stdio.h> /*stderr(), fprintf(), - from debug mode*/
#include "sorting.h"
#include "definitions.h" /* file_t */
#include "debug.h"

/*#define DEBUG*/

/*************************************
Author: Darshan Phuyal 24951941 

Description:

*************************************/
void swap(void* a, void* b, size_t size) {
    DEBUG_PRINT("\nDEBUG: in function swap\n");
    void* temp;
    temp = malloc(size); /* Allocate memory for the temporary buffer */
    if (temp == NULL) {  /*Check if memory allocation was successful */
        DEBUG_PRINT("Error: Memory allocation failed.\n");
    }
    
    memcpy(temp, a, size); /* Copies the value of 'a' to the temporary buffer */
    memcpy(a, b, size); /* Copies the value of 'b' to 'a' */
    memcpy(b, temp, size); /* Copies the value from the temporary buffer to 'b' */
    free(temp); /* Free the dynamically allocated memory */
}


/*************************************
Author: Darshan Phuyal 24951941 
*************************************/
int partition(void* arr, int low, int high, size_t size, int (*compare)(const void*, const void*)) {
    DEBUG_PRINT("\nDEBUG: in function partition\n");
    char* pivot = (char*)arr + high * size; /* Selects the last element as the pivot */
    int i = low - 1;
    int j;
    for (j = low; j < high; j++) { /* Iterates from 'low' to 'high-1' */
        if (compare((char*)arr + j * size, pivot) <= 0) { /* Compares the current element with the pivot */
            i++;
            swap((char*)arr + i * size, (char*)arr + j * size, size); /* Swaps the current element with the element at index 'i' */
        }
    }
    swap((char*)arr + (i + 1) * size, (char*)arr + high * size, size); /* Swaps the pivot element with the element at index 'i+1' */
    return i + 1; /* Returns the partition index */
}

/*************************************
Author: Darshan Phuyal 24951941 
*************************************/
void quicksort(void* arr, int low, int high, size_t size, int (*compare)(const void*, const void*)) {
     DEBUG_PRINT("\nDEBUG: in function quicksort\n");
    int pi;
    if (low < high) { /* Checks if there are at least two elements to sort */
        pi = partition(arr, low, high, size, compare); /* Partitions the array and gets the partition index */
        quicksort(arr, low, pi - 1, size, compare); /* Recursively sorts the left subarray */
        quicksort(arr, pi + 1, high, size, compare); /* Recursively sorts the right subarray */
    }
}

/*************************************
Author: Darshan Phuyal 24951941 
*************************************/
void sort(void* arr, int size, size_t elem_size, int (*compare)(const void*, const void*)) {
     DEBUG_PRINT("\nDEBUG: in function sort\n");
    quicksort(arr, 0, size - 1, elem_size, compare); /* Calls the quicksort function with the appropriate parameters */
}

/*************************************
Author: Adrian Rolfo 24818686
*************************************/
int compare_file_name(const void* a, const void* b) {
    DEBUG_PRINT("\nDEBUG: in function compare_file_name\n");
    const file_t* fileA = (const file_t*)a;
    const file_t* fileB = (const file_t*)b;
    char name_a[MAX_NAME_SIZE], name_b[MAX_NAME_SIZE];
    strcpy(name_a, fileA->name);
    strcpy(name_b, fileB->name);
    to_lowercase(name_a);
    to_lowercase(name_b);
    return strcmp(name_a, name_b); /* Compare the strings in ascending order */
}

/*************************************
Author: Adrian Rolfo 24818686

Description: 

*************************************/
int compare_file_size(const void* a, const void* b) {
    DEBUG_PRINT("\nDEBUG: in function compare_file_size\n");
    const file_t* fileA = (const file_t*)a;
    const file_t* fileB = (const file_t*)b;

    if (fileA->file_size < fileB->file_size) {
        return 1;
    } else if (fileA->file_size > fileB->file_size) {
        return -1;
    } else {
        return 0;
    }
}

/*************************************
Author: Adrian Rolfo 24818686

Description:
expects an input string containing only alphabet input.
if input is uppercase it makes it to lower case
*************************************/

void to_lowercase(char *str) {
    DEBUG_PRINT("\nDEBUG: in function to_lowercase\n");
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str = *str + ('a' - 'A');
        }
        str++;
    }
}
