#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

#define EV_PRESSED 1
#define EV_RELEASED 0


int main(void){
	int fd = 0;
	char *device = "/dev/input/event0";

	if ((fd = open(device, O_RDWR))>0)
	{
		struct input_event event;
		event.type = EV_KEY;
		event.value = EV_PRESSED;
		event.code = KEY_B;
		write(fd, &event, sizeof(struct input_event));

		event.value = EV_RELEASED;
		event.code = KEY_B;
		write(fd, &event, sizeof(struct input_event));

		close(fd);
	}

	/* read it back */

	int fd1 = 0;
	if ((fd1 = open(device, O_RDONLY))>0)
	{
		struct input_event event;
		unsigned int scan_code = 0;

		if(event.type != EV_KEY)
		{
			return 0;
		}

		if(event.value == EV_RELEASED)
		{
			scan_code = event.code;
			printf("%d\n", scan_code);
		}
		close(fd1);
	}
}


