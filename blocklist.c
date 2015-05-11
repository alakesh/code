/* Test program to check file system blocks used by a file
 * <Alakesh.Haloi@gmail.com>
 **/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]){
	struct stat st;
	int i=0, block=0, bcount=0, blocksize=0;

	if (argc != 2){
		printf("Usage: %s  <filename> \n", argv[1]);
		return 0;
	}

	int fd = open(argv[1], O_RDONLY);

	ioctl(fd, FIGETBSZ, &blocksize);
	
	if(stat(argv[1], &st)){
		printf("failed to stat file %s \n", argv[1]);
	}

	bcount = (st.st_size + blocksize - 1) / blocksize;
	for (i=0; i<bcount; i++){
		block = i;
		if (ioctl(fd, FIBMAP, &block)){
			printf("IOCTL failed \n");
			return 0;
		}
		printf("Block: %d \n", block);
	}
	return 0;
}


