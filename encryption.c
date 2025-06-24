#include <string.h> /* strcpy(), strlen(), */
#include <stdlib.h> 
#include <stdio.h>
#include "definitions.h"
#include "encryption.h" 
#include "debug.h"
/*void generate_key(), void XOR_string(), void XOR_user()*/


/*************************************************
Author: Adrian Rolfo 24818686

Description:
writes a permutation of 4P4 chosen by input permutation_index
to input pointer permutation.

input:
- permuation: 1D int array for chosen permutation to be written too.
- permuation_index: input int that chooses permutation

output: 
- void
*************************************************/
void get_permutation(int* permutation, int permutation_index) {
    DEBUG_PRINT("\nDEBUG: In function get_permutation\n");
    const int permutations[24][4] = {
        {2, 1, 3, 0}, {0, 2, 3, 1}, {1, 3, 0, 2}, {3, 1, 2, 0},
        {0, 3, 1, 2}, {1, 0, 2, 3}, {2, 0, 1, 3}, {3, 2, 0, 1},
        {1, 2, 3, 0}, {2, 3, 1, 0}, {0, 1, 2, 3}, {3, 0, 2, 1},
        {0, 3, 2, 1}, {1, 0, 3, 2}, {2, 0, 3, 1}, {3, 1, 0, 2},
        {0, 2, 1, 3}, {1, 3, 2, 0}, {2, 3, 0, 1}, {3, 2, 1, 0},
        {0, 1, 3, 2}, {1, 2, 0, 3}, {2, 1, 0, 3}, {3, 0, 1, 2}
    };

    memcpy(permutation, permutations[permutation_index], 4*sizeof(int));
}

/*************************************************
Author: Adrian Rolfo 24818686

Description:
generates a unique key for any string input username, and writes 
generated key to input key. Username input must be null terminated
and must not exceed MAX_NAME_SIZE (including null terminator)

input:
- username: input for generated key to be created from
- key: char array for input to be written too

output: 
- void

*************************************************/
void generate_key(const char username[], char key[]) {
    DEBUG_PRINT("\nDEBUG: In function generate_key\n");
    char random_arr[][7] = {"k`t'=_", "|bdUbK", "E^3(-}", ";.<@wD"};
    char permuted_random_arr[4][7];
    char random[MAX_NAME_SIZE];
    int permutation[4];
    get_permutation(permutation, strlen(username));

    int i;
    for(i = 0; i < 4; i++) {
        strcpy(permuted_random_arr[i], random_arr[permutation[i]]);
    }

    strcpy(random, permuted_random_arr[0]);
    for(i = 1; i < 4; i++) strcat(random, permuted_random_arr[i]);

    char username_copy[MAX_NAME_SIZE];
    strcpy(username_copy, username);
    XOR_user(username_copy, username);

    int len = strlen(username_copy);

    for (i = 0; i < MAX_NAME_SIZE; i++) {
        key[i] = (username_copy[i%len] ^ random[i])%95 + 32;
    }
    key[MAX_NAME_SIZE - 1] = '\0';
}

/*************************************************
Author: Adrian Rolfo 24818686

Description:
applies XOR algorithm on source input and writes input to dest
This just provides an extra layer of security for encryption key

input:
- source: string to be XOR
- dest: where XOR input is being written too

output: 
- void

*************************************************/
void XOR_user(char dest[], const char source[]) {
    DEBUG_PRINT("\nDEBUG: In function XOR_user\n");

    char random[] = "D9#-<{y|d]A'd/SD.!9~iH#k";
    int len1 = strlen(source);

    int i;
    for (i = 0; i < MAX_NAME_SIZE; i++) {
        dest[i] = (source[i%len1] ^ random[i])%95 + 32;
    }
    dest[MAX_NAME_SIZE - 1] = '\0';
}

/**/
void XOR_string(char dest[], const char source[], const char key[]) {
    DEBUG_PRINT("\nDEBUG: In function XOR_string\n");

    char random[] = "(4S`};:/B,y7sFS*9$k2B$3@";
    int len1 = strlen(source);

    int i;
    for (i = 0; i < MAX_NAME_SIZE; i++) {
        dest[i] = (source[i%len1] ^ key[i] ^ random[i])%95 + 32;
        if(dest[i] == '/') dest[i]++;
    }
    dest[MAX_NAME_SIZE - 1] = '\0';
}

/*************************************************
Author: Adrian Rolfo 24818686

Description:
Applies the XOR algorithm to file

input:
- file path
- data
- key: char array 

output: 
- void

*************************************************/
void XOR_file_encrypt(const char* file_path, unsigned char* data, size_t data_size, const char* key) {
    DEBUG_PRINT("\nDEBUG: In function XOR_file_encrypt\n");

    unsigned char encrypted_data[data_size];
    memcpy(encrypted_data, data, data_size);
    
    int i;
    for (i = 0; i < (int)data_size; i++) {
        encrypted_data[i] = (encrypted_data[i] ^ key[i%24]);
    }

    FILE* fdest = fopen(file_path, "wb");
    if (fdest == NULL) {
        printf("Error: failure opening file in file_RLE_compress %s\n", file_path);
        return;
    }
    fwrite(encrypted_data, data_size, 1, fdest);
    fclose(fdest);

    DEBUG_PRINT_INT("DEBUG: data_size in XOR_file_encrypt: %d\n", (int)data_size);

}

unsigned char* XOR_file_decrypt(unsigned char* data, size_t data_size, const char* key) {
    DEBUG_PRINT("\nDEBUG: In function XOR_file_decrypt\n");
    unsigned char* decrypted_data = (unsigned char*)malloc(data_size);
    memcpy(decrypted_data, data, data_size);
    
    int i;
    for (i = 0; i < (int)data_size; i++) {
        decrypted_data[i] = (decrypted_data[i] ^ key[i%24]);
    }

    DEBUG_PRINT_INT("DEBUG: data_size in XOR_file_decrypt: %d\n", (int)data_size);
    return decrypted_data;
}
