#include <stdio.h>
#include "types.h"
#include <string.h>
#include "encode.h"
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status encode_secret_file_extn_size(int data, EncodeInfo *encInfo);    
/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
        printf("open_files is a success\n");
    }
    else
    {
        printf("open_files is a failure\n");
        return e_failure;
    }
    if(check_capacity(encInfo)==e_success)
    {
        printf("check_capacity is a sucess\n");
    }
    else
    {
        printf("check_capacity is a failure\n");
        return e_failure;
    }
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
        printf("copy bmp header is a success\n");
        
    }
    else
    {
        printf("copy bmp header is a failure\n");
        return e_failure;
    }
    if(encode_magic_string("#*",encInfo)==e_success)
    {
        printf("Encoding magic string is success\n");
    }
    else
    {
        printf("Encoding magic string is failure\n");
        return e_failure;
    }
    if(encode_secret_file_extn_size(4,encInfo)==e_success)
    {
        printf("Encoding secret_file_extn_size is success\n");
    }
    else
    {
        printf("Encoding secret_file_extn_size is failure\n");
        return e_failure;
    }
    if(encode_secret_file_extn_size(4, encInfo)==e_success)
    {
        printf("encode_secret_file_extn_size is a success\n");
    }
    else
    {
        printf("encode_secret_file_extn_size is a failure\n");
        return e_failure;
    }
    if(encode_secret_file_extn(".txt", encInfo)==e_success)
    {
        printf("encode_secret_file_extn is a success\n");
    }
    else
    {
        printf("encode_secret_file_extn is a failure\n");
        return e_failure;
    }
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
    {
        printf("encode_secret_file_size is a success\n");
    }
    else
    {
        printf("encode_secret_file_size is a failure\n");
        return e_failure;
    }
    char str[100];
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    fread(str, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    printf("%s",str);
    if(encode_secret_file_data(str,encInfo)==e_success)
    {
        printf("encode_secret_file_data is a success\n");
    }
    else
    {
        printf("encode_secret_file_data is a failure\n");
        return e_failure;
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
        printf("copy_remaining_img_data is a success\n");
    }
    else
    {
        printf("copy_remaining_img_data is a failure\n");
        return e_failure;
    }
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
        char image_buffer[8];
        fseek(encInfo->fptr_src_image, 54, SEEK_SET);
        fseek(encInfo->fptr_stego_image, 54, SEEK_SET);
        for(int i=0;i<2;i++)
        {    
            fread(image_buffer,8,1,encInfo->fptr_src_image);
            encode_byte_to_lsb(magic_string[i],image_buffer);
            fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
        }
        return e_success;
}
/* Encode secret file extenstion size*/
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    fseek(encInfo->fptr_src_image,70, SEEK_SET);
    fseek(encInfo->fptr_stego_image,70, SEEK_SET);
    fread(image_buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(extn_size,image_buffer);
    fwrite(image_buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}
/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char image_buffer[8];
    fseek(encInfo->fptr_src_image,102,SEEK_SET);
    fseek(encInfo->fptr_stego_image,102,SEEK_SET);
    for(int i=0;i<4;i++)
    {
        fread(image_buffer,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i],image_buffer);
        fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    fseek(encInfo->fptr_src_image,134, SEEK_SET);
    fseek(encInfo->fptr_stego_image,134, SEEK_SET);
    fread(image_buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,image_buffer);
    fwrite(image_buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}

/* Encode secret file data*/
Status encode_secret_file_data(char *str,EncodeInfo *encInfo)
{
    char image_buffer[8];
    fseek(encInfo->fptr_src_image,166,SEEK_SET);
    fseek(encInfo->fptr_stego_image,166,SEEK_SET);
    for(int i=0;i<encInfo->size_secret_file;i++)
    {
        fread(image_buffer,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(str[i],image_buffer);
        fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src)==1)
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}
/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
        int len = strlen(argv[2]);
        if(len>4 && !(strcmp((argv[2]+len-4),".bmp")))
        {
            encInfo ->src_image_fname=argv[2];
            if(strlen(argv[3])>4 && !(strcmp((argv[3]+strlen(argv[3])-4),".txt")))
            {
                encInfo->secret_fname = argv[3];
            }
            else
            {
                return e_failure;
            }
            if(argv[4]==NULL)
            {
                encInfo->stego_image_fname = "Stego.bmp";
                return e_success;
            }
            else
            {
                if(strlen(argv[4])>4 && !(strcmp((argv[4]+strlen(argv[4])-4),".bmp")))
                {
                    encInfo->stego_image_fname = argv[4];
                    return e_success;
                }
                else
                {
                    return e_failure;
                }
            }
        }
        else
        {
            return e_failure;
        }
}

/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if(encInfo->image_capacity>(16+32+32+32+encInfo->size_secret_file))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
        for(int i=0;i<8;i++)
        {
            image_buffer[i] = ((image_buffer[i] & 0xFE) | ((data>>i)&1));
        }
}
/* Encode a size into LSB of image data array */
Status encode_size_to_lsb(int data, char *image_buffer)
{
        for(int i=0;i<32;i++)
        {
            image_buffer[i] = (image_buffer[i]&(0xFE))|((data>>i)&1);
        }
}
/* Get file size */
uint get_file_size(FILE *fptr)
{
    uint count=0;
    while(fgetc(fptr)!=EOF)
    {
        count++;
    }
    return count;
}
/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char arr[54];
    fseek(fptr_src_image, 0, SEEK_SET);
    fread(arr,54,1,fptr_src_image);
    fwrite(arr,54,1,fptr_dest_image);
    return e_success;
}
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}