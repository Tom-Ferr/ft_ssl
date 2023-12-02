#include <ft_ssl.h>

t_dsakey* gen_dsa_primes(int p_bits, int q_bits, int seedlen)
{
    if(seedlen < q_bits)
        return(NULL);

    int sl;
    int prime_reduction;
    uint64_t r;
    t_bigint* first_seed;
    t_dsakey* key;

    sl = seedlen;
    first_seed = init_bigint(0);
    while(sl)
    {
        prime_reduction = (64 - (sl % 64)) % 64;
        bigint_left_shift(first_seed, 64 - prime_reduction);
        r = ft_urand() & (~0UL >> prime_reduction);
        bigint_add_int(first_seed, r);
        sl -= 64 - prime_reduction;
    }

    t_shawe_taylor* q;
    t_shawe_taylor* p_not;
    t_shawe_taylor* p;

    q = shawe_taylor(q_bits, first_seed);
    write(1, "\n", 1);
    p_not = shawe_taylor((int)ft_ceil(((double)p_bits / 2.0) + 1.0), q->prime_seed);

    p = st_generate_random_prime(p_bits, p_not, q->prime);
    write(1, "\n", 1);

    key = (t_dsakey*)malloc(sizeof(t_dsakey));

    key->p = copy_bigint(p->prime);
    key->q = copy_bigint(q->prime);

    free_bigint(first_seed);
    free_shawe_taylor(p);
    free_shawe_taylor(q);

    return(key);
}

void gen_dsa_generator(t_dsakey* params)
{
    t_bigint* one;
    t_bigint* tmp;
    t_bigint* e;
    t_bigint* h;
    t_montgomery* m;

    one= init_bigint(1);
    tmp = bigint_sub(params->p, one);
    e = bigint_div(tmp, params->q, NULL);
    m = init_montgomery(params->p);
    free_bigint(tmp);

    while(1)
    {
        h = init_bigint(ft_rand());

        tmp = montgomery_pow(h, e, params->p, m);
        params->g = bigint_mul_mod(tmp, m->r_inv, params->p);
        if(bigint_compare(params->g, one) > 0)
            break;
        free_bigint(h);
        free_bigint(tmp);
        free_bigint(params->g);
    }
    free_bigint(h);
    free_bigint(e);
    free_bigint(tmp);
    free_bigint(one);
    free_montgomery(m);
}