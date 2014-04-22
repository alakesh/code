#include <stdio.h>
#include <sys/types.h>
#include <sys/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int write_mode = 0;
int level = 3;
uint16_t port = 0;
uint32_t val = 0;

void help(){
	printf("Usage: For write op [ioportb|ioportw|ioportl] [reg] [val]\n"); 
	printf("Usage: For read op [ioportb|ioportw|ioportl] [reg] \n"); 
}

int main(int argc, char *argv[]){

	int mode = 0;

	if (argc == 1)
	{
		help();
		return 0;
	}

	if (strstr(argv[0],"ioportb")){
		mode = 1;
		printf("Byte mode read/write \n");
	} else if (strstr(argv[0], "ioportw")){
		mode = 2;
		printf("Word mode read/write \n");
	} else if (strstr(argv[0], "ioportl")){
		mode =3;
		printf("Long mode read/write \n");
	}

	if (argc > 2){
		write_mode = 1;
	} 

	port = strtoul(argv[1],0,16);

	if(write_mode)
	{
		val = strtoul(argv[2],0,16);
	}

	ioperm(port,1,1);
	if (write_mode){
		if(mode==1)
			outb(val, port);
		else if(mode==2)
			outw(val, port);
		else if(mode==3)
			outl(val, port);
		printf("Wrote 0x%x to port 0x%x\n", val, port);
	} else {
		if(mode==1)
			val = inb(port);
		else if(mode==2)
			val = inw(port);
		else if(mode==3)
			val = inl(port);

		printf("Read 0x%x from port 0x%x\n", val, port);
	}
	return 0;
}
