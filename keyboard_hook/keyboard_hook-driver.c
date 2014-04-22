
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/utsname.h>
#include <net/tcp.h>
#include <linux/kbd_kern.h>
#include <linux/vt_kern.h>

MODULE_DESCRIPTION("My kernel module");
MODULE_AUTHOR("Alakesh Haloi (alakesh.haloi@gmail.com)");
MODULE_LICENSE("GPL");

void **sys_call_table;


static int keyboard_hook_init_module(void)
{
	char sysname[70];
	sysname[70]='\0';
	printk( KERN_DEBUG "Linux Banner String\n" );
	memcpy(sysname, init_uts_ns.name.sysname,65);
	printk( KERN_ERR "UTS_SYSNAME %s \n", sysname);
	printk( KERN_ERR "TCP MEMORY ALLOCATED %ld \n", tcp_memory_allocated);
	return 0;
}

static void keyboard_hook_exit_module(void)
{
	printk( KERN_DEBUG "Module keyboard_hook exit\n" );
}

module_init(keyboard_hook_init_module);
module_exit(keyboard_hook_exit_module);
