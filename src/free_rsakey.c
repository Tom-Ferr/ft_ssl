#include <ft_ssl.h>

void free_key(void* k, int mode)
{
    t_bigint** iter_key;

    iter_key = (t_bigint**)k;

    for (int i = 0; i < mode / 2; i++)
        free_bigint(iter_key[i]);
    free(k);
}