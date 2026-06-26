#include "ft_traceroute.h"

struct s_opts default_opts(void)
{
	struct s_opts opts;

	memset(&opts, 0, sizeof(opts));
	opts.verbose     = 0;
	opts.iface       = NULL;
	opts.max_ttl     = 30;
	opts.port        = 33434;
	opts.src_addr    = NULL;
	opts.n_queries   = 3;
	opts.s_queries   = 3;
	opts.tos         = 0;
	opts.flow_label  = 0;
	opts.hostname    = NULL;
	return opts;
}

t_opts  parse_args(int argc, char **argv)
{
    t_opts  opts;
    int     opt;
    int     longindex;

	opts = default_opts();
    while ((opt = getopt_long(argc, argv, "vi:m:p:s:q:N:t:l:?", long_opts, &longindex)) != -1)
    {
		switch (opt)
        {
            case 'v':
				opts.verbose = 1;
				break;
			case 'i':
				opts.iface = argv[optind - 1];
				break;
			case 'm': /* convertir optarg en int, valider les bornes */
				opts.max_ttl = atoi(optarg);
				if (opts.max_ttl < 0 || opts.max_ttl > 255)
				{
					fprintf(stderr, "ft_traceroute: invalid argument: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 'p':
				opts.port = atoi(optarg);
				if (opts.port < 0 || opts.port > 65535)
				{
					fprintf(stderr, "ft_traceroute: invalid argument: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 's':
				opts.src_addr = argv[optind - 1];
				break;
			case 'q':
				opts.n_queries = atoi(optarg);
				break;
			case 'N':
				opts.s_queries = atoi(optarg);
				break;
			case 't':
				opts.tos = atoi(optarg);
				if (opts.tos < 0 || opts.tos > 255)
				{
					fprintf(stderr, "ft_traceroute: invalid argument: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 'l':
				opts.flow_label = atoi(optarg);
				break;
			case '?':
				print_usage();
				exit(0);
				break;
        }
    }
    if (optind >= argc)
        return opts;
    opts.hostname = argv[optind];
    return opts;
}
