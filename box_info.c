#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <unistd.h>
#include <time.h>
#include "box_info.h"
#include "net_speed.h"

#define BUFFER 1024
#define SECOND 1

//1:true 0:false -1:error
int is_start_with(const char *str1, char *str2)
{
	if (str1 == NULL || str2 == NULL)
		return -1;
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if ((len1 < len2) || (len1 == 0 || len2 == 0))
		return -1;
	char *p = str2;
	int i = 0;
	while (*p != '\0') {
		if (*p != str1[i])
			return 0;
		p++;
		i++;
	}
	return 1;

}

char *getCPU()
{
	char *cpu = "MT7628";
	printf("*getCPU() return %s  addr = %p\n", cpu, cpu);
	return cpu;
}

unsigned long getCPUTotalTime(CPUInfo * pstCPUInfo)
{
	return pstCPUInfo->user + pstCPUInfo->nice + pstCPUInfo->system +
	    pstCPUInfo->idle + pstCPUInfo->iowait + pstCPUInfo->irq +
	    pstCPUInfo->softirq;
}


void displayCPUInfo(CPUInfo * pstCPUInfo)
{
	printf("%s %lu %lu %lu %lu %lu %lu %lu\n", pstCPUInfo->name,
	       pstCPUInfo->user, pstCPUInfo->nice, pstCPUInfo->system,
	       pstCPUInfo->idle, pstCPUInfo->iowait, pstCPUInfo->irq,
	       pstCPUInfo->softirq);
}


int getCPUInfo(CPUInfo * pstCPUInfo)
{
	FILE *fp = fopen("/proc/stat", "r");

	char buf[1024];
	fgets(buf, sizeof(buf), fp);

	sscanf(buf, "%s %lu %lu %lu %lu %lu %lu %lu",
	       pstCPUInfo->name, &pstCPUInfo->user, &pstCPUInfo->nice,
	       &pstCPUInfo->system, &pstCPUInfo->idle, &pstCPUInfo->iowait,
	       &pstCPUInfo->irq, &pstCPUInfo->softirq);

	fclose(fp);

	return 0;
}

float getCPUUsageRate()
{
	CPUInfo stCPUInfo1;
	CPUInfo stCPUInfo2;

	memset(&stCPUInfo1, 0, sizeof(stCPUInfo1));
	memset(&stCPUInfo2, 0, sizeof(stCPUInfo2));

	getCPUInfo(&stCPUInfo1);
	displayCPUInfo(&stCPUInfo1);
	sleep(1);
	getCPUInfo(&stCPUInfo2);
	displayCPUInfo(&stCPUInfo2);

	unsigned long nTotalTime =
	    getCPUTotalTime(&stCPUInfo2) - getCPUTotalTime(&stCPUInfo1);
	unsigned long nIdleTime = stCPUInfo2.idle - stCPUInfo1.idle;
	printf("total time: %lu\n", nTotalTime);
	printf("idle time:  %lu\n", nIdleTime);
	if (nTotalTime == 0) {
		return 0;
	}

	float usage = (nTotalTime - nIdleTime) * (float) 100 / nTotalTime;
/*
	float usage =
	    (float) 100 * (stCPUInfo1.user + stCPUInfo1.nice +
			   stCPUInfo1.system) / (stCPUInfo1.user +
						 stCPUInfo1.nice +
						 stCPUInfo1.system +						 stCPUInfo1.idle);
						 */
	return usage;
}

MEMORY *get_memory()
{

	FILE *fp;
	char name[20], name2[20];
	int n;
	char buff[256];
	unsigned long total, free1;
	double mem_total, free, mem_used_rate;
	MEMORY *dev = (MEMORY *) malloc(sizeof(MEMORY));
	fp = fopen("/proc/meminfo", "r");
	fgets(buff, sizeof(buff), fp);
	sscanf(buff, "%s %lu %s\n", name, &total, name2);
	mem_total = (double) total;
	fgets(buff, sizeof(buff), fp);
	sscanf(buff, "%s %lu %s\n", name, &free1, name2);
//      fgets(buff, sizeof(buff), fp);
//      sscanf(buff, "%s %lu %s\n", name, &free1, name2);
	free = (double) free1;
	mem_used_rate = (1 - free / mem_total) * 100;
	dev->total = mem_total;
	dev->free = free;
	dev->used_rate = mem_used_rate;
	fclose(fp);

	return dev;
}

DISK *get_disk_stat()		// get hard disk message
{
	FILE *fp;
	int h = 0;
	char buffer[80], a[80], d[80], e[80], f[80], buf[1024];
	double c, b;
	fp = popen("df", "r");
	fgets(buf, sizeof(buf), fp);
	double dev_total = 0, dev_used = 0;
	DISK *dev = (DISK *) malloc(sizeof(DISK));
	while (fscanf(fp, "%s %lf %lf %s %s %s", a, &b, &c, d, e, f) > 0) {
		if (is_start_with(a, "/dev/sda") == 1)
			dev_total += b;
		dev_used += c;
	}
	dev->total = dev_total / 1024;
	dev->free = (dev_total - dev_used) / 1024;
	dev->used_rate = dev_used / dev_total * 100;
	pclose(fp);
	return dev;
}

char *get_net_speed(char *eth)
{

	char ethc[] = "";
	strcat(ethc, eth);
	strcat(ethc, ":");
	printf("eth = %s\n", ethc);
	net_data nd1, nd2;

	get_net_data(&nd1, ethc);
	sleep(WAIT);
	get_net_data(&nd2, ethc);
	double ns = ((nd2.rb + nd2.tb) - (nd1.rb + nd1.tb)) / 1024;

	char *pnetspeed;
	char netspeed[15];
	sprintf(netspeed, "%.2f kB/s", ns);
	pnetspeed = malloc(strlen(netspeed));
	strcpy(pnetspeed, netspeed);

	printf("get net speed = %s %s\n", ethc, pnetspeed);
	return pnetspeed;
}

const char *getDeviceJsonString(char *eth)
{
	struct json_object *root = json_object_new_object();
	json_object_object_add(root, "devid",
			       json_object_new_string("dev001"));
	json_object_object_add(root, "devtype",
			       json_object_new_string("1"));

	struct json_object *params = json_object_new_object();

	//params
	json_object_object_add(params, "cpu",
			       json_object_new_string("MT7628"));
	//memory
	MEMORY *memory = get_memory();
	json_object_object_add(params, "mem",
			       json_object_new_double(memory->total -
						      memory->free));
	free(memory);
	//disk
	DISK *dev_disk = get_disk_stat();
	json_object_object_add(params, "freedisk", json_object_new_double(dev_disk->free));	//Mb
	free(dev_disk);
	//battery
	json_object_object_add(params, "battery",
			       json_object_new_double(100));
	//GPS (longitude,latitude)
	json_object_object_add(params, "GPS",
			       json_object_new_string
			       ("113.729159,34.77061"));
	//nettype
	json_object_object_add(params, "nettype", json_object_new_string("3"));	// 0:unknow 1:wireline 2:wifi 3:4G
	//netspeed kb/s
	char *pnp = get_net_speed(eth);
	json_object_object_add(params, "netspeed", json_object_new_string(pnp));	//kb
	free(pnp);

	time_t t;
	time(&t);
	json_object_object_add(params, "runtime", json_object_new_double(t * 1000));	//unix time stamp

	json_object_object_add(root, "parmas", params);

	return json_object_to_json_string(root);
}
