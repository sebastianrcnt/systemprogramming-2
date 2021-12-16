#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

// Tasklet Handler
void tasklet_func(unsigned long data);

DECLARE_TASKLET(my_tasklet,tasklet_func);

void tasklet_func(unsigned int data)
{
	tasklet_disable(&my_tasklet);
	printk("my_tasklet run: do what the tasklet want to do...\n");
	tasklet_enable(&my_tasklet);
}

static int __init hello_init(void)
{
	printk("HW2 MODULE INIT START\n");
	printk("Scheduling Tasklet my_tasklet\n");

	tasklet_schedule(&my_tasklet);
	printk("HW2 MODULE INIT END");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("HW2 EXITED\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Sebastian Jeong");
MODULE_DESCRIPTION("HW2 Module");
MODULE_LICENSE("GPL");
