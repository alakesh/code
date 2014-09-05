#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int filedes[2];
	char buf[20];
	pipe(filedes);
	pid_t pid = fork();
	if (pid == 0){
		/* child closes read */
		dup2(1, filedes[1]);
		execlp("sort", "sort", NULL);
	} 
	else {
		close(filedes[1]);
		while(read(filedes[0], buf, sizeof(buf)))
				printf("%s",buf);
	}
}
