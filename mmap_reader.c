#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#define BUFSIZE 16
int main (int argc, char *argv[])
{
	int fdin;
	char *src;
	struct stat statbuf;
	char buf[BUFSIZE];

	if (argc != 2)
		return 0;

	/* open the input file */
	if ((fdin = open (argv[1], O_RDONLY)) < 0)
		return 0;

	/* find size of input file */
	if (fstat (fdin,&statbuf) < 0)
		return 0;

	/* mmap the input file */
	if ((src = mmap (0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0))
			== (caddr_t) -1)
		return 0;
	/* read from the file */
	memcpy(buf,src, BUFSIZE-1);
	buf[BUFSIZE-1]='\0';
	printf("buf: %s",buf);
	munmap(src, statbuf.st_size);
	close(fdin);
}
