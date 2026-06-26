#include "ft_traceroute.h"

int parse_reply(uint8_t *buf, uint16_t dst_port_sent, int hops)
{
    t_ip   *ip   = (t_ip *)buf;
    t_icmp *icmp = (t_icmp *)(buf + ip->ihl * 4);
    t_ip   *ip2  = (t_ip *)((uint8_t *)icmp + sizeof(t_icmp));
    t_udp  *udp  = (t_udp *)((uint8_t *)ip2 + ip2->ihl * 4);

    if (icmp->type == 11)
    {
        if (ip2->ttl != hops)  // ← vérifie que c'est bien notre sonde
            return (-1);
        return (0);
    }
    else if (icmp->type == 3) // && icmp->code == 3)
    {
        if (ntohs(udp->dest) == dst_port_sent)
            return (1);
    }
    return (-1);
}