#include <ft_ssl.h>

size_t parse_length(uint8_t** length)
{
    if(*(*length) & 0x80)
    {
        size_t ret = 0;
        size_t blocks = *(*length)++ & 0x7f;
        for (size_t i = 0; i < blocks; i++)
        {
           ret <<= 8;
           ret |= *(*length)++;
        }
        return(ret);
    }
    else
        return(*(*length)++);
}

void parse_integer(uint8_t** iter_der, uint8_t** der)
{
    iter_der[1] = (uint8_t*)parse_length(der);
    iter_der[0]  = (uint8_t*)malloc((size_t)iter_der[1]);
    for (size_t i = 0; i < (size_t)iter_der[1]; i++)
       iter_der[0][i] = *(*der)++;
    
}

void parse_der(t_der* p, uint8_t* der, size_t size, int mode)
{
    uint8_t** iter_der = (uint8_t**)p;
    uint8_t** iter_der_0 = iter_der;
    uint8_t* der_0 = der;
    uint8_t* checker;
    if(*der++ != 0x30)
    {
        write(2, "corrupted key\n", 14);
        exit(1);
    }
    p->sequence_length = parse_length(&der);
    checker = der;
    if(mode != PUBLIC)
    {
        der++;
        der++;
        der++;
    }
    for (size_t i = der - der_0; i < size; i = der - der_0)
    {
        if(*der++ == 0x02)
        {
            parse_integer(iter_der, &der);
            iter_der += 2;
        }
    }
    if((size_t)(der - checker) != p->sequence_length || iter_der - iter_der_0 != mode)
    {   free_der(p, iter_der - iter_der_0);
        write(2, "corrupted key\n", 14);
        exit(1);
    }
}