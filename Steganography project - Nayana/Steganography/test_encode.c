/*
NAME : NAYANA V NAIR
DESCRIPTION : To implement LSB Image Steganography
DATE : 27/10/2025
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])  
{

    if(argc < 4)
    {
        //error messages
        printf("<---------INVALID ARGUMENTS--------->\n");
        printf("FOR ENCODING : ./a.out -e beautiful.bmp secret.bmp stego.bmp\n");
        printf("FOR DECODING : ./a.out -d stego.bmp output.txt\n");

        return e_failure;
    }
    
   if(check_operation_type(argv) == e_encode)    //checks if the operation is encoding
   {
    printf("You have choosed ENCODING\n");
    EncodeInfo encInfo;      //declaring a variable for encoding operations

    if(read_and_validate_encode_args(argv, &encInfo) == e_success)   //function call to read and validate the encoding arguments
    {
        printf("Read and Validate is successful\n");
        if(do_encoding(&encInfo) == e_success)         //function call to perform encoding operations
        {
            printf("<------ENCODING SUCCESSFUL------>\n");
        }
        else
        {
            printf("ENCODING UNSUCCESSFUL\n");
            return e_failure;
        }
    }
    else
    {
        printf("Read and Validate is unsuccessful\n");
        return e_failure;
    }
   }

   else if(check_operation_type(argv) == e_decode)     //checks if the operation is decoding
   {
    printf("You have choosed DECODING\n");
    DecodeInfo decInfo;          //declaring a variable for decoding operations

    if(read_and_validate_decode_args(argv, &decInfo) == e_success)  //function call to read and validate the decoding arguments
    {
        printf("Read and Validate is successful\n");
        if(do_decoding(&decInfo) == e_success)         //function call to perform decoding operations
        {
            printf("<------DECODING SUCCESSFUL------>\n");
        }
        else
        {
            printf("DECODING UNSUCCESSFUL\n");
            return e_failure;
        }
    }
    else
    {
        printf("Read and Validate is unsuccessful\n");
        return e_failure;
    }
   }

   else
   {
    printf("It is unsupported\n");
    return e_failure;
   }

  
   return e_success;
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)     //checks the operation type (encoding)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)  //checks the operation type (decoding)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
