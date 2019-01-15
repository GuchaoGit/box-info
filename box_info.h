#ifndef BOX_INFO_H
#define BOX_INFO_H

/**判断str1是否以str2开头
 * 如果是返回1
 * 不是返回0
 * 出错返回-1
 */
extern int is_start_with(const char *str1, char *str2);


typedef struct cpu_info {
	char name[8];
	unsigned long user;
	unsigned long nice;
	unsigned long system;
	unsigned long idle;
	unsigned long iowait;
	unsigned long irq;
	unsigned long softirq;
} CPUInfo;

extern char *getCPU();
extern float getCPUUsageRate();

typedef struct device_memory {
	double total, free, used_rate;
} MEMORY;
MEMORY *get_memory(void);


typedef struct DISK		//定义一个disk 结构体
{
	double total, free, used_rate;
} DISK;

DISK *get_disk_stat(void);

extern const char *getDeviceJsonString(char *eth);

#endif
