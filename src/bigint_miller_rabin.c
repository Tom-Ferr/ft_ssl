#include <ft_ssl.h>

static uint64_t refactor(t_bigint* n_0, t_bigint** m)
{
    uint64_t i;
    t_bigint* tmp;
    t_bigint* n;
    t_bigint* zero;
    t_bigint* two;

    zero = init_bigint(0);

    if(bigint_compare(n_0, zero) == 0)
    {
        free_bigint(zero);
        return(0);
    }

    n = copy_bigint(n_0);

    two = init_bigint(2);

    i = 0;
    while (!(n->len[n->n-1] & 1))
    {
        tmp = bigint_div(n,two, NULL);
        free_bigint(n);
        n = tmp;
        *m = n;
        i++;
    }
    free_bigint(two);
    free_bigint(zero);
    return(i);
}

int bigint_miller_rabin(t_bigint* n, uint8_t p, bool print)
{
    uint64_t k;
    uint64_t i;
    t_bigint* m;
    t_bigint* a;
    t_bigint* b;
    t_bigint* _n;
    t_bigint* one;
    t_bigint* two;
    t_bigint* tmp;

    one = init_bigint(1);
    two = init_bigint(2);

    if(bigint_compare(n, two) == 0)
    {
        free_bigint(one);
        free_bigint(two);
        return(-1);
    }

    if(!(n->len[n->n-1] & 1) || bigint_compare(n, one) == 0)
    {
        free_bigint(one);
        free_bigint(two);
        return(0);
    }
    _n = bigint_sub(n, one);

    k = refactor(_n, &m);
    if(!k)
    {
        free_bigint(one);
        free_bigint(two);
        free_bigint(_n);
        return(0);
    }
    t_montgomery* r = init_montgomery(n);
    t_bigint* _n_r = bigint_mul_mod(_n, r->r, n);
    while (p)
    {
        do
        {
            tmp = init_bigint(ft_urand());
            free_bigint(bigint_div(tmp, _n, &a));
            free_bigint(tmp);
        }  
        while (bigint_compare(one, a) != -1);
        b = montgomery_pow(a, m, n, r);

        if(bigint_compare(b, _n_r) == 0 || bigint_compare(b,r->one_r) == 0)
        {
            p--;
            free_bigint(a);
            free_bigint(b);
            if(print)
                write(1, "+", 1);
            continue;
        }

        i = k;
        while (bigint_compare(b,_n_r) != 0 && bigint_compare(b,r->one_r) != 0 && --i)
        {
            tmp = montgomery_mul(b, b, r, n);
            free_bigint(b);
            b = tmp;
        }
        if(bigint_compare(b,r->one_r) == 0 || !i)
        {
            free_bigint(one);
            free_bigint(two);
            free_bigint(_n);
            free_bigint(_n_r);
            free_bigint(m);
            free_bigint(a);
            free_bigint(b);
            free_montgomery(r);
            return(1);
        }
        free_bigint(a);
        free_bigint(b);
        if(print)
            write(1, "+", 1);
        p--;
    }
    free_bigint(one);
    free_bigint(two);
    free_bigint(_n);
    free_bigint(_n_r);
    free_bigint(m);
    free_montgomery(r);
    return(-1);
}