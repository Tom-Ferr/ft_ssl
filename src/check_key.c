#include <ft_ssl.h>

void check_rsakey(t_rsakey* k)
{
    t_bigint* check_n;
    t_bigint* check_e;
    t_bigint* check_d;
    t_bigint* check_dp;
    t_bigint* check_dq;
    t_bigint* check_iq;
    t_bigint* t;
    t_bigint* _p;
    t_bigint* _q;
    t_bigint* one;
    one = init_bigint(1);
    check_n = bigint_mul(k->p, k->q);
    check_e   = init_bigint(0x10001);
    _p = bigint_sub(k->p, one);
    _q = bigint_sub(k->q, one);
    t = bigint_mul(_p, _q);
    check_d  = bigint_mul_mod(k->e, k->d, t);
    check_dp  = bigint_mul_mod(k->e, k->dp, _p);
    check_dq  = bigint_mul_mod(k->e, k->dq, _q);
    check_iq  = bigint_mul_mod(k->q, k->iq, k->p);
    
    while(1)
    {
        if(bigint_miller_rabin(k->p, 3, false) != -1)
            break ;
        if(bigint_miller_rabin(k->q, 3, false) != -1)
            break ;
        if(bigint_compare(check_n, k->n) != 0)
            break ;
        if(bigint_compare(k->e, check_e) != 0)
            break ;
        if(bigint_compare(check_d, one) != 0)
            break ;
        if(bigint_compare(check_dp, one) != 0)
            break ;
        if(bigint_compare(check_dq, one) != 0)
            break ;
        if(bigint_compare(check_iq, one) != 0)
            break ;
        write(1, "RSA key ok\n", 11);
        free_bigint(one);
        free_bigint(_p);
        free_bigint(_q);
        free_bigint(t);
        free_bigint(check_n);
        free_bigint(check_e);
        free_bigint(check_d);
        free_bigint(check_dp);
        free_bigint(check_dq);
        free_bigint(check_iq);
        return ;
    }
    free_bigint(one);
    free_bigint(_p);
    free_bigint(_q);
    free_bigint(t);
    free_bigint(check_n);
    free_bigint(check_e);
    free_bigint(check_d);
    free_bigint(check_dp);
    free_bigint(check_dq);
    free_bigint(check_iq);
    write(1, "RSA key not ok\n", 15);
}