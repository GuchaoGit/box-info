#include <stdio.h>
#include <string.h>
#include "net_speed.h"
void get_net_data(net_data * net_data, char *eth)
{

	FILE *fp;
	char name[20], buff[1024];
	double rb, rp, re, rd, rfi, rfr, rc, rm,
	    tb, tp, te, td, tfi, tfr, tc, tm;

	double total;

	bzero(net_data, sizeof(net_data));
//      printf("eth = %s\n", eth);
	fp = fopen("/proc/net/dev", "r");
	while (fscanf
	       (fp,
		"%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
		name, &rb, &rp, &re, &rd, &rfi, &rfr, &rc, &rm, &tb, &tp,
		&te, &td, &tfi, &tfr, &tc, &tm) > 0) {

		if (strcmp(name, eth) == 0) {
//                      printf("find eth \n");
			net_data->rb = rb;
			net_data->rp = rp;
			net_data->tb = tb;
			net_data->tp = tp;
			break;
		}

	}
}
