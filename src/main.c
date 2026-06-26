#include "ft_traceroute.h"

t_opts  g_opts;

void    sig_handler(int sig)
{
    (void)sig;
    exit(0);
}

int     main(int argc, char **argv)
{
    struct sockaddr_in dest;
    struct sockaddr_in from;

	if (argc < 2)
	{
		print_usage();
		return (1);
	}
    // -- droits root
    if (geteuid() != 0)
    {
        fprintf(stderr, "ft_traceroute: permission denied (need root)\n");
        return (1);
    }

    // -- parsing
    g_opts = parse_args(argc, argv);

    // -- signal
    signal(SIGINT, sig_handler);

    // -- résolution DNS
    dest = resolve_host(g_opts.hostname);
    memset(&from, 0, sizeof(from));

    int socketin, socketout;
    if ((socketin = create_socket_icmp(&g_opts)) == -1)
    {
        fprintf(stderr, "ft_traceroute: create_socket_icmp failed\n");
        return (1);
    }
    if ((socketout = create_socket_udp(&g_opts)) == -1)
    {
        fprintf(stderr, "ft_traceroute: create_socket_udp failed\n");
        return (1);
    }

    // -- affichage entête
    printf("traceroute to %s (%s), 64 hops max, %d byte packets\n",
        g_opts.hostname, inet_ntoa(dest.sin_addr), 64);

    dest.sin_port = htons(33435);
    // connect "virtuel" pour que le kernel choisisse l'interface
    if (connect(socketout, (struct sockaddr *)&dest, sizeof(dest)) < 0)
    {
        fprintf(stderr, "ft_traceroute: connect: %s\n", strerror(errno));
        return (1);
    }
    GET_LOCAL_IP(g_opts.local_ip);
    if (g_opts.verbose)
        printf("Using interface: %s\n", g_opts.iface ? g_opts.iface : g_opts.iface_name);
    struct sockaddr unspec;
    memset(&unspec, 0, sizeof(unspec));
    unspec.sa_family = AF_UNSPEC;
    connect(socketout, &unspec, sizeof(unspec));
    dest.sin_port = 0;

    // -- boucle
    traceroute_loop(socketin, socketout, &from, &dest, &g_opts);

    close(socketin);
    close(socketout);
    return (0);
}