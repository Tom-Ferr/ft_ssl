#include<ft_ssl.h>

t_montgomery* init_montgomery(t_bigint* mod)
{
    t_bigint* one;
    size_t blocks = mod->n;
    size_t msbl;
    t_montgomery* m;

    m = (t_montgomery*)malloc(sizeof(t_montgomery));

    m->r_bits = 0;

    for (size_t i = 0; i < mod->n; i++)
    {
        blocks--;
        msbl = mod->len[i];
        if(mod->len[i])
            break;
    }

    while(msbl)
    {
        msbl >>= 1;
        m->r_bits++;
    }

    m->r_bits += blocks * 64;
    m->r_bits = (m->r_bits / 8 + 1) * 8;

    one = init_bigint(1);

    m->r = init_bigint(1);
    bigint_left_shift(m->r, m->r_bits);
    m->mask = bigint_sub(m->r, one);
    m->r_inv = bigint_inverse_mod_mul(m->r, mod);
    m->mod_inv = bigint_inverse_mod_mul(mod, m->r);
    m->k = bigint_sub(m->r, m->mod_inv);

    m->one_r = bigint_mul_mod(one, m->r, mod);

    free_bigint(one);
    return(m);
}

void free_montgomery(t_montgomery* m)
{
    free_bigint(m->r);
    free_bigint(m->r_inv);
    free_bigint(m->mod_inv);
    free_bigint(m->k);
    free_bigint(m->mask);
    free_bigint(m->one_r);
    free(m);
}

t_bigint* montgomery_mul(t_bigint* _a, t_bigint* _b, t_montgomery* m, t_bigint* mod)
{
    t_bigint* x;
    t_bigint* s;
    t_bigint* e;
    t_bigint* t;
    t_bigint* u;
    t_bigint* _c;

    x = bigint_mul(_a,_b);
    s = bigint_mul(x,m->k);
    e = bigint_and(s, m->mask);
    t = bigint_mul(e,mod);
    u = bigint_add(x, t);
    bigint_right_shift(u, m->r_bits);
    free_bigint(x);
    free_bigint(s);
    free_bigint(e);
    free_bigint(t);

    if(bigint_compare(u, mod) == -1)
        return(u);
    _c = bigint_sub(u, mod);
    free_bigint(u);
    return(_c);

}

t_bigint* montgomery_pow(t_bigint* a_0, t_bigint* n_0, t_bigint* mod, t_montgomery* m)
{
    t_bigint* x;
    t_bigint* y;
    t_bigint* n;
    t_bigint* tmp;
    t_bigint* zero;
    

    zero = init_bigint(0);

    n = copy_bigint(n_0);

    y = bigint_mul_mod(a_0, m->r, mod);
    x = copy_bigint(m->one_r);
    while (bigint_compare(n, zero) == 1)
    {
        if (n->len[n->n-1] & 1)
        {
            tmp = montgomery_mul(x, y, m, mod);
            free_bigint(x);
            x = tmp;
        }
        tmp = montgomery_mul(y, y, m, mod);
        free_bigint(y);
        y = tmp;
        bigint_right_shift(n,1);
    }
    free_bigint(zero);
    free_bigint(y);
    free_bigint(n);
    return(x);
}