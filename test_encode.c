#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"
int main(int argc, char* argv[])
{
    EncodeInfo E1;
    DecodeInfo D1; 
    if(argc<3)
    {
        printf("Please enter ./a.out -e/-d beautiful.bmp/encoded.bmp secret.txt\n");
        return 0;
    }
    int ret = check_operation_type(argv);
    if(ret==e_encode)
    {
        printf("User selected encoding\n");
        if(read_and_validate_encode_args(argv,&E1)==e_success)
        {
            printf("read_and_validate_encode_args is a success\n");
            if(do_encoding(&E1)==e_success)
            {
                printf("Encoding is a success\n");
            }
            else
            {
                printf("Encoding is a failure\n");
                return e_failure;
            }
        }
        else
        {
            printf("read_and_validate_encode_args is a failure\n");
            return e_failure;
        }
    }
    else if(ret==e_decode)
    {
        printf("User selected decoding\n");
        if(read_and_validate_decode_args(argv,&D1)==e_success)
        {
            printf("read_and_validate_decode_args is a success\n");
            if(do_decoding(&D1)==e_success)
            {
                printf("Decoding is a success\n");
            }
            else
            {
                printf("Decoding is a failure\n");
                return e_failure;
            }
        }
        else
        {
            printf("read_and_validate_decode_args is a failure\n");
            return e_failure;
        }

    }
    else
    {
        printf("Invalid option\n");
    }
}
OperationType check_operation_type(char *argv[])
{
        if(strcmp(argv[1],"-e")==0)
        {
            return e_encode;
        }
        else if(strcmp(argv[1],"-d")==0)
        {
            return e_decode;
        }
        else
        {
            return e_unsupported;
        }
}
