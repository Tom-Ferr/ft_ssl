#include <ft_ssl.h>

t_der* key_to_der(void* key, int mode)
{
    t_der* der;
    size_t* iter_der;
    t_bigint** iter_key;

    der = (t_der*)malloc(sizeof(t_der));

    iter_key = (t_bigint**)key;
    iter_der = (size_t*)der;

    for (int i = 0; i < mode; i += 2)
        iter_der[i+1] = bigint_to_string((uint8_t**)&iter_der[i], iter_key[i/2]);

    return(der);
}