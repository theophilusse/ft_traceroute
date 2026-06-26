#include "ft_traceroute.h"

ssize_t recv_packet(int sockfd, uint8_t *buf, size_t buf_size,
                    struct sockaddr_in *from)
{
    socklen_t from_len = sizeof(*from);
    
    return recvfrom(sockfd, buf, buf_size, 0,
                    (struct sockaddr *)from, &from_len);
}