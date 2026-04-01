#include<stdio.h>
#include "common.h"

char mag_str[50]; //declaring a character array

/*function defintion of the function to input magic string*/

void input_magic_str() 
{
    printf("Enter the magic string: ");
    scanf("%s",mag_str);  //reads the magic string
}