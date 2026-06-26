#include "ft_traceroute.h"

static uint16_t    parse_get_port(uint8_t *buf)
{
    t_ip   *ip   = (t_ip *)buf;
    t_icmp *icmp = (t_icmp *)(buf + ip->ihl * 4);
    t_ip   *ip2  = (t_ip *)((uint8_t *)icmp + sizeof(t_icmp));
    t_udp  *udp  = (t_udp *)((uint8_t *)ip2 + ip2->ihl * 4);
    return ntohs(udp->dest);
}

void    traceroute_loop(int sockin, int sockout, struct sockaddr_in *from,
                        struct sockaddr_in *dest, t_opts *opts)
{
    int             hops = 1;
    double          rtt[opts->s_queries];
    int             received[opts->s_queries];
    uint8_t         buf[1024];
    ssize_t         n;
    struct timeval  sent[opts->s_queries];

    int base_port = opts->port;
    while (hops <= opts->max_ttl)
    {
        printf("%2d  ", hops);
        memset(from, 0, sizeof(*from));
        for (int i = 0; i < opts->s_queries; i++)
            rtt[i] = -1;
        for (int i = 0; i < opts->s_queries; i++)
            received[i] = 0;

        // PHASE 1 : envoyer toutes les sondes
        for (int i = 0; i < opts->s_queries; i++)
        {
            gettimeofday(&sent[i], NULL);
            send_probe(sockout, dest, hops, base_port + i);
        }

        // PHASE 2 : collecter les reponses
        struct timeval deadline;
        gettimeofday(&deadline, NULL);
        deadline.tv_sec += 5;
        int total_received = 0;
        int reached = 0;
        while (total_received < opts->s_queries)
        {
            struct timeval now, timeout;
            gettimeofday(&now, NULL);
            timeout.tv_sec  = deadline.tv_sec  - now.tv_sec;
            timeout.tv_usec = deadline.tv_usec - now.tv_usec;
            if (timeout.tv_sec < 0 || (timeout.tv_sec == 0 && timeout.tv_usec <= 0))
                break;
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(sockin, &fds);
            if (select(sockin + 1, &fds, NULL, NULL, &timeout) <= 0)
                break;
            n = recv_packet(sockin, buf, sizeof(buf), from);
            if (n > 0)
            {
                int port_recu = parse_get_port(buf);
                int idx = port_recu - base_port;
                if (idx >= 0 && idx < opts->s_queries && !received[idx])
                {
                    received[idx] = 1;
                    rtt[idx] = compute_rtt(&sent[idx]);
                    total_received++;
                    if (parse_reply(buf, port_recu, hops) == 1)
                    {
                        reached = 1;
                        break;
                    }
                }
            }
        }
        if (from->sin_addr.s_addr == 0)
            printf("* * *\n");
        else
            print_reply(from, rtt, n, opts);
        base_port += opts->s_queries;
        fd_set fds;
        struct timeval zero = {0, 0};
        FD_ZERO(&fds);
        FD_SET(sockin, &fds);
        while (select(sockin + 1, &fds, NULL, NULL, &zero) > 0)
        {
            recv_packet(sockin, buf, sizeof(buf), from);
            FD_ZERO(&fds);
            FD_SET(sockin, &fds);
        }
        if (reached)
            return;
        hops++;
    }
}