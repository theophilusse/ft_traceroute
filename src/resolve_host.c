#include "ft_traceroute.h"

struct sockaddr_in  resolve_host(const char *hostname)
{
    struct addrinfo     hints;
    struct addrinfo     *res;
    struct sockaddr_in  addr;
    int                 ret;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    ret = getaddrinfo(hostname, NULL, &hints, &res);
    if (ret != 0)
    {
        fprintf(stderr, "ft_traceroute: %s: %s\n", hostname, gai_strerror(ret));
        exit(1);
    }
    memcpy(&addr, res->ai_addr, sizeof(addr));
    freeaddrinfo(res);
    return addr;
}
