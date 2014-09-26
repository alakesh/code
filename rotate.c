#include <stdio.h>
#include <stdlib.h>


/* Rotate a given array of numbers of size S by P steps without using any 
 * additional space 
 * Complexity of 1 step rotation for array of size N is O(N);
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
