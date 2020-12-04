#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/gfp.h>
MODULE_LICENSE("GPL");
unsigned int objnum = 2000;
module_param(objnum, int, 0);
struct P
{
	unsigned int arr[8];
};
static struct timeval start;
static struct timeval end;
int order;
static struct page *page;
static int page_alloc (void* none)
{        
	int size;
	struct P p1;
 	size=sizeof(p1);

	printk(KERN_INFO "Value of objnum is  %u",objnum);
        printk(KERN_INFO "Size of struct is %d bytes",size);
	do_gettimeofday(&start);
	order = (int)ilog2(1+(objnum/128));
	printk(KERN_INFO "Two power pages allocation:  %d",order);
	page = alloc_pages(GFP_KERNEL, order);
	__free_pages(page, order);
	do_gettimeofday(&end);
	printk(KERN_INFO "Difference in timings: %lu", (end.tv_usec - start.tv_usec));
	return 0;
}
static struct task_struct *thread;
static int simple_init (void) {
	
	thread=kthread_create(page_alloc,NULL, "thread");
	kthread_bind(thread, 0);
	wake_up_process(thread);
 	return 0;
}

static void simple_exit (void) {
	printk(KERN_INFO "Page alloc module unloaded");
}
module_init (simple_init);
module_exit (simple_exit);