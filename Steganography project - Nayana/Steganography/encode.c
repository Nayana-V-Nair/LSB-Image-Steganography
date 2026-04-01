#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */


 Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
 {
    char copy1[20];
    strcpy(copy1, argv[2]);

    if(copy1[0] == '.')   //checks if the argument begins with '.'
        {
            printf("ERROR.....Argument passed is not proper as it should not begin with '.' \n");
            return e_failure;
        }

    if(strstr(argv[2], ".bmp") != NULL)  //checks if beautiful.bmp argument is present or not
    {
        printf("beautiful.bmp is PRESENT\n");
        encInfo -> src_image_fname = argv[2];  
    }
    else
    {
        printf("ERROR ----> The .bmp extension is not proper....!\n");
        return e_failure;
    }

    char copy2[20];
    strcpy(copy2, argv[3]);

    if(copy2[0] == '.')   //checks if the argument begins with '.'
        {
            printf("ERROR.....Argument passed is not proper as it should not begin with '.' \n");
            return e_failure;
        }

    if(strstr(argv[3], ".txt") != NULL)  //checks if secret.txt argument is present or not
    {
        printf("secret.txt is PRESENT\n");
        encInfo -> secret_fname = argv[3];  
    }
    else
    {
        printf("ERROR ----> The .txt extension is not proper....!\n");
        return e_failure;
    }

    if(argv[4] != NULL)        //checks if argument is present or not
    {
        char copy3[20];
        strcpy(copy3, argv[4]);

    if(copy3[0] == '.')  //checks if the argument begins with '.'
        {
            printf("ERROR.....Argument passed is not proper as it should not begin with '.' \n");
            return e_failure;
        }

       if(strstr(argv[4], ".bmp") != NULL)  //checks if stego.bmp argument is present or not
      {
        printf("stego.bmp is PRESENT\n");
       encInfo -> stego_image_fname = argv[4];  
      }
      else
      {
        printf("The .bmp extension is not proper.....!\n");
        return e_failure;
      }
    }
    else
    {
        encInfo -> stego_image_fname = "stego.bmp";  //if not it will create it
    }    

    input_magic_str();   //function call to input magic string

    return e_success;
 }

 Status do_encoding(EncodeInfo *encInfo)
 {

    if(open_files(encInfo) == e_success)   //checks if all files are opened or not
    {
        printf("All files are opened successfully\n");
    }
    else
    {
        printf("Files are not opened successfully\n");
        return e_failure;
    }

    if(check_capacity(encInfo) == e_success)   //checks the capacity of beautiful.bmp and secret.txt
    {
        printf("Successfully calculated the size of beautiful.bmp and secret.txt size...\n");
    }
    else
    {
        printf("Calculating the size of beautiful.bmp and secret.txt is unsuccessful...\n");
        return e_failure;
    }

    /* copying the header bytes of beautiful.bmp to stego.bmp */
    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success) 
    {
        printf("The header bytes of beautiful.bmp is SUCCESSFULLY COPIED to stego.bmp!!\n");
    }
    else
    {
        printf("Copying the header bytes of beautiful.bmp to stego.bmp is UNSUCCESSFUL!!\n");
        return e_failure;
    }

    if(encode_magic_string(mag_str, encInfo) == e_success) // checks if encoding magic string is success or not
    {
        printf("Magic String encoded SUCCESSFULLY...\n");
    }
    else
    {
        printf("Magic String encoding UNSUCCESSFUL...\n");
        return e_failure;
    }

     // checks if encoding secret file extension is success or not
    if(encode_secret_file_extn(strstr(encInfo -> secret_fname, ".txt"), encInfo) == e_success) 
    {
        printf("Encoding Secret file Extension & Secret file size SUCCESSFUL...!\n");
    }
    else
    {
        printf("Encoding Secret file Extension & Secret file size UNSUCCESSFUL...!\n");
        return e_failure;
    }

     // checks if encoding secret file size is success or not
    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
    {
        printf("Secret file size encoded SUCCESSFULLY...\n");
    }
    else
    {
        printf("Encoding Secret file size UNSUCCESSFUL...\n");
        return e_failure;
    }

    if(encode_secret_file_data(encInfo) == e_success) //checks if encoding secret file data is success or not
    {
        printf("Secret file data encoded SUCCESSFULLY...!\n");
    }
    else
    {
        printf("Encoding Secret file data UNSUCCESSFUL...!\n");
        return e_failure;
    }

    //checks if copying the remaining data is success or not
    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("Copying remaining data SUCCESSFUL...\n");
    }
    else
    {
        printf("Copying remaining data UNSUCCESSFUL...\n");
        return e_failure;
    }


    return e_success;
 }

 Status open_files(EncodeInfo *encInfo)
 {
    encInfo -> fptr_src_image = fopen(encInfo -> src_image_fname, "r"); //opens the source file in read mode
    
    if(encInfo -> fptr_src_image == NULL) //checks if source file is present or not
    {
        printf("Source file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Source file is opened\n");
    }

    encInfo -> fptr_secret = fopen(encInfo -> secret_fname, "r"); //opens the secret file in read mode
    
    if(encInfo -> fptr_secret == NULL)  //checks if secret file is present or not
    {
        printf("Secret file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Secret file is opened\n");
    }

    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname, "w"); //opens the stego file in write mode

        printf("Stego file is opened\n");

    return e_success;
 }

 Status check_capacity(EncodeInfo *encInfo)
 {
    //calculating the size of beautiful.bmp
    if(get_image_size_for_bmp(encInfo -> fptr_src_image, encInfo) == e_success)
    {
        printf("Size of beautiful.bmp calculated SUCCESSFULLY!!\n");
    }
    else
    {
        return e_failure;
    }

    //calculating the size of secret.txt
    if(get_file_size(encInfo -> fptr_secret,encInfo) == e_success)
    {
        printf("Size of secret.txt calculated SUCCESSFULLY!!\n");
    } 
    else
    {
        return e_failure;
    }

    //checks if the beautiful.bmp file has enough capacity to hide the required data
    if(encInfo -> image_capacity < (54 + (2 * 4 * 4 * 4 * (encInfo -> size_secret_file)) * 8))
    {
        printf("The image capacity is not sufficient to hide the required data...!!\n");
        return e_failure;
    }

    return e_success;
 }


 uint get_image_size_for_bmp(FILE *fptr_image, EncodeInfo *encInfo)
 {
    int width, height;

    fseek(fptr_image, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, fptr_image); //finding the width of beautiful.bmp file

    fseek(fptr_image, 22, SEEK_SET);
    fread(&height, sizeof(int), 1, fptr_image); //finding the height of beautiful.bmp file

    int image_size = width * height * 3;      //calculating the image size
    
    encInfo -> image_capacity = image_size;

    return e_success;
 }

 uint get_file_size(FILE *fptr, EncodeInfo *encInfo)
 {
    fseek(fptr, 0, SEEK_END);   //finding the secret file size
    
    encInfo -> size_secret_file = ftell(fptr);
    
    printf("Secret File size ----> %ld\n", encInfo -> size_secret_file);

    return e_success;
 }

 Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
 {
    rewind(fptr_src_image);  //making the file pointer to point at the beginning of the file
    int arr[54];
    fread(arr, 54, 1, fptr_src_image); //reads the 54 header bytes from beautiful.bmp
    fwrite(arr, 54, 1, fptr_dest_image); //writes the 54 header bytes to stego.bmp

    return e_success;
 }

 Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
 {
    int size = strlen(mag_str); //finds the size of magic string
    char arr[32];

    fread(arr, 32, 1, encInfo -> fptr_src_image); //reads 32 bytes from beautiful.bmp

    encode_size_to_lsb(size, arr);   //function call to encode size to lsb

    fwrite(arr, 32, 1, encInfo -> fptr_stego_image); //writes 32 bytes to stego.bmp

    //encodes the magic string to stego.bmp
    encode_data_to_image(mag_str, strlen(mag_str), encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

    return e_success;

 }

 Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
 {
    char arr[8];
    int i;
    for(i = 0; i < size; i++)
    {
        fread(arr, 8, 1, fptr_src_image);  //reads 8 bytes from beautiful.bmp
        
        encode_byte_to_lsb(data[i], arr);  //function call to encode a byte to lsb

        fwrite(arr, 8, 1, fptr_stego_image);  //writes 8 bytes to stego.bmp
    }
 }

             /* Encoding byte to LSB*/
 Status encode_byte_to_lsb(char data, char *image_buffer)
 {
    int i;
    int n = 7;

    for(i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & (~1)) | ((data >> n) & 1); 
        n--;              
    }
 }

 Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
 {
    int size = strlen(file_extn);  //calculating the size of secret file extension
    char arr[32];

    fread(arr, 32, 1, encInfo -> fptr_src_image);  //reads 32 bytes from beautiful.bmp

    encode_size_to_lsb(size, arr);   //function call to encode size to lsb

    fwrite(arr, 32, 1, encInfo -> fptr_stego_image);  //writes 32 bytes to stego.bmp

    //encoding the secret file extension to stego.bmp
    encode_data_to_image(file_extn, strlen(file_extn), encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

    return e_success;
}

           /* Encoding size to LSB*/
Status encode_size_to_lsb(int data, char *image_buffer)
{
   int i;
    int n = 31;

    for(i = 0; i < 32; i++)
    {
        image_buffer[i] = (image_buffer[i] & (~1)) | ((data >> n) & 1); 
        n--;              
    } 
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char arr[32];

    fread(arr, 32, 1, encInfo -> fptr_src_image);  //reads 32 bytes from beautiful.bmp

    encode_size_to_lsb(file_size, arr);   //function call to encode size to lsb

    fwrite(arr, 32, 1, encInfo -> fptr_stego_image);  //writes 32 bytes to stego.bmp

    return e_success;

}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo -> fptr_secret);  //making the pointer to point at the beginning of the secret file

    char buffer[encInfo -> size_secret_file + 1];  //declaring a character array

    fread(buffer, encInfo -> size_secret_file, 1, encInfo -> fptr_secret); //reads the secret file data

    //encoding the secret file data into stego.bmp
    encode_data_to_image(buffer, encInfo -> size_secret_file, encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
  
   return e_success; 
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;

    while(fread(&ch, 1, 1, fptr_src) == 1)  //reading character by character from the source
    {
        fwrite(&ch, 1, 1, fptr_dest); //writing those characters into the destination
    }
    return e_success;
}
