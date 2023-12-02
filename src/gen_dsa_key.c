#include<ft_ssl.h>

void gen_dsa_key(t_dsakey* key)
{
    t_bigint* one;
    t_bigint* tmp;
    t_bigint* c;
    t_montgomery* m;
    int prime_reduction;
    int bits;
    uint64_t r;

    bits = 224;
    one = init_bigint(1);
    c = init_bigint(0);
    m = init_montgomery(key->p);
    while(bits)
    {
        prime_reduction = (64 - (bits % 64)) % 64;
        bigint_left_shift(c, 64 - prime_reduction);
        r = ft_urand() & (~0UL >> prime_reduction);
        bigint_add_int(c, r);
        bits -= 64 - prime_reduction;
    }

    tmp = bigint_sub(key->q, one);
    free_bigint(bigint_div(c, tmp, &key->x));
    bigint_add_int(key->x,1);
    free_bigint(tmp);
    tmp = montgomery_pow(key->g, key->x, key->p, m);
    key->y = bigint_mul_mod(tmp, m->r_inv, key->p);

    free_bigint(c);
    free_bigint(tmp);
    free_bigint(one);
    free_montgomery(m);
}