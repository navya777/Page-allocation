#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL");
unsigned int objnum = 2000;
module_param(objnum, int, 0);
struct P
{
	unsigned int arr[8];
};
static struct timeval start;
static struct timeval end;
static int simple_init (void) {
	int i;
	struct P * mem;
	do_gettimeofday(&start);
	for(i=0; i < objnum; i++)
	{
		mem = (struct P*)kmalloc( sizeof(struct P), GFP_KERNEL);
		kfree(mem);
	}
	do_gettimeofday(&end);
	printk(KERN_INFO "Difference in timings: %lu", (end.tv_usec - start.tv_usec));
    	return 0;
}
	static void simple_exit (void) {
	printk(KERN_INFO "kmem_alloc Module being unloaded");
}
module_init (simple_init);
module_exit (simple_exit);
