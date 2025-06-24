#ifndef SORTING_H
#define SORTING_H
#include <stdlib.h> /* size_t */

/*  
Author: Darshan Phuyal 
This function swaps the contents of two memory locations pointed to by a and b, each of size bytes. 
    It's used to interchange the values of two elements during sorting. */
void swap(void* a, void* b, size_t size);

/*  This function is used in the QuickSort algorithm to partition the array into two halves. It rearranges
    the elements so that all elements less than a pivot element are on one side and all elements greater
    than the pivot are on the other. It returns the index of the pivot element after partitioning.  */
int partition(void* arr, int low, int high, size_t size, int (*compare)(const void*, const void*));

/*  This function implements the QuickSort algorithm. It recursively sorts the array by partitioning it and
    then sorting the subarrays. It uses the partition function and a comparison function compare to determine
    the order of elements.   */
void quicksort(void* arr, int low, int high, size_t size, int (*compare)(const void*, const void*));

/*  This is a general-purpose sort function that sorts an array arr of size elements, each of elem_size bytes.
    It uses the QuickSort algorithm internally and relies on the compare function to determine the ordering of 
    the elements.     */
void sort(void* arr, int size, size_t elem_size, int (*compare)(const void*, const void*));

/*  This function compares two file names (or strings) pointed to by a and b. It can be used as a comparison
    function in sorting algorithms to sort files by their names.    */
int compare_file_name(const void* a, const void* b);

/*  This function compares the sizes of two files pointed to by a and b. It can be used as a comparison 
    function in sorting algorithms to sort files by their sizes.    */
int compare_file_size(const void* a, const void* b);

/*  This function converts all the characters in the string str to lowercase. */
void to_lowercase(char* str);

#endif
