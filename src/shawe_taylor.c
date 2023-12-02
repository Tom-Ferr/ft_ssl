#include<ft_ssl.h>

void free_shawe_taylor(t_shawe_taylor* c)
{
    free_bigint(c->prime);
    free_bigint(c->prime_seed);
    free(c);
}

t_shawe_taylor* st_generate_random_prime(size_t length, t_shawe_taylor* key, t_bigint* salt)
{
    size_t iterations = (size_t)(ft_ceil((double)length / 256.0) - 1.0); 
    size_t old_counter = key->prime_gen_counter;

    t_bigint* zero;
    t_bigint* one;
    t_bigint* tmp;
    t_bigint* prime_seed;
    t_bigint* hat_len;
    t_bigint* s2;
    t_bigint* t2;
    t_bigint* t;
    t_bigint* c;
    t_bigint* c2;
    t_bigint* c_3;
    t_bigint* x;
    t_bigint* a;
    t_bigint* z;
    t_bigint* z_hat_q;
    t_bigint* gcd;
    t_bigint* mod;
    t_bigint* mask;
    t_montgomery* m;

    one = init_bigint(1);
    mod = init_bigint(1);
    bigint_left_shift(mod, length - 1);
    mask = bigint_sub(mod, one);

    zero = init_bigint(0);

    if(salt == NULL)
        salt = one;

    x = init_bigint(0);
    for (size_t i = 0; i <= iterations; i++)
    {
        uint8_t* str_seed;
        prime_seed = copy_bigint(key->prime_seed);
        bigint_add_int(prime_seed, i);

        size_t h_seed = bigint_to_string(&str_seed, prime_seed);
        uint8_t* hash = sha256(str_seed, h_seed);
        free_bigint(prime_seed);
        prime_seed = string_to_bigint((char*)hash, 256 / 8);

        t_bigint* iout = init_bigint(1);
        bigint_left_shift(iout, i * 256);

        tmp = bigint_mul(prime_seed, iout);
        free_bigint(prime_seed);
        prime_seed = tmp;

        tmp = bigint_add(x, prime_seed);
        free_bigint(x);
        x = tmp;

        free_bigint(prime_seed);
        free_bigint(iout);
        free(hash);
        free(str_seed);
    }
    bigint_add_int(key->prime_seed, iterations + 1);

    tmp = bigint_and(x, mask);
    free_bigint(x);
    x = bigint_add(mod, tmp);
    free_bigint(tmp);

    s2 = bigint_add(salt, salt);
    c2 = bigint_mul(s2, key->prime);
    t = bigint_div(x, c2, &tmp);
    if(bigint_compare(tmp, zero) > 0)
        bigint_add_int(t, 1);
    free_bigint(tmp);

    hat_len = init_bigint(1);
    bigint_left_shift(hat_len, length);

    while (1)
    {
        tmp = bigint_add(t,t);
        t2 = bigint_mul(tmp, salt);
        c = bigint_mul(t2, key->prime);
        bigint_add_int(c, 1);
        free_bigint(tmp);

        if(bigint_compare(c, hat_len) > 0)
        {
            free_bigint(c);
            free_bigint(t);
            free_bigint(t2);
            t = bigint_div(mod, c2, &tmp);
            if(bigint_compare(tmp, zero) > 0)
                bigint_add_int(t, 1);
            free_bigint(tmp);
            tmp = bigint_add(t,t);
            t2 = bigint_mul(tmp, salt);
            c = bigint_mul(t2, key->prime);
            bigint_add_int(c, 1);
            free_bigint(tmp);
        }
        key->prime_gen_counter++;

        int sv = sieve(c, length);

        if(sv < 0)
        {
            write(1, ".", 1);
            a = init_bigint(0);
            for (size_t i = 0; i <= iterations; i++)
            {
                uint8_t* str_seed;
                uint8_t* hash ;
                size_t h_seed;
                t_bigint* iout;

                prime_seed = copy_bigint(key->prime_seed);
                bigint_add_int(prime_seed, i);

                h_seed = bigint_to_string(&str_seed, prime_seed);
                hash = sha256(str_seed, h_seed);
                free_bigint(prime_seed);
                prime_seed = string_to_bigint((char*)hash, 256 / 8);

                iout = init_bigint(1);
                bigint_left_shift(iout, i * 256);
                tmp = bigint_mul(prime_seed, iout);
                free_bigint(prime_seed);

                prime_seed = tmp;
                tmp = bigint_add(a, prime_seed);
                free_bigint(a);
                a = tmp;

                free_bigint(prime_seed);
                free_bigint(iout);
                free(hash);
                free(str_seed);
            }
            bigint_add_int(key->prime_seed, iterations + 1);

            c_3 = copy_bigint(c);
            bigint_sub_int(c_3, 3);

            free_bigint(bigint_div(a, c_3, &tmp));
            free_bigint(a);
            a = tmp;
            bigint_add_int(a,2);

            m = init_montgomery(c);
            tmp = montgomery_pow(a,t2, c, m);
            z = bigint_mul_mod(tmp, m->r_inv, c);
            free_bigint(tmp);

            tmp = montgomery_pow(z, key->prime, c, m);
            z_hat_q = bigint_mul_mod(tmp, m->r_inv, c);
            free_bigint(tmp);

            bigint_sub_int(z,1);
            gcd = bigint_gcd(z, c);

            free_bigint(z);
            free_bigint(a);
            free_bigint(c_3);
            free_montgomery(m);

            if(bigint_compare(gcd, one) == 0 && bigint_compare(z_hat_q, one) == 0)
            {
                write(1, "+", 1);
                free_bigint(key->prime);
                key->prime = c;
                free_bigint(z_hat_q);
                free_bigint(gcd);
                free_bigint(hat_len);
                free_bigint(t2);
                free_bigint(s2);
                free_bigint(c2);
                free_bigint(t);
                free_bigint(x);
                free_bigint(zero);
                free_bigint(one);
                free_bigint(mod);
                free_bigint(mask);
                return(key);
            }
            free_bigint(gcd);
            free_bigint(z_hat_q);
            if(key->prime_gen_counter > ((4 * length) + old_counter))
            {
                free_bigint(hat_len);
                free_bigint(t2);
                free_bigint(s2);
                free_bigint(c2);
                free_bigint(t);
                free_bigint(x);
                free_bigint(zero);
                free_bigint(one);
                free_bigint(mod);
                free_bigint(mask);
                free_bigint(c);
                return(NULL);
            }
        }
        free_bigint(c);
        free_bigint(t2);
        bigint_add_int(t,1);
    }
}

