#include <ft_ssl.h>

void* der_to_key(void* der, int mode)
{
    void* key;
    t_bigint** iter_key;
    uint8_t** iter_der;

    iter_der = (uint8_t**)der;
    key = (void*)malloc(sizeof(void*) * (mode / 2));
    iter_key = (t_bigint**)key;
    for (int i = 0; i < mode; i += 2)
        iter_key[i/2] = string_to_bigint((char*)iter_der[i], (size_t)iter_der[i+1]);
    return(key);
}