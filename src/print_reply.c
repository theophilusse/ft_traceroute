#include "ft_traceroute.h"

void print_reply(struct sockaddr_in *from, double *rtt, ssize_t bytes, t_opts *opts)
{
    char server[NI_MAXHOST];

    if (getnameinfo((struct sockaddr *)from, sizeof(*from), server, NI_MAXHOST, NULL, 0, 0) != 0)
    {
        memcpy(server, inet_ntoa(from->sin_addr), NI_MAXHOST);
        server[NI_MAXHOST - 1] = '\0';
    }
    char from_ip[INET_ADDRSTRLEN];
    char local_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &from->sin_addr, from_ip, sizeof(from_ip));
    inet_ntop(AF_INET, &opts->local_ip, local_ip, sizeof(local_ip));

    printf("%s (%s) %zu bytes to %s ",
        server, from_ip, bytes - sizeof(t_ip), local_ip);
    for (int i = 0; i < opts->s_queries; i++)
    {
        if (rtt[i] == -1)
            printf(" * ");
        else
            printf(" %.3f ms ", rtt[i]);
    }
    printf("\n");
}