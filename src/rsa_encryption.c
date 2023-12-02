#include <ft_ssl.h>

t_bigint* rsa_encode(uint8_t* m, size_t m_bytes, t_rsakey k)
{
    t_montgomery* mont = init_montgomery(k.n);
    
    t_bigint* m_int = string_to_bigint((char*)m, m_bytes);

    if(bigint_compare(m_int, k.n) == 1)
    {
        error_message("rsautl", NULL, ": message too long\n");
        exit(1);
    }
    t_bigint* _c = montgomery_pow(m_int, k.e, k.n, mont);
    t_bigint* c = bigint_mul_mod(_c, mont->r_inv, k.n);
    free_bigint(m_int);
    free_bigint(_c);
    free_montgomery(mont);
    return(c);
}

t_bigint* rsa_decode(uint8_t* c, size_t c_bytes, t_rsakey k)
{
    t_montgomery* mont1 = init_montgomery(k.p);
    t_montgomery* mont2 = init_montgomery(k.q);

    t_bigint* m_int = string_to_bigint((char*)c, c_bytes);

    t_bigint* _m1 = montgomery_pow(m_int, k.dp, k.p, mont1);
    t_bigint* _m2 = montgomery_pow(m_int, k.dq, k.q, mont2);
    t_bigint* m1 = bigint_mul_mod(_m1, mont1->r_inv, k.p);
    t_bigint* m2 = bigint_mul_mod(_m2, mont2->r_inv, k.q);
    t_bigint* ms = bigint_sub(m1, m2);
    if(ms->sign)
    {
        t_bigint* tmp = bigint_add(ms, k.p);
        free_bigint(ms);
        ms = tmp;
    }
    t_bigint* h = bigint_mul_mod(ms, k.iq, k.p);
    t_bigint* qh = bigint_mul(k.q, h);
    t_bigint* pl = bigint_add(m2, qh);

    free_bigint(m_int);
    free_bigint(m1);
    free_bigint(m2);
    free_bigint(_m1);
    free_bigint(_m2);
    free_bigint(ms);
    free_bigint(h);
    free_bigint(qh);
    free_montgomery(mont1);
    free_montgomery(mont2);

    return(pl);
}