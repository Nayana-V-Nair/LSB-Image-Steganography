#include "types.h"

typedef struct _DecodeInfo
{
    FILE *fptr_steg;
    FILE *fptr_out;
    char out[10];
    char *steg_name;
    char decode_string[10];
    char decode_extn[10];
    char decode_secret[60];

    int secret_file_size;
    int secret_file_extn_size;

    int mag_len;   //to store the length of the magic string
    
}DecodeInfo;


/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_dec(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_magic_from_data(int size, FILE *fptr_steg_image, DecodeInfo *decInfo);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char *image_buffer);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode size from LSB of image data array */
Status decode_size_from_lsb(char *image_buffer);

/* Decode secret file extenstion */
Status decode_extn_from_data(int size, FILE *fptr_steg_image, DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_from_data(int size, FILE *fptr_steg_image, DecodeInfo *decInfo);
