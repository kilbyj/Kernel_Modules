#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define PERF_DEF_OPTS (1 | 16) //this is where you set what you want the performance counters to read

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jonathan Kilby");
MODULE_DESCRIPTION("performance counter driver");
static dev_t first;

static void enable_cpu_counters(void* data){

	 /* Enable user-mode access to counters. */
        asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));
        /* Program PMU and enable all counters */
        asm volatile("mcr p15, 0, %0, c9, c12, 0" :: "r"(PERF_DEF_OPTS));
        asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));

}

static void disable_cpu_counters(void *data){

	  /* Disable user-mode access to counters. */
        asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(0));
}

static int __init perf_init(void){

	on_each_cpu(enable_cpu_counters, NULL, 1);
	printk(KERN_INFO "perf_counter  registered");

	return 0;
}




static void __exit perf_exit(void){

	on_each_cpu(disable_cpu_counters, NULL, 1);
	printk(KERN_INFO "perf_counter destroyed");

}

module_init(perf_init);
module_exit(perf_exit);
