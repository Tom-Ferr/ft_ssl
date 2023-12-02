
#include <ft_ssl.h>

uint64_t f_process(uint64_t x,uint64_t y,uint64_t z)
{
    return((x & y) | ((~x) & z));
}

uint64_t g_process(uint64_t x,uint64_t y,uint64_t z)
{
    return((x & z) | (y & (~z)));
}

uint64_t h_process(uint64_t x,uint64_t y,uint64_t z)
{
    return(x ^ y ^ z);
}

uint64_t i_process(uint64_t x,uint64_t y,uint64_t z)
{
    return(y ^ (x | (~z)));
}