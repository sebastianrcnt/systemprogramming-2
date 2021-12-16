#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int __init hello_init(void)
{
	printk("hello, world!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR();
MODULE_DESCRIPTION();
MODULE_LICENSE();
