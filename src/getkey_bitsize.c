#include <ft_ssl.h>

size_t getkey_bitsize(t_der* der)
{
    size_t bit_size;
    int neg_bit = 0;
    uint8_t msb;
    bit_size = (der->l_n * 8);
    msb = ~(*(der->n));
    while (msb & 0x80)
    {
        neg_bit++;
        msb <<= 1;
    }
    bit_size -= neg_bit;
    return(bit_size);
}