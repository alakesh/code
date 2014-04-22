#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/utsname.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <net/tcp.h>
#include <linux/kbd_kern.h>
#include <linux/vt_kern.h>
#include <linux/types.h>

MODULE_DESCRIPTION("My kernel module");
MODULE_AUTHOR("Alakesh Haloi (alakesh.haloi@gmail.com)");
MODULE_LICENSE("GPL");

int scull_major=0;
int scull_minor=20;
int scull_nr_devs=4;

module_param(scull_major, int, 0);

static int chr_init_module(void)
{
	dev_t dev;
	int result;
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_nr_devs, "scull");
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
		scull_major = MAJOR(dev);
	}
	return 0;
}

static void chr_exit_module(void)
{
	unregister_chrdev_region(MKDEV(scull_major,scull_minor), scull_nr_devs);
	printk( KERN_DEBUG "chr module exit\n" );
}

module_init(chr_init_module);
module_exit(chr_exit_module);
