#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static inline uint32_t read_cycles(void){
#if defined(__GNUC__) && defined(__ARM_ARCH_7A__)
	uint32_t r = 0;
	asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r));
	return r;
#else
#error Unsopported architecture/compiler!
#endif
}
static inline uint32_t read_instructions(void){
#if defined(__GNUC__) && defined(__ARM_ARCH_7A__)
        uint32_t r = 0;
        asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(r));
        return r;
#else
#error Unsopported architecture/compiler!
#endif
}



int main(void){

	uint32_t start_time = 0;
	uint32_t end_time = 0;
	uint32_t read1,read2,overhead;
	read1 = read_cycles();
	read2 = read_cycles();
	overhead = (read2-read1)/2;
	start_time = read_cycles();
	int i=0;
	while(1){
		i++;
		if(i > 100)break;
	}
	end_time = read_cycles();
	printf("overhead = %u\n", overhead);
	printf("cycle delta = %u\n", end_time - start_time);
	return 0;
}
