
#include <ft_ssl.h>

uint64_t sigma1(uint64_t x)
{
    return(right_rotation(x,6) ^ right_rotation(x,11) ^ right_rotation(x,25));
}

uint64_t sigma2(uint64_t x)
{
    return(right_rotation(x,2) ^ right_rotation(x,13) ^ right_rotation(x,22));
}
uint64_t sigma1_512(uint64_t x)
{
    return(right_rotation_64(x,14) ^ right_rotation_64(x,18) ^ right_rotation_64(x,41));
}

uint64_t sigma2_512(uint64_t x)
{
    return(right_rotation_64(x,28) ^ right_rotation_64(x,34) ^ right_rotation_64(x,39));
}

uint64_t ch(uint64_t x,uint64_t y,uint64_t z)
{
    return((x & y) ^ ((~x) & z));
}


uint64_t maj(uint64_t x,uint64_t y,uint64_t z)
{
    return((x & y) ^ (x & z) ^ (y & z));
}