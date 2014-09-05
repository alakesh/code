#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	char buf[1024];
	fp = popen("ls -al", "r");
	if (!fp)
		return -1;
	while(fgets(buf, 1024, fp)){
		printf("%s", buf);
	}
}
