#include <ft_ssl.h>

void put_int_base(int fd, uint64_t e, int base)
{
    uint64_t f;
    uint64_t g;
    size_t len;
    
    f = 0;
    len = 1;
    if(e == 0)
        write(fd, "0", 1);
    while (e)
    {
        f <<= 4;
        f |= e % base;
        e /= base;
        len++;
    }
    while (--len)
    {
        g = (f & 0xf) + '0';
        if(g >= 58)
            g += 7;
        write(fd, &g, 1);
        f >>= 4;   
    }
    
}