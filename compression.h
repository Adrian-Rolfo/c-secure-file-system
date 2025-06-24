#ifndef COMPRESSION_H
#define COMPRESSION_H
#include <stdio.h>

/*displays all compressed data in formatted output*/
void display_buffer(unsigned char* compressed_data, size_t compressed_size);

/*Applies RLE compression algorithm to input data*/
unsigned char* file_RLE_compress(unsigned char* data, size_t decompressed_size, size_t* compressed_size);

/*Applies RLE decompression algorithm to input data, and uploads data to file designated by file_path*/
void file_RLE_decompress(char* file_path, unsigned char* data, size_t compressed_size, size_t decompressed_size);

#endif
