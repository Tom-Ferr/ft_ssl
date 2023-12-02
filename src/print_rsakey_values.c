#include <ft_ssl.h>

static void put_key_value(int fd, uint8_t* v, size_t len)
{
    for (size_t i = 1; i < len; i++)
    {
        ft_puthex(fd, *v++, false);
        write(fd, ":",1);
        if(i % 15 == 0)
            write(fd, "\n\t",2);
    }
    ft_puthex(fd, *v, false);
    write(fd, "\n",1);
}

void print_key_values(int fd, t_der* der, int vis)
{
    size_t bit_size = getkey_bitsize(der);
    
    if(vis == PRIVATE)
    {
        write(1, "Private-Key: (", 14);
        put_int_base(1, bit_size, 10);
        write(1, " bit, 2 primes)\n", 16);
    }
    else
    {
        write(1, "Public-Key: (", 13);
        put_int_base(1, bit_size, 10);
        write(1, " bit)\n", 6);
    }
    write(fd, "modulus:\n\t", 10);
    put_key_value(fd, der->n, der->l_n);

    write(fd, "publicExponent: ", 16);
    t_bigint* e = string_to_bigint((char*)der->e, der->l_e);
    print_eval(fd, e);
    free_bigint(e);

    if(vis == PRIVATE)
    {
        write(fd, "privateExponent:\n\t", 18);
        put_key_value(fd, der->d, der->l_d);

        write(fd, "prime1:\n\t", 9);
        put_key_value(fd, der->p,der->l_p);

        write(fd, "prime2:\n\t", 9);
        put_key_value(fd, der->q, der->l_q);

        write(fd, "exponent1:\n\t", 12);
        put_key_value(fd, der->dp, der->l_dq);

        write(fd, "exponent2:\n\t", 12);
        put_key_value(fd, der->dq, der->l_dq);

        write(fd, "coefficient:\n\t", 14);
        put_key_value(fd, der->iq, der->l_iq);
    }
}

void print_modulus(int fd, t_rsakey* k)
{
    write(fd, "Modulus=", 8);
    print_bigint(fd, k->n, 16);
    write(fd, "\n", 1);
} 