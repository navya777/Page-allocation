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

static int page_alloc (void* none)
{        
	
	printk(KERN_INFO "Value of objnum is  %u",objnum);
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