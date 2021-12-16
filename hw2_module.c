#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

// Tasklet Handler
void tasklet_func(unsigned long data);

DECLARE_TASKLET_OLD(my_tasklet, tasklet_func);

void tasklet_func(unsigned long data)
{
	printk("my_tasklet run: do what the tasklet want to do...\n");
}

static int __init hw2_init(void)
{
	printk("HW2 MODULE INIT START\n");
	printk("Scheduling Tasklet my_tasklet\n");
	tasklet_schedule(&my_tasklet);
	printk("HW2 MODULE INIT END");
	return 0;
}

static void __exit hw2_exit(void)
{
	tasklet_kill(&my_tasklet);
	printk("HW2 EXITED\n");
}

module_init(hw2_init);
module_exit(hw2_exit);

MODULE_AUTHOR("Sebastian Jeong");
MODULE_DESCRIPTION("HW2 Module");
MODULE_LICENSE("GPL");
