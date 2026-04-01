#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char copy1[20];
    strcpy(copy1, argv[2]);

    if(copy1[0] == '.')   //checks if the argument begins with '.'
        {
            printf("ERROR.....Argument passed is not proper as it should not begin with '.' \n");
            return e_failure;
        }


    if(strstr(argv[2], ".bmp") != NULL)  //checks if stego.bmp is present or not
    {
        printf("stego.bmp is present\n");
        decInfo -> steg_name = argv[2];  
    }
    else
    {
        printf("The .bmp extension is not proper\n");
        return e_failure;
    }


    if(argv[3] != NULL)  //checks if argument in that index is present or not
    {
        char *token;
        char copy2[20];
       
        strcpy(copy2, argv[3]);
        
        token = strtok(argv[3],".");

        if(copy2[0] == '.')    //checks if the argument begins with '.'
        {
            printf("ERROR.....Argument passed is not proper as it should not begin with '.' \n");
            return e_failure;
        }

        if(token != NULL)
        {
            if(strstr(copy2, ".txt") != NULL)
            {
                printf("output.txt is present\n");
                strcpy(decInfo -> out, token);  
            }
            else
            {
                printf("ERROR : The .txt extension is not proper\n");
                return e_failure;
            }
  
        }
    }  
    else
    {
        strcpy(decInfo -> out, "output");
        //printf("output.txt is not present\n");
    }

    input_magic_str(); //function call to input magic string from the user

    return e_success;
}



Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_dec(decInfo) == e_success) //function call to check if all the files are opened or not
    {
        printf("All files are opened successfully\n");
    }
    else
    {
        printf("Files are not opened successfully\n");
        return e_failure;
    }

    //function call to check if decoding magic string is success or not
    if(decode_magic_string(mag_str, decInfo) == e_success)
    {
        printf("Magic String is Decoded SUCCESSFULLY...!\n");
    }
    else
    {
        printf("Magic String Decoding UNSUCCESSFUL...!\n");
        return e_failure;
    }

    //function call to check if decoding secret file extension is success or not
    if(decode_secret_file_extn(decInfo) == e_success)
    {
        printf("Secret File Extension Decoded SUCCESSFULLY...\n");
    }
    else
    {
        printf("Decoding Secret File Extension UNSUCCESSFUL...\n");
        return e_failure;
    }

    //function call to check if decoding secret file size is success or not
    if(decode_secret_file_size(decInfo) == e_success)
    {
        printf("Secret file size Decoded SUCCESSFULLY...!\n");
    }
    else
    {
        printf("Decoding Secret file size UNSUCCESSFUL...!\n");
        return e_failure;
    }

    //function call to check if decoding secret file data is success or not
    if(decode_secret_file_data(decInfo) == e_success)
    {
        printf("Secret file data Decoded SUCCESSFULLY...!\n");
    }
    else
    {
        printf("Decoding Secret file data UNSUCCESSFUL...!\n");
        return e_failure;
    }

    return e_success;

}

Status open_files_dec(DecodeInfo *decInfo)
{
   decInfo -> fptr_steg = fopen(decInfo -> steg_name, "r"); //opens the stego file in read mode
    
    if(decInfo -> fptr_steg == NULL) //checks if stego file is present or not
    {
        printf("Stego file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Stego file is opened\n");
    }

    return e_success;
 
}

             /*Decoding Magic String*/
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo -> fptr_steg, 54, SEEK_SET); //the file pointer will point at the position after 54 bytes

    char magic_str[32]; //declaring an array

    fread(magic_str, 32, 1, decInfo -> fptr_steg); //reads 32 bytes from stego.bmp
    decInfo -> mag_len = decode_size_from_lsb(magic_str); //function call to decode size from lsb

    printf("Size----> %d\n",decInfo -> mag_len);

    //function call to decode magic string from stego.bmp
    decode_magic_from_data(decInfo -> mag_len, decInfo -> fptr_steg, decInfo);

    return e_success;
}
                     /*Decoding Magic String*/
