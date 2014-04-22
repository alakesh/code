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

	if ((fdin = open (argv[1], O_RDWR)) < 0)
		return 0;

	if (fstat (fdin,&statbuf) < 0)
		return 0;
	lseek(fdin, 1024*1024, SEEK_SET);

	//if ((src = mmap (0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fdin, 0))
	//		== (caddr_t) -1)
	if ((src = mmap (0, 1024*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fdin, 0))
			== (caddr_t) -1)
		return 0;
	memcpy(src+1024*1000, "WR", 2);
	munmap(src, statbuf.st_size);
	close(fdin);
}
