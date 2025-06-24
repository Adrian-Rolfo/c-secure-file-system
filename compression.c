#include <stdlib.h>
#include <stdio.h> /*fopen(), fwrite(), fclose(), printf()*/
#include <string.h>
#include "compression.h" 
#include "debug.h"
/*void display_buffer(), void file_RLE_compress(), void file_RLE_decompress()*/

/*************************************************
Author: Adrian Rolfo 24818686

Description:
displays all compressed data in formatted output

*************************************************/
void display_buffer(unsigned char* compressed_data, size_t compressed_size) { 
    DEBUG_PRINT("\nDEBUG: In function display_buffer\n");
    int i;
    for(i = 0; i < compressed_size; i = i + 2) {
        printf("Data: %c Count: %d\n", (char)compressed_data[i], (int)compressed_data[i + 1]);
        }
}


/*************************************************
Author: Adrian Rolfo 24818686

Description:
Applies RLE compression algorithm to input data

input:
- data: data stored as an unsigned char array to be compressed
- decompressed size: size of data before compression
- compressed size: a pointer to compressed size that is updated after RLE compression

output: 
- compressed_data: heap allocated unsigned char array containing compressed data information

*************************************************/
unsigned char* file_RLE_compress(unsigned char* data, size_t decompressed_size, size_t* compressed_size) {
    DEBUG_PRINT("\nDEBUG: In function file_RLE_compress\n");
    unsigned char* compressed_data = (unsigned char*)malloc(2*decompressed_size);

    int i = 0, write_index = 0;
    while (i < decompressed_size) {
        unsigned char byte = data[i];
        size_t count = 1;
        /* counts consecutive bits and traverses array accordingly up until EOF
        or count exceeds 255 which is greater than what a bit can display */
        while (i + 1 < decompressed_size && data[i] == data[i + 1] && count < 255) {
            i++;
            count++;
        }
        compressed_data[write_index++] = byte;
        compressed_data[write_index++] = count;
        i++;
    }
    *compressed_size = write_index;
    
    DEBUG_PRINT_INT("DEBUG: decompressed size in file_RLE_compress: %d\n", (int)decompressed_size);
    DEBUG_PRINT_INT("DEBUG: compressed size in file_RLE_compress: %d\n", *(int*)compressed_size);

    return compressed_data;
}

/*************************************************
Author: Adrian Rolfo 24818686

Description:
Applies RLE decompression algorithm to input data, and uploads data to file
designated by file_path

input:
- file_path: filepath for data to be written too.
- data: compressed data stored as an unsigned char
- compressed size: size of data before decompression
- decompressed size: size of data before decompression

output: 
- void

*************************************************/
void file_RLE_decompress(char* file_path, unsigned char* data, size_t compressed_size, size_t decompressed_size) {
    DEBUG_PRINT("\nDEBUG: In function file_RLE_decompress\n");
    unsigned char decompressed_data[decompressed_size];

    size_t i, write_index = 0;
    for(i = 0; i < compressed_size; i += 2) {
        unsigned char byte = data[i];
        unsigned char count = data[i + 1];
        
        /* writes 'count' bytes to decompressed_data file */
        unsigned char j;
        for(j = 0; j < count; j++) {
            if (write_index >= decompressed_size) {
                printf("Error: write index greater than decompressed_size\n");
                return;
            }
            decompressed_data[write_index++] = byte;
        }
    }

    FILE* fdest = fopen(file_path, "wb");
    if (fdest == NULL) {
        printf("Error: failure opening file in file_RLE_compress %s\n", file_path);
        return;
    }
    fwrite(decompressed_data, decompressed_size, 1, fdest);
    fclose(fdest);

    DEBUG_PRINT_INT("DEBUG: decompressed size in file_RLE_compress: %d\n", (int)decompressed_size);
    DEBUG_PRINT_INT("DEBUG: compressed size in file_RLE_compress: %d\n", (int)compressed_size);
}