Status decode_magic_from_data(int size, FILE *fptr_steg_image, DecodeInfo *decInfo)
{
    char magic[10];

    int i;

    for(i = 0; i < size; i++)
    {
        fread(magic, 8, 1, fptr_steg_image); //reads 8 bytes from stego.bmp
        decInfo -> decode_string[i] = decode_byte_from_lsb( magic ); //function call to decode byte from lsb
    }
    decInfo -> decode_string[size] = '\0';

    //checks if the entered magic string & the decoded magic string is same or not
    if(strcmp(decInfo -> decode_string, mag_str) == 0) 
    {
        printf("Magic String MATCHED...\n");
    }
    else
    {
        printf("Magic String MISMATCHED...\n");
        return e_failure;
    }

    for(i = 0; i < size; i++)
    {
        printf("%c",decInfo -> decode_string[i]); //prints the magic string
    }
    printf("\n");

}

      /*Decoding byte from LSB*/
Status decode_byte_from_lsb(char *image_buffer)
{
    char dec = 0;
    int i;
    for(i = 0; i < 8; i++)
    {
      dec = (dec << 1) | (image_buffer[i] & 1);  
    }

    return dec;
}

     /*Decoding the secret file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char arr[32];

    fread(arr, 32, 1, decInfo -> fptr_steg); //reads 32 bytes from stego.bmp
    decInfo -> secret_file_extn_size = decode_size_from_lsb(arr); //decoding size from lsb

    printf("Size----> %d\n",decInfo -> secret_file_extn_size); //prints the size of secret file extension

    //function call to decode secret file extension
    decode_extn_from_data(decInfo -> secret_file_extn_size, decInfo -> fptr_steg, decInfo);

    return e_success;
}

      /*Decoding size from LSB*/
Status decode_size_from_lsb(char *image_buffer)
{
    int dec = 0;
    int i;
    for(i = 0; i < 32; i++)
    {
        dec =  (dec << 1) | (image_buffer[i] & 1);      
    }

    return dec;
}

         /*Decoding secret file extension from stego.bmp*/
Status decode_extn_from_data(int size, FILE *fptr_steg_image, DecodeInfo *decInfo)
{
        char extn[10];
        char str[20];

    int i;

    for(i = 0; i < size; i++)
    {
        fread(extn, 8, 1, fptr_steg_image); //reads 8 bytes from stego.bmp
        decInfo -> decode_extn[i] = decode_byte_from_lsb( extn ); //decoding bytes from lsb
    }
    decInfo -> decode_extn[size] = '\0';

    printf("Extn---> %s\n",decInfo -> decode_extn); //prints the secret file extension

    //concatenates the output filename with the decoded secret file extension
    char *concat = strcat(decInfo -> out, decInfo -> decode_extn);

    strcpy(str, concat); //copies the result to another array

    decInfo -> fptr_out = fopen(str, "w"); //opens the output file in write mode
    
    if(decInfo -> fptr_out == NULL) //checks if output file is present or not
    {
        printf("Output file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Output file is opened\n");
    }
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char sec_arr[32];

    fread(sec_arr, 32, 1, decInfo -> fptr_steg);   //reads 32 bytes from stego.bmp
    decInfo -> secret_file_size = decode_size_from_lsb(sec_arr);   //decoding size from lsb


    printf("Size Secret ---> %d\n",decInfo -> secret_file_size);   //prints the secret file size

    return e_success;
}

      /*Decoding secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    //decoding secret file data from stego.bmp
     decode_secret_from_data( decInfo -> secret_file_size, decInfo -> fptr_steg, decInfo);

     return e_success;
}

                       /*Decoding secret file data*/
Status decode_secret_from_data(int size, FILE *fptr_steg_image, DecodeInfo *decInfo)
{
    char secret[8];

    int i;

    for(i = 0; i < size; i++)
    {
        fread(secret, 8, 1, fptr_steg_image);   //reads 8 bytes from stego.bmp
        decInfo -> decode_secret[i] = decode_byte_from_lsb( secret );    //decoding byte from lsb
    }
    decInfo -> decode_secret[size] = '\0';

    printf("Secret Data ---> %s",decInfo -> decode_secret); //prints secret file data

    for(i = 0; i < decInfo -> secret_file_size; i++)
    {
        fputc(decInfo -> decode_secret[i], decInfo -> fptr_out);  //writing the secret file data into output.txt
    }

    return e_success;
    
}    