t_shawe_taylor* shawe_taylor(size_t length, t_bigint* input_seed)
{
    if(length < 2)
        return(NULL);
    if(length < 33)
    {
        t_bigint* tmp;
        t_bigint* prime_seed;
        t_bigint* c;
        t_bigint* one;
        t_bigint* mod;
        t_bigint* mask;

        size_t prime_gen_counter = 0;
        one = init_bigint(1);

        mod = init_bigint(1);
        bigint_left_shift(mod, length - 1);
        mask = bigint_sub(mod, one);


        prime_seed = copy_bigint(input_seed);
        while (1)
        {
            t_bigint* partials[2];
            for (size_t i = 0; i < 2; i++)
            {
                uint8_t* str_seed;
                size_t h_seed = bigint_to_string(&str_seed, prime_seed);
                uint8_t* hash = sha256(str_seed, h_seed);
                partials[i] = string_to_bigint((char*)hash, 256 / 8);
                free(str_seed);
                free(hash);
                bigint_add_int(prime_seed, 1);
            }
            c = bigint_xor(partials[0], partials[1]);
            tmp = bigint_and(c, mask);
            free_bigint(c);
            c = bigint_add(mod, tmp);
            free_bigint(tmp);
            bigint_right_shift(c, 1);
            tmp = bigint_add(c, c);
            free_bigint(c);
            c = tmp;
            bigint_add_int(c,1);
            prime_gen_counter++;

            free_bigint(partials[0]);
            free_bigint(partials[1]);

            int is_prime = bigint_miller_rabin(c, 100, false);
            if(is_prime < 0)
            {
                t_shawe_taylor* key = (t_shawe_taylor*)malloc(sizeof(t_shawe_taylor));
                key->prime = c;
                key->prime_seed = prime_seed;
                key->prime_gen_counter = prime_gen_counter;
                free_bigint(one);
                free_bigint(mod);
                free_bigint(mask);
                return(key);
            }
            if(prime_gen_counter > 4 * length)
            {
                free_bigint(c);
                free_bigint(one);
                free_bigint(mod);
                free_bigint(mask);
                free_bigint(prime_seed);
                return(NULL);
            }
            free_bigint(c);
        }
    }
    t_shawe_taylor* key = shawe_taylor((int)(ft_ceil((double)length / 2.0) + 1.0), input_seed);
    if(!key)
        return(NULL);
    return(st_generate_random_prime(length, key, NULL));
    
}