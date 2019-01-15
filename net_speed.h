#ifndef __NETSPEED_H__
#define __NETSPEED_H__

#define WAIT 1

typedef struct net_data {
	double rp, rb;
	double tp, tb;

} net_data;


void get_net_data(net_data * net_data, char *eth);

#endif
