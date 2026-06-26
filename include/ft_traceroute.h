#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H
# include <sys/socket.h>    // socket(), setsockopt(), sendto(), recvfrom()
# include <netinet/in.h>    // struct sockaddr_in, IPPROTO_ICMP, htons()
# include <arpa/inet.h>     // inet_ntoa(), inet_pton(), inet_ntop()
# include <netdb.h>         // getaddrinfo(), getnameinfo()
# include <sys/time.h>      // struct timeval, gettimeofday()
# include <signal.h>        // signal(), SIGINT
# include <stdio.h>         // printf(), fprintf()
# include <stdlib.h>        // exit()
# include <string.h>        // memset(), memcpy(), strlen()
# include <unistd.h>        // close()
# include <stdint.h>        // uint8_t, uint16_t, uint32_t
# include <getopt.h>        // getopt_long()
# include <errno.h>         // errno, strerror()
# include <ifaddrs.h>       // getifaddrs()
# include <sys/select.h>    // select()
# ifdef __linux__
#  define GET_LOCAL_IP(ip) do { \
    struct ifaddrs *ifap, *ifa; \
    getifaddrs(&ifap); \
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) { \
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) { \
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr; \
            if (sa->sin_addr.s_addr != htonl(INADDR_LOOPBACK)) { \
                (ip) = sa->sin_addr; g_opts.iface_name = strdup(ifa->ifa_name); break; } } } \
    freeifaddrs(ifap); \
} while(0)
#  define BIND_IFACE(fd, iface) setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, iface, strlen(iface))
# else
#  include <net/if.h>
#  define GET_LOCAL_IP(ip) do { \
    struct ifaddrs *ifap, *ifa; \
    getifaddrs(&ifap); \
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) { \
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) { \
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr; \
            if (sa->sin_addr.s_addr != htonl(INADDR_LOOPBACK)) { \
                (ip) = sa->sin_addr; g_opts.iface_name = strdup(ifa->ifa_name); break; } } } \
    freeifaddrs(ifap); \
} while(0)
#  define BIND_IFACE(fd, iface) ({ \
    unsigned int idx = if_nametoindex(iface); \
    setsockopt(fd, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx)); \
})
# endif
# include "ip.h"
# include "icmp.h"
# include "udp.h"
# include "opts.h"
void    print_usage(void);
t_opts  parse_args(int argc, char **argv);
struct sockaddr_in  resolve_host(const char *hostname);
int     create_socket_icmp(t_opts *opts);
int     create_socket_udp(t_opts *opts);
void    send_probe(int sockfd, struct sockaddr_in *dest, int ttl, int dst_port);
ssize_t recv_packet(int sockfd, uint8_t *buf, size_t buf_size, struct sockaddr_in *from);
int     parse_reply(uint8_t *buf, uint16_t dst_port_sent, int hops);
double  compute_rtt(struct timeval *sent);
void    print_reply(struct sockaddr_in *from, double *rtt, ssize_t bytes, t_opts *opts);
void    traceroute_loop(int sockin, int sockout, struct sockaddr_in *from,
            struct sockaddr_in *dest, t_opts *opts);
extern t_opts   g_opts;
#endif