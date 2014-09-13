#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <fcntl.h>
#include <linux/fb.h>

int main() {
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

	int fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd){
		printf("error opening framebuffer device");
		return 0;
	}

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		return 0;
	}

	printf("start of mem: 0x%x \n", (uint32_t)finfo.smem_start);
	printf("Size of mem: %d \n", finfo.smem_len);
	printf("Line length: %d \n", finfo.line_length);
	printf("mmio_start: 0x%x \n", (uint32_t)finfo.mmio_start);
	

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		return 0;
	}

	printf("xres:  %d \n", vinfo.xres);
	printf("yres:  %d \n", vinfo.yres);


	close(fbfd);
	return 0;
}
