#include <ft_ssl.h>

t_bigint* prime_gen(int bit_size)
{
    uint64_t n;
    t_bigint* bg;
    int res;
    int prime_size;
    int prime_reduction;


    res = 0;
        prime_size = bit_size / 2;
        bg = init_bigint(0);
        while(prime_size)
        {
            prime_reduction = (64 - (prime_size % 64)) % 64;
            bigint_left_shift(bg, 64 - prime_reduction);
            n = ft_urand() & (~0UL >> prime_reduction);
            if(prime_size == bit_size /2)
                n |= (3UL << ((64 - prime_reduction) - 2) );
            bigint_add_int(bg, n);
            prime_size -= 64 - prime_reduction;
        }
        bg->len[bg->n-1] |= 1;
    while(res >= 0)
    {
        int sv = sieve(bg, bit_size / 2);
        if(sv < 0)
        {
            write(1, ".", 1);
            res = bigint_miller_rabin(bg, 12, true);
            if(res == -1)
                break;
        }
        bigint_add_int(bg, 2);
    }
    write(1, "\n", 1);
    return(bg);
}