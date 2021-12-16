#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched/signal.h>

// DATA
#define STUDENT_NAME "Shiwon Jeong"
#define STUDENT_ID "2018199061"

// Prototypes
void print_bar(void);

// Tasklet Handler
void tasklet_func(unsigned long data);

// Utils

void print_bar()
{
    printk("************************************\n");
}

// Declare Tasklet
DECLARE_TASKLET_OLD(my_tasklet, tasklet_func);

void tasklet_func(unsigned long data)
{
	// get all tasks
	struct task_struct *task_iter;
    struct task_struct *target_task;
	for_each_process(task_iter) {
        if (task_iter->state == TASK_RUNNING) {
            target_task = task_iter;
            break;
        }
	}


    int is_kernel_thread = 0;
    if (target_task->mm == NULL) {
        is_kernel_thread = 1;
    }

    

    printk("Student ID: %s, Name: %s\n", STUDENT_ID, STUDENT_NAME);
    printk("Virtual Memory Address Information\n");
    printk("Process Name: %s\n", target_task->comm);
    printk("Process id: %d\n", target_task->pid);

    print_bar();

    if (is_kernel_thread) {
        printk("This is kernel thread!\n");
        print_bar();
        return;
    }
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

MODULE_AUTHOR("Shiwon Jeong");
MODULE_DESCRIPTION("HW2 Module");
MODULE_LICENSE("GPL");
