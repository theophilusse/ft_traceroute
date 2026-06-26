# ft_traceroute

A re-implementation of the `traceroute` command, written in C as part of the 42 school Unix projects.

## Overview

`ft_traceroute` traces the route IP packets take to reach a network host.
It works by sending UDP probes with increasing TTL values and listening for
ICMP "Time Exceeded" responses from intermediate routers, and ICMP
"Destination Unreachable" from the final host.

## Requirements

- Linux kernel >= 4.0 (64-bit Debian recommended for evaluation)
- Root privileges (raw sockets require root)

## Build

```
make
```

## Usage

```
sudo ./ft_traceroute [OPTIONS] HOST
```

## Options

```
-v                   verbose output (show interface)
-i, --iface=IFACE    set network interface
-m, --max_ttl=TTL    set max time-to-live (default: 30)
-p, --port=PORT      set base destination port (default: 33434)
-s, --src_addr=ADDR  set source address
-q, --n_queries=N    set number of probes per hop (default: 3)
-N, --s_queries=N    set number of simultaneous probes (default: 3)
-t, --tos=TOS        set type of service (default: 0)
-l, --flow_label=N   set flow label (default: 0)
-?, --help           show this help
```

## Example

```
$ sudo ./ft_traceroute google.com
traceroute to google.com (142.251.142.14), 30 hops max, 64 byte packets
 1  livebox.home (192.168.1.1) 76 bytes to 192.168.1.13  5.184 ms  *  *
 2  80.15.225.157 (80.15.225.157) 76 bytes to 192.168.1.13  7.334 ms  *  *
 3  ae109-0.ncren102.rbci.orange.net (193.253.81.226) 36 bytes to 192.168.1.13  9.810 ms  *  *
 4  * * *
 5  193.252.137.78 (193.252.137.78) 76 bytes to 192.168.1.13  24.663 ms  *  *
 ...
 9  tzpara-an-in-f14.1e100.net (142.251.142.14) 36 bytes to 192.168.1.13  16.597 ms  *  *
```

## How it works

1. Send UDP probes to a high port (>33434) with TTL = 1
2. Each router that drops the packet sends back ICMP "Time Exceeded"
3. Increment TTL and repeat to discover the next hop
4. Stop when ICMP "Destination Unreachable" is received (host reached)
   or when max TTL is reached

## Sockets

- `SOCK_DGRAM` + `IPPROTO_UDP` : send probes
- `SOCK_RAW`  + `IPPROTO_ICMP` : receive ICMP replies

## Author

ttrossea@student.42.fr
