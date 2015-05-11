/* A simple version of game of life 
 * 
 * By <Alakesh.Haloi@gmail.com>
 **/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define X 8
#define Y 8

/* Rules */

/* If number of live neighbours < 2 : dies
 * If number of live neighbours 2 or 3 : lives
 * If number of live neighbours > 3 : dies
 * if dead and number of live neighbours  = 3 : becomes alive
 */

int life[X][Y] = {0};
int temp[X][Y] = {0};

int live_neighbours(int i, int j){
	int count=0;
	if (i-1 >= 0)
		count = count + life[i-1][j];

	if (j-1 >= 0)
		count = count + life[i][j-1];

	if (i+1 < X)
		count = count + life[i+1][j];

	if (j+1 < Y)
		count = count + life[i][j+1];

	if (i-1 >=0 && j-1 >= 0) 
		count = count + life[i-1][j-1];

	if (i+1 < X && j+1 < Y) 
		count = count + life[i+1][j+1];

	if (i-1 >=0 && j+1 < Y) 
		count = count + life[i-1][j+1];

	if (i+1 < X && j-1 >= 0) 
		count = count + life[i+1][j-1];
	return count;
}

void evolve() {
	int i, j, count;
	for (i=0; i < X; i++){
		for(j=0; j < Y; j++){
			temp[i][j] = life[i][j];
			count = live_neighbours(i, j);
			if(life[i][j] == 0){
				if (count == 3)
					temp[i][j] = 1;
			} else {
				if (count < 2)
					temp[i][j] = 0;
				if (count & 0x2)
				 	temp[i][j] = 1;
				if (count > 3)
					temp[i][j] = 0;
			}
		}
	}

	for (i=0; i < X; i++)
		for(j=0; j < Y; j++)
			life[i][j] = temp[i][j];
}

void print_life() {
	int i,j;
	for (i=0; i < X; i++)
		for(j=0; j < Y; j++)
			printf("%d %c", life[i][j], (j==Y-1 ? '\n' : ' '));
	printf("\n\n");
}

int main(int argc, char *argv[]) {
	int rnum = 0,i;
	int iter=0;
	printf("Welcome to Game of Life \n");

	srand(time(NULL));

	for (i=0; i < X*Y; i++){
		*(*life + i) = ((rand()%10) > 4)?1:0;
	}

	printf("At Start \n");
	print_life();
	sleep(5);

	for(;;){
		printf("After %d iteration \n",++iter);
		evolve();
		print_life();
		sleep(2);
	}

	return 0;
}
