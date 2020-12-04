#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <asm/page.h>
#include <asm/cache.h>
MODULE_LICENSE("GPL");
unsigned int objnum = 2000;
module_param(objnum, int, 0);
struct P
{
	unsigned int arr[8];
};
static struct timeval start;
static struct timeval end;
static struct task_struct *thread0;
static struct kmem_cache* memory_slab;
static void ctor(void* param)
{}
static int page_alloc (void* none)
{
	int i;
	do_gettimeofday(&start);
	memory_slab = (struct kmem_cache*)kmem_cache_create("cache_slab", sizeof(objnum)*objnum, 0, SLAB_HWCACHE_ALIGN, ctor);
	for(i=0; i < objnum; i++)
	{
		kmem_cache_free(memory_slab,kmem_cache_alloc(memory_slab, GFP_KERNEL));
	}
	do_gettimeofday(&end);
	printk(KERN_INFO "Difference in timings: %lu",(end.tv_usec - start.tv_usec));
	return 0;
}
static int simple_init (void) {
	thread0=kthread_create(page_alloc,NULL, "thread0");
	kthread_bind(thread0, 0);
	wake_up_process(thread0);
    	return 0;
}
static void simple_exit (void) {
	kmem_cache_destroy(memory_slab);
	printk(KERN_INFO "cache_alloc Module being unloaded");
}
module_init (simple_init);
module_exit (simple_exit);