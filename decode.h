#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Destination File Info */
    char *dest_fname;
    FILE *fptr_dest;
    char extn_dest_file[MAX_FILE_SUFFIX];
    char dest_data[MAX_SECRET_BUF_SIZE];
    long size_dest_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;


/* Decoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_file(DecodeInfo *decInfo);

/* check capacity */
//Status check_capacity(DecodeInfo *decInfo);

/* Get image size */
uint decode_get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint decode_get_file_size(FILE *fptr, DecodeInfo *DecodeInfo);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);
/* Decode secret file extenstion size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);
/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Decode a byte into LSB of image data array */
char decode_byte_to_lsb(char *image_buffer);

/* Decode a size into LSB of image data array */
int decode_size_to_lsb(char *image_buffer);

/* Copy remaining image bytes from src to stego image after decoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
