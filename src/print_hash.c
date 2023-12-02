#include <ft_ssl.h>

void print_hash(int fd, uint8_t* hash, uint32_t bit_size)
{
	uint32_t i = 0;
	uint32_t block_size;
	block_size = bit_size / 8;
    while(i < block_size)
    {
        ft_puthex(fd, *(hash + i), false);
        i++;

    }
}