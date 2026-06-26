#include "ft_traceroute.h"

uint16_t    checksum(void *data, size_t len)
{
    uint16_t    *buf;
    uint32_t    sum;

    buf = (uint16_t *)data;
    sum = 0;

    // -- additionner tous les mots de 16 bits
    while (len > 1)
    {
        sum += *buf++;
        len -= 2;
    }

    // -- s'il reste un octet impair, le padder avec un zéro
    if (len == 1)
        sum += *(uint8_t *)buf;

    // -- ramener les retenues dans les 16 bits inférieurs
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    // -- complément à un on inverse le résultat
    return (~sum);
}