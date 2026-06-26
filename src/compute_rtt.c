#include "ft_traceroute.h"

double compute_rtt(struct timeval *sent)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (double)(now.tv_sec  - sent->tv_sec)  * 1000.0
         + (double)(now.tv_usec - sent->tv_usec) / 1000.0;
}