#include <stdio.h>
#include <stdlib.h>


/* Rotate a given array of numbers of size S by P steps without using any 
 * additional space 
 * Complexity of 1 step rotation for array of size N is O(N);
 */

/* Algorithm
 *
 * Take an example of  5 numbers that we need to rotate by 1 step
 *                A    B    C   D   E
 * So start with  swap (A, B). The new array becomes
 *                B    A    C   D   E
 * 1st swap moves A to the right destination location
 * Now repeat it for B but this time to B's new location which is 1 more than A
 * So swap(B,C)
 * 				  C    A    B   D   E
 * Repeat this till E
 *
 * In a nutshell, it will look like this
 *
 * 				A B C D E
 * swap(0,1)    B A C D E
 * swap(0,2)    C A B D E
 * swap(0,3)    D A B C E
 * swap(0,4)    E A B C D
 *
 *
 *
 * The idea is to use location 0 as a temporary location
 * Repeat the above for N steps
 */


int *arr = NULL;

void help(char *prog)
{
	printf("Usage:  %s  <size>  <list of numbers>  <rotate-by> \n", prog);
}

void swap(int *a, int *b)
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}


void print(int *arr, int count)
{
	int i;
	for(i=0; i<count; i++)
		printf(" %d ", *(arr + i));
	printf("\n");
}

void rotate(int *arr, int count, int step)
{
	int i, j;
	for (j=0; j<step; j++){
		for (i=1; i < count; i++){
			swap(arr, arr+i); 
		}
		printf("RUN %d \n", j);
		print(arr, count);
	}
}

int main(int argc, char *argv[]){
	int count = -1000;
	int step = 0, i = 0;
	if (argc > 1) {
		count = strtoul(argv[1], NULL, 0);
		if (count != argc - 3)
		{
			help(argv[0]);
			return 0;
		} 

		arr = (int *) malloc (sizeof(int) * count);
		for (i = 0; i < count; i++)
			*(arr + i) = strtoul(argv[i+2], NULL, 0);

		printf("Given array : \n");
		print(arr, count);

		step = strtoul(argv[argc-1], NULL, 0);

		rotate(arr, count, step);
		free(arr);

	} else {
		help(argv[0]);
	}
}
