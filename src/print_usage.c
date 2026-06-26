#include "ft_traceroute.h"

void    print_usage(void)
{
    printf("Usage: ft_traceroute [OPTION...] HOST\n");
    printf("Trace the route to a network host.\n");
    printf("\n");
    printf("  -v                 verbose output\n");
    printf("  -i, --iface=IFACE  set network interface\n");
    printf("  -m, --max_ttl=TTL  set max time-to-live (default: 30)\n");
    printf("  -p, --port=PORT    set destination port (default: 33434)\n");
    printf("  -s, --src_addr=ADDR  set source address\n");
    printf("  -q, --n_queries=N  set number of queries to send (default: 3)\n");
    printf("  -N, --s_queries=N  set number of queries to send per hop (default: 3)\n");
    printf("  -t, --tos=TOS       set type of service (default: 0)\n");
    printf("  -l, --flow_label=LABEL  set flow label (default: 0)\n");
    printf("\n");
    printf("  -?, --help         give this help list\n");
    printf("\n");
    printf("Report bugs to <ttrossea@42.fr>.\n");
}
