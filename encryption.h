#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <stdlib.h> /* size_t */

/*writes a permutation of 4P4 chosen by input permutation_index to input pointer permutation.*/
void get_permutation(int* permutation, int permutation_index);

/*randomises string for the XOR algorithm*/
void randomize_string(char str[], int length);

/*generates a unique key for any string input username, and writes generated key to input key.*/
void generate_key(const char str[], char key[]);

/**/
void XOR_string(char dest[], const char source[], const char key[]);

/*applies XOR algorithm on source input and writes input to dest*/
void XOR_user(char dest[], const char source[]);

/*applies the XOR algorithm to file to encrypt*/
void XOR_file_encrypt(const char* file_path, unsigned char* data, size_t data_size, const char* key);

/*applies the XOR algorithm to file to decrypt*/
unsigned char* XOR_file_decrypt(unsigned char* data, size_t data_size, const char* key);

#endif
