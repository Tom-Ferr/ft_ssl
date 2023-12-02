#include <ft_ssl.h>

uint64_t pbox(uint64_t plain, int* p, int input_length, int output_length)
{
    int i;
    uint64_t permuted;

    i=0;
    permuted = 0;
    while(i < output_length)
    {
        permuted <<= 1;
        permuted |= (plain >> (input_length - p[i])) & 1;
        i++;
    }
    return(permuted);
}