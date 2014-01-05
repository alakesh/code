/* Write code to reverse a C-Style String. (C-String means that “abcd” is 
 * represented as five characters, including the null character.)
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    char tmp;
    char *str = argv[1];
    int i=0,l=0,n=0;
    char *ptr= (char *)malloc(n=strlen(str)+1);
    strncpy(ptr,str,n);
    for(i=0,l=strlen(ptr);i<l/2;i++)
    {
        tmp = *(ptr+i);
        *(ptr+i) = *(ptr+l-i-1);
        *(ptr+l-i-1) = tmp;
    }
    printf("%s\n",ptr);
    free(ptr);
}

