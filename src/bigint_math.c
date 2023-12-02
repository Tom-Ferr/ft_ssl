#include<ft_ssl.h>

t_bigint* bigint_gcd(t_bigint* a_0, t_bigint* m_0)
{
    t_bigint* _a;
    t_bigint* _m;
    t_bigint* tmp;
    t_bigint* q;
    t_bigint* r;
    t_bigint* zero;
    t_bigint* one;
  
    _a = copy_bigint(m_0);
    _m = copy_bigint(a_0);

    zero = init_bigint(0);
    one = init_bigint(1);

    if (bigint_compare(m_0, one) == 0)
    {
        free_bigint(zero);
        free_bigint(one);
        return(zero);
    }

    while (bigint_compare(_m, zero) == 1)
    {
        q = bigint_div(_a, _m, &r);

        tmp = _m;
        _m = r;
        free_bigint(_a);
        _a = tmp;
        free_bigint(q);
    }
    free_bigint(_m);
    free_bigint(zero);
    free_bigint(one);
    return(_a);
}

t_bigint* bigint_inverse_mod_mul(t_bigint* a_0, t_bigint* m_0)
{
    t_bigint* _a;
    t_bigint* _m;
    t_bigint* tmp;
    t_bigint* q;
    t_bigint* x;
    t_bigint* y;
    t_bigint* r;
    t_bigint* p;
    t_bigint* zero;
  
    _a = copy_bigint(m_0);
    _m = copy_bigint(a_0);

    x = init_bigint(0);
    y = init_bigint(1);
    zero = init_bigint(0);

    if (bigint_compare(m_0, y) == 0)
    {
        free_bigint(y);
        free_bigint(_m);
        free_bigint(_a);
        free_bigint(zero);
        return(x);
    }

    while (bigint_compare(_m, zero) == 1)
    {
        q = bigint_div(_a, _m, &r);

        tmp = _m;
        _m = r;
        free_bigint(_a);
        _a = tmp;

        tmp = y;
        p = bigint_mul(q, y);
        y = bigint_sub(x, p);

        free_bigint(x);
        free_bigint(q);
        free_bigint(p);
        x = tmp;
    }

    if(x->sign)
    {
        tmp = bigint_add(x, m_0);
        free_bigint(x);
        x = tmp;
    }
    free_bigint(_a);
    free_bigint(_m);
    free_bigint(zero);
    free_bigint(y);
    return(x);
}

t_bigint* bigint_mul_mod(t_bigint* a_0, t_bigint* b_0, t_bigint* mod)
{
    t_bigint* r;
    t_bigint* m;
    t_bigint* a;
    t_bigint* b;

    free_bigint(bigint_div(a_0,mod,&a));
    free_bigint(bigint_div(b_0,mod,&b));

    m = bigint_mul(a, b);
    free_bigint(bigint_div(m,mod,&r));
    free_bigint(m);
    free_bigint(a);
    free_bigint(b);
    return(r);
}

t_bigint* bigint_pow_mod(t_bigint* a_0, t_bigint* n_0, t_bigint* mod)
{
    t_bigint* x;
    t_bigint* y;
    t_bigint* n;
    t_bigint* tmp;
    t_bigint* zero;

    zero = init_bigint(0);

    n = copy_bigint(n_0);
    x = init_bigint(1);
    y = copy_bigint(a_0);
    while (bigint_compare(n, zero) == 1)
    {
        if (n->len[n->n-1] & 1)
        {
            tmp = bigint_mul_mod(x, y, mod);
            free_bigint(x);
            x = tmp;
        }
        tmp = bigint_mul_mod(y, y, mod);
        free_bigint(y);
        y = tmp;
        bigint_right_shift(n,1);
    }
    free_bigint(zero);
    free_bigint(y);
    free_bigint(n);
    return(x);
}

// t_bigint* bigint_pow_mod(t_bigint* a_0, t_bigint* n_0, t_bigint* mod)
// {
//     uint64_t cursor;
//     t_bigint* x;
//     t_bigint* tmp;
//     free_bigint(bigint_div(a_0,mod,&x));
//     for (size_t i = 0; i < n_0->n; i++)
//     {
//         cursor = 1UL << 63;
//         while(!(cursor & n_0->len[i]) && cursor)
//         {
//             cursor >>= 1;
//         }
//         if(cursor)
//             break ;
//     }
//     cursor >>= 1;
//     for (size_t i = 0; i < n_0->n; i++)
//     {
//         while (cursor)
//         {
//             tmp = bigint_mul_mod(x,x,mod);
//             free_bigint(x);
//             x = tmp;
//             if(cursor & n_0->len[i])
//             {
//                 tmp = bigint_mul_mod(x,a_0,mod);
//                 free_bigint(x);
//                 x = tmp;
//             }
//             cursor >>= 1;
//         }
//         cursor = 1UL << 63;
//     }
//    return(x);
// }