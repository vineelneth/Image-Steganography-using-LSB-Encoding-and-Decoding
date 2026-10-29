#include <stdio.h>
#include "types.h"
#include <string.h>
#include "decode.h"
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);    
/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_file(decInfo)==e_success)
    {
        printf("open_file is a success\n");
    }
    else
    {
        printf("open_file is a failure\n");
        return e_failure;
    }
/*  if(check_capacity(decInfo)==e_success)
    {
        printf("check_capacity is a sucess\n");
    }
    else
    {
        printf("check_capacity is a failure\n");
        return e_failure;
    }
    if(copy_bmp_header(decInfo->fptr_src_image, decInfo->fptr_stego_image)==e_success)
    {
        printf("copy bmp header is a success\n");
        
    }
    else
    {
        printf("copy bmp header is a failure\n");
        return e_failure;
    }
*/  if(decode_magic_string(decInfo)==e_success)
    {
        printf("Decoding magic string is success\n");
    }
    else
    {
        printf("Decoding magic string is failure\n");
        return e_failure;
    }
    if(decode_secret_file_extn_size(decInfo)==e_success)
    {
        printf("Decoding secret_file_extn_size is success\n");
    }
    else
    {
        printf("Decoding secret_file_extn_size is failure\n");
        return e_failure;
    }
    if(decode_secret_file_extn(decInfo)==e_success)
    {
        printf("Decoding secret_file_extn is success\n");
    }
    else
    {
        printf("Decoding secret_file_extn is failure\n");
        return e_failure;
    }
    if(decode_secret_file_size(decInfo)==e_success)
    {
        printf("Decoding secret_file_size is success\n");
    }
    else
    {
        printf("Decoding secret_file_size is failure\n");
        return e_failure;
    }
    if(decode_secret_file_data(decInfo)==e_success)
    {
        printf("Decoding secret_file_data is success\n");
    }
    else
    {
        printf("Decoding secret_file_data is failure\n");
        return e_failure;
    }
    return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo)
{
        char image_buffer[8];
        char magic_string[3];
        fseek(decInfo->fptr_src_image, 54, SEEK_SET);
        for(int i=0;i<2;i++)
        {    
            fread(image_buffer,8,1,decInfo->fptr_src_image);
            magic_string[i] = decode_byte_to_lsb(image_buffer);
        }
        magic_string[2] = '\0';
        for(int i=0;i<2;i++)
        {
            printf("magic_string[%d] -> %c\n",i,magic_string[i]);
        }
        if(strcmp(magic_string,"#*")==0)
        {
            printf("Magic string matched\n");    
            return e_success;
        }
        return e_failure;
}
/* Decode secret file extenstion size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char image_buffer[32];
    int extn_size;
    fseek(decInfo->fptr_src_image,70, SEEK_SET);
    fread(image_buffer,32,1,decInfo->fptr_src_image);
    extn_size = decode_size_to_lsb(image_buffer);
    if(extn_size==4)
        return e_success;
    return e_failure;
}
/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char image_buffer[8];
    char file_extn[5];
    fseek(decInfo->fptr_src_image,102,SEEK_SET);
    for(int i=0;i<4;i++)
    {
        fread(image_buffer,8,1,decInfo->fptr_src_image);
        file_extn[i] = decode_byte_to_lsb(image_buffer);
    }
    file_extn[4]='\0';
    if(strcmp(file_extn,".txt")==0)
    {
        printf("File extn: %s\n",file_extn);
        printf("file extn matched\n");
    }
    return e_success;
}
/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char image_buffer[32];
    uint  file_size=0;
    fseek(decInfo->fptr_src_image,134, SEEK_SET);
    fread(image_buffer,32,1,decInfo->fptr_src_image);
    file_size = decode_size_to_lsb(image_buffer);
    printf("file_size: %u bytes\n",file_size);
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char image_buffer[8];
    char str[100];
    uint dest_file_size = decode_get_file_size(decInfo->fptr_src_image,decInfo);
    fseek(decInfo->fptr_src_image,166,SEEK_SET);
    for(int i=0;i<dest_file_size;i++)
    {
        fread(image_buffer,8,1,decInfo->fptr_src_image);
        str[i] = decode_byte_to_lsb(image_buffer);
    }
    str[dest_file_size] ='\0';
    printf("%s",str);
    fseek(decInfo->fptr_dest,0,SEEK_SET);
    fwrite(str,dest_file_size,1,decInfo->fptr_dest);
    return e_success;
}
/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
        int len = strlen(argv[2]);
        if(len>4 && !(strcmp((argv[2]+len-4),".bmp")))
        {
            decInfo ->src_image_fname=argv[2];
            if(argv[3]==NULL)
            {
                decInfo->dest_fname = "decode.txt";
            }
            else
            {
                if(strlen(argv[3])>4 && !(strcmp((argv[3]+strlen(argv[3])-4),".txt")))
                {
                    decInfo->dest_fname = argv[3];
                }
                else
                {
                    return e_failure;
                }
            }
            return e_success;
        }
        else
        {
            return e_failure;
        }
}

/* check capacity */
//Status check_capacity(DecodeInfo *decInfo){}
/* Decode a byte into LSB of image data array */
char decode_byte_to_lsb(char *image_buffer)
{
        char data=0;
        for(int i=0;i<8;i++)
        {
            data = data|((image_buffer[i]&1)<<i);
        }
        return data;
}
/* Decode a size into LSB of image data array */
int decode_size_to_lsb(char *image_buffer)
{
        int data=0;
        for(int i=0;i<32;i++)
        {
            data = data|((image_buffer[i]&1)<<i);
        }
        return data;
}
/* Get file size */
uint decode_get_file_size(FILE *fptr,DecodeInfo *decInfo)
{
    char image_buffer[32];
    uint  file_size=0;
    fseek(decInfo->fptr_src_image,134, SEEK_SET);
    fread(image_buffer,32,1,decInfo->fptr_src_image);
    file_size = decode_size_to_lsb(image_buffer);
  //  printf("file_size: %u bytes\n",file_size);
    return file_size;
}
uint decode_get_image_size_for_bmp(FILE *fptr_image)
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
Status open_file(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

    	return e_failure;
    }
    // dest file
    decInfo->fptr_dest = fopen(decInfo->dest_fname, "w");
    // Do Error handling
    if (decInfo->fptr_dest == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->dest_fname);

    	return e_failure;
    }
    // No failure return e_success
    return e_success;
}
