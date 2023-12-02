#include <ft_ssl.h>

void free_der(t_der* der, size_t mode)
{
    uint8_t** iter_der;

    iter_der = (uint8_t**)der;

    for (size_t i = 0; i < mode; i += 2)
        free(iter_der[i]);
    free(der);
    
}