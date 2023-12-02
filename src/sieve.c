#include <ft_ssl.h>

int sieve(t_bigint* p, int bit_size)
{
    int sl;
    int it;
    int prime_reduction;
    uint64_t r;
    t_bigint* random;
    t_bigint* gcd;
    t_bigint* one;

    if((p->len[p->n-1] & 0xf) == 5)
        return(1);

    one = init_bigint(1);
    it = 15;

    while (it--)
    {
        sl = bit_size / 2;
        random = init_bigint(0);
        while(sl)
        {
            prime_reduction = (64 - (sl % 64)) % 64;
            bigint_left_shift(random, 64 - prime_reduction);
            r = ft_urand() & (~0UL >> prime_reduction);
            bigint_add_int(random, r);
            sl -= 64 - prime_reduction;
        }
        gcd = bigint_gcd(random, p);

        if(bigint_compare(gcd, one) != 0)
        {
            free_bigint(random);
            free_bigint(gcd);
            free_bigint(one);
            return(1);
        }
        free_bigint(random);
        free_bigint(gcd);
    }
    free_bigint(one);
    return(-1);
}