#include "ft_traceroute.h"

void    send_probe(int sockfd, struct sockaddr_in *dest, int ttl, int dst_port)
{
    char payload[40];

    setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
    dest->sin_port = htons(dst_port);
    memset(payload, 0, sizeof(payload));
    sendto(sockfd, payload, sizeof(payload), 0, (struct sockaddr *)dest, sizeof(*dest));
}