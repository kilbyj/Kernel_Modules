#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define N               16*1024*1024
#define CLOCK_RATE      700.0e6

main(){
    unsigned int perf_counts[6];
    unsigned int cycles,us;
    float time,mbps;
    struct timeval time1,time2;
    static unsigned int a[N],b[N];
    
    FILE *myFile=fopen("/dev/perf_counter","rb");
    if (!myFile) {
        perror("opening performance counter");
        
        return 0; 
    }
    
    gettimeofday(&time1,0);
    fread ((void *)perf_counts,4,3,myFile);
    memcpy(b,a,N*sizeof(unsigned int));
    fread (((void *)perf_counts)+12,4,3,myFile);
    gettimeofday(&time2,0);
    
    cycles = perf_counts[3]-perf_counts[0];
    
    time = (float) (cycles/CLOCK_RATE);
    
    mbps = (float) ((N*2*sizeof(unsigned int)))/time/(1024.0*1024.0);
    
    us = time2.tv_sec*1000000 + time2.tv_usec - time1.tv_sec*1000000 - time1.tv_usec;
    
    printf("memory test\n-----------\n");
    printf("[perf_counter] cycles = %u (%0.0f us, %0.0f MB/s)\n" cycles,time*1.0e6,mbps);
    printf("[system_clock] time = %u us\n\n",us);
    fclose (myFile);
}
