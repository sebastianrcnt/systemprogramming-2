#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/sched/signal.h>
#include <linux/pgtable.h>

// DATA
#define STUDENT_NAME "Shiwon Jeong"
#define STUDENT_ID "2018199061"

// Prototypes
void print_bar(void);


// Tasklet Handler
void tasklet_func(unsigned long data);
int task_is_kernel_thread(struct task_struct* task);
int task_is_running_state(struct task_struct* task);

// Utils

void print_bar()
{
    printk("************************************\n");
}

// Task Methods
int task_is_kernel_thread(struct task_struct* task)
{
    int is_kernel_thread = 0;
    if (task->mm == NULL) {
        is_kernel_thread = 1;
    }

    return is_kernel_thread;
}

int task_is_running_state(struct task_struct* task)
{
    int is_running_state = 0;
    if (task->state == TASK_RUNNING) {
        is_running_state = 1;
    }
    return is_running_state;
}

// Declare Tasklet
DECLARE_TASKLET_OLD(my_tasklet, tasklet_func);


void tasklet_func(unsigned long data)
{
	// get all tasks
	struct task_struct *task_iter;
    struct task_struct *target_task;
	for_each_process(task_iter) {
        int is_kernel_thread = task_is_kernel_thread(task_iter);
        int is_running_state  = task_is_running_state(task_iter);
        if (!is_kernel_thread && is_running_state) {
            target_task = task_iter;
            break;
        }
	}



    

    printk("Student ID: %s, Name: %s\n", STUDENT_ID, STUDENT_NAME);
    printk("Virtual Memory Address Information\n");
    printk("Process Name: %s\n", target_task->comm);
    printk("Process id: %d\n", target_task->pid);

    print_bar();

    int is_kernel_thread = task_is_kernel_thread(target_task);
    if (is_kernel_thread) {
        printk("This is kernel thread!\n");
        print_bar();
        return;
    }

    // Print info about code area
    unsigned long code_area_start = target_task->mm->start_code;
    unsigned long code_area_end = target_task->mm->end_code;
    unsigned long code_area_size = code_area_end - code_area_start;
    unsigned long code_area_size_in_pages = code_area_size / 4096;

    printk("0x%08lx - 0x%08lx : Code Area, %lu page(s)\n",
            code_area_start,
            code_area_end,
            code_area_size_in_pages);


    // Print info about data area
    unsigned long data_area_start = target_task->mm->start_data;
    unsigned long data_area_end = target_task->mm->end_data;
    unsigned long data_area_size = data_area_end - data_area_start;
    unsigned long data_area_size_in_pages = data_area_size / 4096;

    printk("0x%08lx - 0x%08lx : Data Area, %lu page(s)\n",
            data_area_start,
            data_area_end,
            data_area_size_in_pages);


    // Print info about bss area
    unsigned long bss_area_start = target_task->mm->end_data;
    unsigned long bss_area_end = target_task->mm->start_brk;
    unsigned long bss_area_size = bss_area_end - bss_area_start;
    unsigned long bss_area_size_in_pages = bss_area_size / 4096;

    printk("0x%08lx - 0x%08lx : BSS Area, %lu page(s)\n",
            bss_area_start,
            bss_area_end,
            bss_area_size_in_pages);

    // Print info about heap area
    unsigned long heap_area_start = target_task->mm->start_brk;
    unsigned long heap_area_end = target_task->mm->brk;
    unsigned long heap_area_size = heap_area_end - heap_area_start;
    unsigned long heap_area_size_in_pages = heap_area_size / 4096;

    printk("0x%08lx - 0x%08lx : Heap Area, %lu page(s)\n",
            heap_area_start,
            heap_area_end,
            heap_area_size_in_pages);

    // Print info about shared libraries
    // find shared libraries
    struct vm_area_struct* ptr;
    struct vm_area_struct* init_ptr;

    ptr = target_task->mm->mmap;
    init_ptr = target_task->mm->mmap;

    int count = 0;
    int shared_count = 0;
    unsigned long max_shared_mem = 0;
    unsigned long min_shared_mem = 0;

    while ((ptr->vm_next != NULL)) {
        count++;
        ptr = ptr->vm_next;

        if (ptr->vm_private_data != NULL) {
            shared_count++;
            
            /* printk("%lx ~ %lx", ptr->vm_start, ptr->vm_end); */

            if (ptr->vm_end > max_shared_mem) {
                max_shared_mem = ptr->vm_end;
            }

            if (min_shared_mem == 0) {
                min_shared_mem = ptr->vm_start;
            } else {
                if (ptr->vm_start < min_shared_mem) {
                    min_shared_mem = ptr->vm_start;
                }
            }
        }
    }


/*     printk("mmap count: %d\n", count); */
/*     printk("shared count: %d\n", shared_count); */
/*     printk("max_shared_mem: %lx\n", max_shared_mem); */
/*     printk("min_shared_mem: %lx\n", min_shared_mem); */

    unsigned long shared_area_start = min_shared_mem;
    unsigned long shared_area_end = max_shared_mem;
    unsigned long shared_area_size = shared_area_end - shared_area_start;
    unsigned long shared_area_size_in_pages = shared_area_size / 4096;
    

    printk("0x%08lx - 0x%08lx : Shared Area, %lu page(s)\n",
            shared_area_start,
            shared_area_end,
            shared_area_size_in_pages);

    
    // Print info about stack area
    unsigned long stack_area_size = target_task->mm->stack_vm * 4096;
    unsigned long stack_area_start = target_task->mm->start_stack - stack_area_size;
    unsigned long stack_area_end = target_task->mm->start_stack;
    unsigned long stack_area_size_in_pages = target_task->mm->stack_vm;

    printk("0x%08lx - 0x%08lx : Stack Area, %lu page(s)\n",
            stack_area_start,
            stack_area_end,
            stack_area_size_in_pages);


    // Start Paging

    pgd_t *base_pgd;
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *ptep, pte;
    unsigned long code_vm = code_area_start;

    struct mm_struct *task_mm = target_task->mm;

    // 1 level paging: PGD INFO
    base_pgd = task_mm->pgd;
    pgd = pgd_offset(task_mm, code_vm);
    p4d = p4d_offset(pgd, code_vm);
    pud = pud_offset(p4d, code_vm);
    pmd = pmd_offset(pud, code_vm);
    ptep = pte_offset_kernel(pmd, code_vm);
    pte = *ptep;


    print_bar();
    printk("Page Directory Entry Information\n");
    print_bar();
    printk("PGD     Base Address        : 0x%08lx\n", (unsigned long) base_pgd);
    printk("code    PGD Address         : 0x%08lx\n", (unsigned long) pgd);
    printk("        PGD Value           : 0x%08lx\n", (unsigned long) pgd_val(*pgd));
    // TODO - add pfn address, page size
    //



    // 2 level paging
    
    print_bar();
    printk("Page 4 Directory Entry Information\n");
    print_bar();
    printk("code    P4D Address         : 0x%08lx\n", (unsigned long) p4d);
    printk("        P4D Value           : 0x%08lx\n", (unsigned long) p4d_val(*p4d));

    
    // pud
    
    print_bar();
    printk("Page Upper Directory Entry Information\n");
    print_bar();
    printk("code    PUD Address         : 0x%08lx\n", (unsigned long) pud);
    printk("        PUD Value           : 0x%08lx\n", (unsigned long) pud_val(*pud));

    // pmd
    
    print_bar();
    printk("Page Middle Directory Entry Information\n");
    print_bar();
    printk("code    PMD Address         : 0x%08lx\n", (unsigned long) pmd);
    printk("        PMD Value           : 0x%08lx\n", (unsigned long) pmd_val(*pmd));
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
