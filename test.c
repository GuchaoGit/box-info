#include <stdio.h>
#include "box_info.h"
int main(int argc, char *argv[])
{
	char *cpu = getCPU();
	printf("main cpu = %s addr - %p\n", cpu, cpu);

	printf("device json string =%s\n", getDeviceJsonString("ens33"));

	MEMORY *memory = get_memory();
	printf("memory total = %g free = %g , memory rate = %g\n",
	       memory->total, memory->free, memory->used_rate);

	for (;;) {
/*
		printf("CPU usage: %.4f%%\n", getCPUUsageRate());
		sleep(3);

		MEMORY *memory = get_memory();
		printf("memory total = %g free = %g , memory rate = %g\n",
		       memory->total, memory->free, memory->used_rate);

		DISK *dev_mem = get_disk_stat();
		printf
		    ("hard disk mem total = %lfM,free = %lfM,used rate %.2lf%%\n",
		     dev_mem->total, dev_mem->free, dev_mem->used_rate);
*/
		sleep(2);
		get_net_speed("ens33");
	}



	return 0;
}
