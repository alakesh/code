/* Implement an algorithm to determine if a string has all unique characters. What if you cannot use additional data structures */

/* Algorithm
 * Maximum length of a string which has all characters unique is 128
 * Declare an array of int where the index is the ascii value of the
 * character it would hold counts for. 
 *
 * For each input string, as we encounter one char, increase the count
 * for corresponding position. If we see that count is > 1 at any time for
 * any character, then its not duplicate */

#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[])
{
    int a;
    int i;
    int arr[128]={0};
    if (argc < 2)
    {
        printf("Usage: ./%s  <str>\n",argv[0]);
        return 0;
    }

    for(i=0;i<strlen(argv[1]);i++){
        arr[argv[1][i]]++;
        if(arr[argv[1][i]] > 1)
        {
            printf("Duplicate character seen\n");
            return 0;
        }
    }
    printf("All unique characters in string\n");
}
