
#include <ft_ssl.h>

uint32_t right_rotation(uint32_t x,uint32_t y)
{
    return( (x >> y) | (x << (32 - y)));
}

uint32_t left_rotation(uint32_t x,uint32_t y)
{
    return( (x << y) | (x >> (32 - y)));
}

uint64_t right_rotation_64(uint64_t x,uint64_t y)
{
    return( (x >> y) | (x << (64 - y)));
}

uint64_t left_rotation_64(uint64_t x,uint64_t y)
{
    return( (x << y) | (x >> (64 - y)));
}