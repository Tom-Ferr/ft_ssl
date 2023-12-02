#include <ft_ssl.h>

uint64_t pick_random_e(uint64_t t)
{
    uint64_t e;

    while(1)
    {
        e = ft_urand() % t;
        if(miller_rabin(e, 75) >= 0)
            continue;
        if((t % e) == 0)
            continue;
        break;
    }
    return(e);
}

void rsa_keygen(t_rsakey* k)
{
    t_bigint* t;
    t_bigint* _p;
    t_bigint* _q;
    t_bigint* one;

    one = init_bigint(1);
    k->n = bigint_mul(k->p, k->q);
    _p = bigint_sub(k->p, one);
    _q = bigint_sub(k->q, one);
    t = bigint_mul(_p, _q);

    k->e = init_bigint(0x10001);

    k->d = bigint_inverse_mod_mul(k->e,t);

    free_bigint(bigint_div(k->d, _p, &k->dp));  
    free_bigint(bigint_div(k->d, _q, &k->dq));  
    k->iq = bigint_inverse_mod_mul(k->q,k->p);

    free_bigint(one);
    free_bigint(_p);
    free_bigint(_q);
    free_bigint(t);

}