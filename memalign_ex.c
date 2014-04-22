#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

int main(void){
	void *ptr = memalign(sizeof(void *)*sizeof(int), 1024);
	printf("%p\n",ptr);
	strcpy((char *)ptr + (sizeof (void *) * 1024),"Alakesh");
	printf("%s\n",ptr + (sizeof (void *) * 1024));
	free(ptr);
}

