#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static inline uint32_t read_cycles(void){
#if defined(__GNUC__) && defined(__ARM_ARCH_7A__)
	uint32_t r = 0;
	asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r));
	return r;
#else
#error Unsupported architecture/compiler!
#endif
}

static void reset_event_counts(void){
	uint32_t r = 0;
	asm volatile("mrc p15, 0, %0, c9, c12, 0": "=r"(r));
	r |= 0x02;
 	asm volatile("mcr p15, 0, %0, c9, c12, 0": : "r"(r));

}

static void setup_event_registers(void){

//set up counter 1 for use with instruction events
asm volatile("mcr p15, 0, %0, c9, c12, 5" :: "r"(0));
asm volatile("mcr p15, 0, %0, c9, c13, 1" :: "r"(0x61)); //Stall Cycles casue by waiting for data cache

asm volatile("mcr p15, 0, %0, c9, c12, 5" :: "r"(1));
asm volatile("mcr p15, 0, %0, c9, c13, 1" :: "r"(0x68)); //number of speculative instructions executed

asm volatile("mcr p15, 0, %0, c9, c12, 5" :: "r"(2));
asm volatile("mcr p15, 0, %0, c9, c13, 1" :: "r"(0x69)); //data linefill from the external AXI bus

asm volatile("mcr p15, 0, %0, c9, c12, 5" :: "r"(3));
asm volatile("mcr p15, 0, %0, c9, c13, 1" :: "r"(0x70)); //instructions executed in main, multiply, and arithmetic logic pipeline

asm volatile("mcr p15, 0, %0, c9, c12, 5" :: "r"(4));
asm volatile("mcr p15, 0, %0, c9, c13, 1" :: "r"(0x71)); //instructions executes in the second execution pipeline

asm volatile("mcr p15, 0, %0, c9, c12, 5" :: "r"(5));
asm volatile("mcr p15, 0, %0, c9, c13, 1" :: "r"(0x72)); //load/store instructions executed

//enable counters
asm volatile("mcr p15, 0, %0, c9, c12, 0" :: "r"(1));
//asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"(1));
}

static inline uint32_t read_event(int reg){
#if defined(__GNUC__) && defined(__ARM_ARCH_7A__)
	uint32_t inst = 0;
	asm volatile("mcr p15, 0, %0, c9, c12, 5" :: "r"(reg));
	asm volatile("mrc p15,  0, %0, c9, c13, 2" : "=r"(inst));
	return inst;
#else
#error Unsupported architecture/compiler!
#endif
}



int main(void){
	
	uint32_t start_time = 0;
	uint32_t end_time = 0;
	uint32_t read1,read2,overhead;
	read1 = read_cycles();
	read2 = read_cycles();
	overhead = (read2-read1);
	setup_event_registers();
	//start_time = read_cycles();
	int i=0;
	float x = 1.5, y = .5, z = 0;
/*	while(1){
		i++;
		if(i > 10000)break;
	}*/
	//end_time = read_cycles();
	//printf("overhead = %u\n", overhead);
	//printf("cycle delta 1 = %u\n", end_time - start_time);
	
//	z = (x*y)+((z+1)*x)+((z+1)*y);
//	z = (z-x)+(z-y)/y;
	reset_event_counts();
	for(i=0;i<100;i++){
		//do nothing
}
	printf("speculative instructions 1 = %u\n", read_event(1));
	reset_event_counts();
	for(i=0;i<1000;i++){
		//do nothing
}
	printf("speculative instructions 2 = %u\n", read_event(1));

	int a[10]={0,1,2,3,4,5,6,7,8,9};
	int b[10];
	int c[100],d[100];

	for(i=0;i<100;i++)
		c[i] = i;
	printf("memcopy a[10] to b[10] = ");
	reset_event_counts();
	memcpy(&b, &a, sizeof(a));
	printf("%u\n", read_event(0));
	printf("memcopy c[100] to d[100] = ");
        reset_event_counts();
        memcpy(&d, &c, sizeof(c));
        printf("%u\n", read_event(0));

	
/*
	printf("num stall cycles caused by waiting for the data cache= %u\n", read_event(0));
	printf("speculative instructions = %u\n", read_event(1));
	printf("data linefill from ext AXI = %u\n", read_event(2));
	printf("Inst exec from main, mult, and arithmetic logic pipeline =  %u\n",  read_event(3));
	printf("Inst exec in the second exec pipeline = %u\n",  read_event(4));
	printf("Load/Store inst exec = %u \n",  read_event(5));
*/
	return 0;
}

