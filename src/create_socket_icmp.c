#include "ft_traceroute.h"

int create_socket_icmp(t_opts *opts)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        fprintf(stderr, "ft_traceroute: socket: %s\n", strerror(errno));
        exit(1);
    }
    // interface -i
    if (opts->iface)
    {
        if (BIND_IFACE(sockfd, opts->iface) < 0)
        {
            fprintf(stderr, "ft_traceroute: interface: %s\n", strerror(errno));
            exit(1);
        }
    }
    // source address -s
    if (opts->src_addr)
    {
        struct sockaddr_in src;
        memset(&src, 0, sizeof(src));
        src.sin_family = AF_INET;
        inet_pton(AF_INET, opts->src_addr, &src.sin_addr);
        bind(sockfd, (struct sockaddr *)&src, sizeof(src));
    }
    return sockfd;
}