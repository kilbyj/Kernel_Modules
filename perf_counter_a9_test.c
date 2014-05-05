#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define PERF_DEF_OPTS (1 | 16) //this is where you set what you want the performance counters to read

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jonathan Kilby");
MODULE_DESCRIPTION("performance counter driver");
static dev_t first;
static struct cdev c_dev;
static struct class *cl;

static ssize_t perf_read(struct file *f, char __user *buf, size_t len, loff_t *off){

	unsigned int cycles0,cycles1,cycles2;
	pringk(KERN_INFO "perf_counter: read()\n");

	asm("mrc p15, 0, %0, c15, c12, 1\t\n" : "=r" (cycles0));
	asm("mrc p15, 0, %0, c15, c12, 2\t\n" : "=r" (cycles1));
	asm("mrc p15, 0, %0, c15, c12, 3\t\n" : "=r" (cycles2));

	*((unsigned int *)buf) = cycles0;
	*((unsigned int *)buf+1) = cycles1;
	*((unsigned int *)buf+2) = cycles2;

	return 12;

}
static void enable_cpu_counters(void* data){

	 /* Enable user-mode access to counters. */
        asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));
        /* Program PMU and enable all counters */
        asm volatile("mcr p15, 0, %0, c9, c12, 0" :: "r"(PERF_DEF_OPTS));
        asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));

}

static void disable_cpu_counters(void *data){

	  /* Enable user-mode access to counters. */
        asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(0));
        /* Program PMU and enable all counters */
        //asm volatile("mcr p15, 0, %0, c9, c12, 0" :: "r"(PERF_DEF_OPTS));
        //asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));

}
static struct file_operations file_ops = {

	.owner = THIS_MODULE,
	.read = perf_read,

};

static int __init perf_init(void){

//	printk(KERN_INFO "perf_counter registered");

	if(alloc_chrdev_region(&first, 0, 1, "perf_counter") <0) return -1;

	if((cl = class_create(THIS_MODULE,"chardev"))==NULL){
		unregister_chrdev_region(first,1);
		return -1;
	}

	if(device_create(cl,NULL,first,NULL,"perf_counter") == NULL){
		class_destroy(cl);
		unregister_chrdev_region(first,1);
		return -1;
	}

	cdev_init(&c_dev,&file_ops);

	if(cdev_add(&c_dev,first,1)==-1){

		device_destroy(cl,first);
		class_destroy(cl);
		unregister_chrdev_region(first,1);

	}

	on_each_cpu(enable_cpu_counters, NULL, 1);
	printk(KERN_INFO "perf_counter  registered");

	return 0;
}




static void __exit perf_exit(void){

	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first,1);
	on_each_cpu(disable_cpu_counters), NULL, 1);
	printk(KERN_INFO "perf_counter destroyed");

}

module_init(perf_init);
module_exit(perf_exit);
